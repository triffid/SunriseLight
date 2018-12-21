#include "Sunrise_State.h"

#include <strings.h>

#include "modules/nrfx/hal/nrf_gpio.h"
#include "components/libraries/bsp/bsp.h"
#include "components/libraries/experimental_log/nrf_log.h"

#include "clock.h"

#define NUM_HANDLERS 8

static State_t _state = OFF;

static const char * const statenames[3] = { "OFF", "SOFT_OFF", "ON" };

Sunrise_StateChangeHandler handlers[NUM_HANDLERS];

static void Sunrise_StateChange(State_t new) {
	if (new == _state)
		return;
	if (new > ON)
		return;

	NRF_LOG_INFO("State change from %s to %s", statenames[_state], statenames[new]);

	for (int i = 0; i < NUM_HANDLERS; i++) {
		if (handlers[i])
			handlers[i](new);
	}
	_state = new;
}

void Sunrise_State_init(void) {
	nrf_gpio_cfg_input(VMEAS, NRF_GPIO_PIN_NOPULL);

	bzero(handlers, sizeof(handlers));
}

void Sunrise_State_poll(void) {
	if (nrf_gpio_pin_read(VMEAS) == 0) {
		Sunrise_StateChange(OFF);
		NRF_LOG_INFO("No power, time is %u", clock_getseconds());
	}
	else if (_state == OFF)
		Sunrise_StateChange(ON);
	else {
		// TODO: handle GPIOTE button events
		// button should change between SOFT_OFF and ON
	}
}

State_t Sunrise_State(void) {
	return _state;
}

// #define THROW_ERROR(err) printf("Register Handler failed: " err " at %d %s", __LINE__, __FILE__)
// #define THROW_ERROR(err) for (;;)
// #define THROW_ERROR(err) app_error_handler(err, __LINE__, __FILE__)
// #define THROW_ERROR(err) __breakpoint()

void Sunrise_State_RegisterStateChangeHandler(Sunrise_StateChangeHandler handler) {
	for (int i = 0; i < NUM_HANDLERS; i++) {
		if (handlers[i] == NULL) {
			handlers[i] = handler;
			return;
		}
	}
	NRF_LOG_ERROR("RegisterStateChangeHandler: no more slots");
}
