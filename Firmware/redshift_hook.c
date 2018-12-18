#include "redshift.h"

#include <math.h>

#include "redshift/src/systemtime.h"
#include "redshift/src/solar.h"
#include "redshift/src/colorramp.h"
#include "redshift/src/location-manual.h"

#include "components/libraries/experimental_log/nrf_log.h"

#include "gamma-rgbtimer.h"

#define N_(s) s
#define _(s) s
#define gettext(s) s
#define printf(...) NRF_LOG_INFO(__VA_ARGS__)
#define fputs(s, f) NRF_LOG_ERROR(s)
#define CLAMP(lo,mid,up)  (((lo) > (mid)) ? (lo) : (((mid) < (up)) ? (mid) : (up)))

/* Duration of sleep between screen updates (milliseconds). */
#define SLEEP_DURATION        5000
#define SLEEP_DURATION_SHORT  100

/* Length of fade in numbers of short sleep durations. */
#define FADE_LENGTH  40

/* Save previous parameters so we can avoid printing status updates if
 *	   the values did not change. */
static period_t prev_period = PERIOD_NONE;

/* Previous target color setting and current actual color setting.
 *	   Actual color setting takes into account the current color fade. */
static color_setting_t prev_target_interp;
static color_setting_t interp;

static location_t loc = { NAN, NAN };

/* Short fade parameters */
static int fade_length = 0;
static int fade_time = 0;
static color_setting_t fade_start_interp;

static transition_scheme_t scheme;
static const gamma_method_t* method = &rgbtimer_gamma_method;
static gamma_state_t* method_state;

static location_state_t* location_state;
static const location_provider_t* provider = &manual_location_provider;

/* Names of periods of day */
static const char *period_names[] = {
	/* TRANSLATORS: Name printed when period of day is unknown */
	N_("None"),
	N_("Daytime"),
	N_("Night"),
	N_("Transition")
};

/* Determine which period we are currently in based on time offset. */
static period_t
get_period_from_time(const transition_scheme_t *transition, int time_offset)
{
	if (time_offset < transition->dawn.start ||
		time_offset >= transition->dusk.end) {
		return PERIOD_NIGHT;
		} else if (time_offset >= transition->dawn.end &&
			time_offset < transition->dusk.start) {
			return PERIOD_DAYTIME;
			} else {
				return PERIOD_TRANSITION;
			}
}

/* Determine which period we are currently in based on solar elevation. */
static period_t
get_period_from_elevation(
	const transition_scheme_t *transition, double elevation)
{
	if (elevation < transition->low) {
		return PERIOD_NIGHT;
	} else if (elevation < transition->high) {
		return PERIOD_TRANSITION;
	} else {
		return PERIOD_DAYTIME;
	}
}

/* Determine how far through the transition we are based on time offset. */
static double
get_transition_progress_from_time(
	const transition_scheme_t *transition, int time_offset)
{
	if (time_offset < transition->dawn.start ||
		time_offset >= transition->dusk.end) {
		return 0.0;
		} else if (time_offset < transition->dawn.end) {
			return (transition->dawn.start - time_offset) /
			(double)(transition->dawn.start -
			transition->dawn.end);
		} else if (time_offset > transition->dusk.start) {
			return (transition->dusk.end - time_offset) /
			(double)(transition->dusk.end -
			transition->dusk.start);
		} else {
			return 1.0;
		}
}

/* Determine how far through the transition we are based on elevation. */
static double
get_transition_progress_from_elevation(
	const transition_scheme_t *transition, double elevation)
{
	if (elevation < transition->low) {
		return 0.0;
	} else if (elevation < transition->high) {
		return (transition->low - elevation) /
		(transition->low - transition->high);
	} else {
		return 1.0;
	}
}

/* Return number of seconds since midnight from timestamp. */
static int
get_seconds_since_midnight(double timestamp)
{
	time_t t = (time_t)timestamp;
	struct tm tm;
	localtime_r(&t, &tm);
	return tm.tm_sec + tm.tm_min * 60 + tm.tm_hour * 3600;
}

