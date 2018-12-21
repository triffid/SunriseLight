#include "rgbtimer.h"

#include <math.h>

#include "components/libraries/bsp/bsp_btn_ble.h"
#include "components/libraries/experimental_log/nrf_log.h"

#include "modules/nrfx/mdk/nrf.h"
#include "modules/nrfx/hal/nrf_gpio.h"
#include "modules/nrfx/hal/nrf_timer.h"

#include "sigmadelta.h"
#include "Sunrise_State.h"

#define RESOLUTION 4096

SigmaDelta_t blue  = { LED_BLUE,  0, RESOLUTION, 0 };
SigmaDelta_t red   = { LED_RED,   0, RESOLUTION, 0 };
SigmaDelta_t green = { LED_GREEN, 0, RESOLUTION, 0 };

typedef struct __attribute__ ((packed)) {
	unsigned red;
	unsigned green;
	unsigned blue;
} rgb;

void hsv2rgb(rgb* o, float h, float s, float v);


void rgbtimer_StateChangeHandler(State_t new) {
	if (new == ON)
		rgbtimer_start();
	else
		rgbtimer_stop();
}

void rgbtimer_init() {
	NVIC_SetPriority(    TIMER1_IRQn, 15); // Lowest priority
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(      TIMER1_IRQn);

	nrf_timer_mode_set(     NRF_TIMER1, NRF_TIMER_MODE_TIMER);
	nrf_timer_bit_width_set(NRF_TIMER1, NRF_TIMER_BIT_WIDTH_8);
	nrf_timer_frequency_set(NRF_TIMER1, NRF_TIMER_FREQ_500kHz);
	nrf_timer_cc_write(     NRF_TIMER1, NRF_TIMER_CC_CHANNEL0, 2);

	SigmaDelta_init(&red);
	SigmaDelta_init(&green);
	SigmaDelta_init(&blue);

	Sunrise_State_RegisterStateChangeHandler(&rgbtimer_StateChangeHandler);
}

void rgbtimer_setrgb(float r, float g, float b) {
	if (r > 1) r = 1;
	if (r < 0) r = 0;
	if (g > 1) g = 1;
	if (g < 0) g = 0;
	if (b > 1) b = 1;
	if (b < 0) b = 0;
	red.value   = r * RESOLUTION;
	green.value = g * RESOLUTION;
	blue.value  = b * RESOLUTION;
}

void rgbtimer_sethsv(float h, float s, float v) {
	rgb z;
	hsv2rgb(&z, h, s, v);
	red.value   = z.red;
	green.value = z.green;
	blue.value  = z.blue;
	NRF_LOG_INFO("RGB: %d %d %d", z.red, z.green, z.blue);
}

void rgbtimer_start() {
	nrf_timer_int_enable(   NRF_TIMER1, NRF_TIMER_INT_COMPARE0_MASK );
	nrf_timer_task_trigger( NRF_TIMER1, NRF_TIMER_TASK_START);
}

void rgbtimer_stop() {
	nrf_timer_int_disable(  NRF_TIMER1, NRF_TIMER_INT_COMPARE0_MASK );
	nrf_timer_task_trigger( NRF_TIMER1, NRF_TIMER_TASK_STOP);
	bsp_board_leds_off();
}

void TIMER1_IRQHandler(void) {
	SigmaDelta_run(&blue);
	SigmaDelta_run(&red);
	SigmaDelta_run(&green);

	nrf_timer_event_clear(NRF_TIMER1, NRF_TIMER_EVENT_COMPARE0);
	nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CLEAR);
}

// from https://stackoverflow.com/a/34407200
void hsv2rgb(rgb* o, float h, float s, float v) {
	h = fmod(h, 2 * ((float) M_PI));
	float r, g, b, p, q, t, fract;
	float h_ = h * 3 / ((float) M_PI);
	fract = h_ - floor(h_);
	p = v * (1.0f - s);
	q = v * (1.0f - (s * fract));
	t = v * (1.0f - (s * (1.0f - fract)));
	if (h_ < 1) {
		r = v;
		g = t;
		b = p;
	}
	else if (h_ < 2) {
		r = q;
		g = v;
		b = p;
	}
	else if (h_ < 3) {
		r = p;
		g = v;
		b = t;
	}
	else if (h_ < 4) {
		r = p;
		g = q;
		b = v;
	}
	else if (h_ < 5) {
		r = t;
		g = p;
		b = v;
	}
	else { // h <= 6
		r = v;
		g = p;
		b = q;
	}
	o->red   = (r < 0)?0:(r > 1)?RESOLUTION:r * RESOLUTION;
	o->green = (g < 0)?0:(g > 1)?RESOLUTION:g * RESOLUTION;
	o->blue  = (b < 0)?0:(b > 1)?RESOLUTION:b * RESOLUTION;
}
