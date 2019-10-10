#include "clock.h"

#include <sys/time.h>

#include "components/libraries/timer/app_timer.h"
#include "components/libraries/atomic/nrf_atomic.h"
#include "integration/nrfx/legacy/nrf_drv_clock.h"
#include "components/libraries/log/nrf_log.h"

#include "sunrise_ble.h"

#include "sunrise_ble_sunriseService.h"

static nrf_atomic_flag_t timer_timeout_flag = 0;
static nrf_atomic_u32_t timer_seconds = START_DATE;

APP_TIMER_DEF(m_app_timer_id);

static void timer_event_handler(nrf_drv_clock_evt_type_t event) {
}

static void timer_timeout_handler(void* p_context) {
	nrf_atomic_flag_set(&timer_timeout_flag);
	nrf_atomic_u32_add(&timer_seconds, 1);
}

// stub to override default library one
int _gettimeofday(struct timeval* tv, void* tz) {
	if (tv) {
		tv->tv_sec = timer_seconds;
		tv->tv_usec = 0;
	}
	return 0;
}

void clock_init() {
	// Create timers.
	ret_code_t err_code;

	err_code = app_timer_create(&m_app_timer_id, APP_TIMER_MODE_REPEATED, timer_timeout_handler);
	APP_ERROR_CHECK(err_code);

	if (nrf_drv_clock_init_check() == false) {
		err_code = nrf_drv_clock_init();
		APP_ERROR_CHECK(err_code);
	}

	static nrf_drv_clock_handler_item_t lfclk_req = {NULL, timer_event_handler};
	nrf_drv_clock_lfclk_request(&lfclk_req);

	err_code = app_timer_start(m_app_timer_id, 32768, NULL);
	APP_ERROR_CHECK(err_code);
}

unsigned clock_getseconds(void) {
	return timer_seconds;
}

void clock_setseconds(unsigned s) {
	nrf_atomic_u32_store(&timer_seconds, s);
}

void clock_offset(int o) {
	nrf_atomic_u32_add(&timer_seconds, o);
}

bool clock_secondsflag() {
	return nrf_atomic_flag_clear_fetch(&timer_timeout_flag);
}

/*
 **** BLE stuff ****
 */

void clock_ble_evt_handler(ble_evt_t const * p_ble_evt, void* p_context) {
	NRF_LOG_INFO("Clock: BLE hook!");
}

ret_code_t clock_ble_connect() {
	ADD_CHARACTERISTIC(clock, SUNRISE_BLE_UUID_CLOCK, "Time", RW_BOTH, BLE_UNIT_TIME_SECOND, BLE_GATT_CPF_FORMAT_UINT32, 0, 4, *((uint32_t *) &timer_seconds), clock_ble_evt_handler);

	return NRF_SUCCESS;
}
