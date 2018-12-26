#include "sunrise_mode.h"

#include <math.h>

#include "components/libraries/experimental_log/nrf_log.h"

#include "Sunrise_State.h"
#include "rgbtimer.h"
#include "util.h"

#include "sunrise_ble.h"
#include "sunrise_ble_sunriseService.h"

static sunrise_mode_t _mode = SUNRISE_MODE_OFF;
static mode_change_notifier_hook_t* _first = NULL;
static uint32_t _ble_mode;

static void sunrise_mode_state_receiver(State_t newstate) {
	static sunrise_mode_t prev_mode = SUNRISE_MODE_SUNRISE;
	if (newstate == OFF) {
		prev_mode = _mode;
		sunrise_mode_set(SUNRISE_MODE_OFF);
	}
	else {
		sunrise_mode_set(prev_mode);
	}
}

void sunrise_mode_init() {
	Sunrise_State_RegisterStateChangeHandler(&sunrise_mode_state_receiver);
}

void sunrise_mode_set(sunrise_mode_t newmode) {
	if (_mode == newmode)
		return;

	if (newmode >= SUNRISE_MODE_INVALID)
		return;

	NRF_LOG_INFO("Changing mode from %d to %d", _mode, newmode);

	for (mode_change_notifier_hook_t* j = _first; j; j = j->next)
		j->callback(newmode);

	_mode = newmode;
	_ble_mode = newmode;
}

sunrise_mode_t sunrise_mode_get() {
	return _mode;
}

void sunrise_mode_register_callback(mode_change_notifier_hook_t* hookstr) {
	if (hookstr == _first)
		return;

	hookstr->next = _first;
	_first = hookstr;
}

void sunrise_mode_ble_evt_handler(ble_evt_t const * p_ble_evt, void* p_context) {
	NRF_LOG_INFO("Mode: BLE hook!");

	switch (p_ble_evt->header.evt_id) {
		case BLE_GATTS_EVT_WRITE: {
			const ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
			NRF_LOG_INFO("BLE: got mode %u (%u, %0x%08x)\n", p_evt_write->data[0], *((uint32_t*) p_evt_write->data), *((uint32_t*) p_evt_write->data));
			sunrise_mode_set(p_evt_write->data[0]);
			if (_mode == SUNRISE_MODE_MANUAL)
				rgbtimer_sethsv(p_evt_write->data[1] * (M_TWOPIF / 255.0f), p_evt_write->data[2] / 255.0f, p_evt_write->data[3] / 255.0f);
		} break;
	}
}

ret_code_t sunrise_mode_ble_connect() {
	ADD_CHARACTERISTIC(mode, SUNRISE_BLE_UUID_MODE, "Mode", RW_BOTH, BLE_UNIT_UNITLESS, BLE_GATT_CPF_FORMAT_UINT32, 0, 4, _ble_mode, sunrise_mode_ble_evt_handler);

	return NRF_SUCCESS;
}
