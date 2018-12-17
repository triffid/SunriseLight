#include "sigmadelta.h"

#include "modules/nrfx/hal/nrf_gpio.h"

void SigmaDelta_init(SigmaDelta_t* self, uint32_t pin, int max, int value) {
	nrf_gpio_cfg_output(pin);
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

	if (nrf_gpio_pin_out_read(self->pin) == 0) {
		self->accumulator += self->value;
		if (self->accumulator >= (self->max >> 1))
			nrf_gpio_pin_set(self->pin);
	}
	else {
		self->accumulator -= (self->max - self->value);
		if (self->accumulator <= 0)
			nrf_gpio_pin_clear(self->pin);
	}
}
