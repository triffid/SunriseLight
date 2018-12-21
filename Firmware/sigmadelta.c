#include "sigmadelta.h"

#include "modules/nrfx/hal/nrf_gpio.h"

void SigmaDelta_init(SigmaDelta_t* self) {
	nrf_gpio_cfg_output(self->pin);
}

void SigmaDelta_set(SigmaDelta_t* self, int value) {
	// sanity check
	if (value > self->max)
		value = self->max;
	if (value < 0)
		value = 0;

	// update
	self->value = value;
}

void SigmaDelta_run(SigmaDelta_t* self) {
	// skip sanity check for speed

	// accumulate value
	self->accumulator += self->value;

	// if we didn't overflow, set output low
	if (self->accumulator < self->max) {
		nrf_gpio_pin_clear(self->pin);
	}
	// if we overflow, modulo accumulator and set output high
	else {
		self->accumulator -= self->max;
		nrf_gpio_pin_set(self->pin);
	}
}