/* Print verbose description of the given period. */
static void
print_period(period_t period, double transition)
{
	switch (period) {
		case PERIOD_NONE:
		case PERIOD_NIGHT:
		case PERIOD_DAYTIME:
			printf(_("Period: %s"), gettext(period_names[period]));
			break;
		case PERIOD_TRANSITION:
			printf(_("Period: %s (%d%% day)"),
				   gettext(period_names[period]),
				   transition*100);
			break;
	}
}

/* Interpolate color setting structs given alpha. */
static void
interpolate_color_settings(
	const color_setting_t *first,
	const color_setting_t *second,
	double alpha,
	color_setting_t *result)
{
	alpha = CLAMP(0.0, alpha, 1.0);

	result->temperature = (1.0-alpha)*first->temperature + alpha*second->temperature;
	result->brightness = (1.0-alpha)*first->brightness + alpha*second->brightness;
	for (int i = 0; i < 3; i++) {
		result->gamma[i] = (1.0-alpha)*first->gamma[i] + alpha*second->gamma[i];
	}
}

/* Interpolate color setting structs transition scheme. */
static void
interpolate_transition_scheme(
	const transition_scheme_t *transition,
	double alpha,
	color_setting_t *result)
{
	const color_setting_t *day = &transition->day;
	const color_setting_t *night = &transition->night;

	alpha = CLAMP(0.0, alpha, 1.0);
	interpolate_color_settings(night, day, alpha, result);
}

/* Return 1 if color settings have major differences, otherwise 0.
 *  Used to determine if a fade should be applied in continual mode. */
static int
color_setting_diff_is_major(
	const color_setting_t *first,
	const color_setting_t *second)
{
	return (abs(first->temperature - second->temperature) > 25 ||
	fabsf(first->brightness - second->brightness) > 0.1 ||
	fabsf(first->gamma[0] - second->gamma[0]) > 0.1 ||
	fabsf(first->gamma[1] - second->gamma[1]) > 0.1 ||
	fabsf(first->gamma[2] - second->gamma[2]) > 0.1);
}

/* Reset color setting to default values. */
static void
color_setting_reset(color_setting_t *color)
{
	color->temperature = NEUTRAL_TEMP;
	color->gamma[0] = 0.0;
	color->gamma[1] = 0.0;
	color->gamma[2] = 0.0;
	color->brightness = 0.0;
}

/* Wait for location to become available from provider.
 *  Waits until timeout (milliseconds) has elapsed or forever if timeout
 *  is -1. Writes location to loc. Returns -1 on error,
 *  0 if timeout was reached, 1 if location became available. */
static int
provider_get_location(
	const location_provider_t *provider, location_state_t *state,
	int timeout, location_t *loc)
{
	int available = 0;
	provider->handle(state, loc, &available);

	return 1;
}

/* Easing function for fade.
 *   See https://github.com/mietek/ease-tween */
static double
ease_fade(double t)
{
	if (t <= 0) return 0;
	if (t >= 1) return 1;
	return 1.0042954579734844 * exp(
		-6.4041738958415664 * exp(-7.2908241330981340 * t));
}

void redshift_init() {
	color_setting_reset(&prev_target_interp);
	color_setting_reset(&interp);

	/* Get initial location from provider */
	provider->init(&location_state);
	provider->set_option(location_state, "lat", "22.5433782");
	provider->set_option(location_state, "lon", "114.0782833");
	provider_get_location(provider, location_state, -1, &loc);

	scheme.low = SOLAR_CIVIL_TWILIGHT_ELEV;
	scheme.high = 3.0f;
	scheme.use_time = 0;
	scheme.day.temperature = 7400;
	scheme.night.temperature = 3000;
	scheme.day.brightness = 0.5;
	scheme.night.brightness = 0.5;
	scheme.day.gamma[0] = 0.45;
	scheme.day.gamma[1] = 0.45;
	scheme.day.gamma[2] = 0.45;
	scheme.night.gamma[0] = 0.45;
	scheme.night.gamma[1] = 0.45;
	scheme.night.gamma[2] = 0.45;

	method->init(&method_state);
}


