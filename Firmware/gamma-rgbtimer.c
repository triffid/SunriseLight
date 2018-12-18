#include "gamma-rgbtimer.h"

#include "components/libraries/experimental_log/nrf_log.h"

#include "redshift/src/colorramp.h"
#include "rgbtimer.h"

#define _(s) s
#define printf(...) NRF_LOG_INFO(__VA_ARGS__)

static int
gamma_rgbtimer_init(void **state)
{
	*state = NULL;
	return 0;
}

static int
gamma_rgbtimer_start(void *state)
{
	return 0;
}

static void
gamma_rgbtimer_restore(void *state)
{
}

static void
gamma_rgbtimer_free(void *state)
{
}

static void
gamma_rgbtimer_print_help(FILE *f)
{
}

static int
gamma_rgbtimer_set_option(void *state, const char *key, const char *value)
{
	return -1;
}

static int
gamma_rgbtimer_set_temperature(
	void *state, const color_setting_t *setting, int preserve)
{
	printf(_("Temperature: %i"), setting->temperature);

	float r = 1, g = 1, b = 1;

	colorramp_fill_float(&r, &g, &b, 1, setting);

	printf(_("RGB: %u%% %u%% %u%%"), (int) (r * 100), (int) (g * 100), (int) (b * 100));

	rgbtimer_setrgb(r, g * 0.33f, b * 0.33f);

	return 0;
}


const gamma_method_t rgbtimer_gamma_method = {
	"dummy", 0,
	(gamma_method_init_func *)gamma_rgbtimer_init,
	(gamma_method_start_func *)gamma_rgbtimer_start,
	(gamma_method_free_func *)gamma_rgbtimer_free,
	(gamma_method_print_help_func *)gamma_rgbtimer_print_help,
	(gamma_method_set_option_func *)gamma_rgbtimer_set_option,
	(gamma_method_restore_func *)gamma_rgbtimer_restore,
	(gamma_method_set_temperature_func *)gamma_rgbtimer_set_temperature
};