static void do_redshift_update(const transition_scheme_t *scheme,
                               const gamma_method_t *method,
                               gamma_state_t *method_state,
                               int use_fade, int preserve_gamma, int verbose) {

	/* Read timestamp */
	double now;
	int r = systemtime_get_time(&now);
	if (r < 0) {
		fputs(_("Unable to read system time."), stderr);
		return;
	}

	do {
		location_t* loc = (location_t*) location_state;
		unsigned t = now;
		unsigned adj = t + ((int) roundf(loc->lon * 12 / 180)) * 3600; // approximate timezone compensation
		printf("Time is %u = %d:%02d:%02d", t, (adj % 86400) / 3600, ((adj % 86400) % 3600) / 60, ((adj % 86400) % 60));
	} while (0);

	period_t period;
	double transition_prog;
	if (scheme->use_time) {
		int time_offset = get_seconds_since_midnight(now);

		period = get_period_from_time(scheme, time_offset);
		transition_prog = get_transition_progress_from_time(scheme, time_offset);
	} else {
		/* Current angular elevation of the sun */
		double elevation = solar_elevation(now, loc.lat, loc.lon);

		do {
			NRF_LOG_INFO("Elevation is " NRF_LOG_FLOAT_MARKER, NRF_LOG_FLOAT(elevation));
		} while (0);

		period = get_period_from_elevation(scheme, elevation);
		transition_prog = get_transition_progress_from_elevation(scheme, elevation);
	}

	/* Use transition progress to get target color
	 *  temperature. */
	color_setting_t target_interp;
	interpolate_transition_scheme(scheme, transition_prog, &target_interp);

	/* Print period if it changed during this update,
	 *  or if we are in the transition period. In transition we
	 *  print the progress, so we always print it in
	 *  that case. */
	if (verbose && (period != prev_period || period == PERIOD_TRANSITION)) {
		print_period(period, transition_prog);
	}

	/* Activate hooks if period changed */
	if (period != prev_period) {
// 		hooks_signal_period_change(prev_period, period);
	}

	/* Start fade if the parameter differences are too big to apply
	 *  instantly. */
	if (use_fade) {
		if (
			(
				fade_length == 0 &&
				color_setting_diff_is_major(
					&interp,
					&target_interp
				)
			) || (
				fade_length != 0 &&
				color_setting_diff_is_major(
					&target_interp,
					&prev_target_interp
				)
			)
		) {
			fade_length = FADE_LENGTH;
			fade_time = 0;
			fade_start_interp = interp;
		}
	}

	/* Handle ongoing fade */
	if (fade_length != 0) {
		fade_time += 1;
		double frac = fade_time / (double)fade_length;
		double alpha = CLAMP(0.0, ease_fade(frac), 1.0);

		interpolate_color_settings(&fade_start_interp, &target_interp, alpha, &interp);

		if (fade_time > fade_length) {
			fade_time = 0;
			fade_length = 0;
		}
	} else {
		interp = target_interp;
	}

	if (verbose) {
		if (prev_target_interp.temperature != target_interp.temperature) {
			printf(_("Color temperature: %uK"), target_interp.temperature);
		}
		if (prev_target_interp.brightness != target_interp.brightness) {
			printf(_("Brightness: %d%%"), target_interp.brightness * 100);
		}
	}

	/* Adjust temperature */
	r = method->set_temperature(method_state, &interp, preserve_gamma);
	if (r < 0) {
		fputs(_("Temperature adjustment failed."), stderr);
		return;
	}

	/* Save period and target color setting as previous */
	prev_period = period;
	prev_target_interp = target_interp;
}

void redshift_update() {
	do_redshift_update(&scheme, method, method_state, 0, 0, 1);
}
