#include "clock.h"

#include <sys/time.h>

#include "components/libraries/timer/app_timer.h"
#include "components/libraries/atomic/nrf_atomic.h"
#include "integration/nrfx/legacy/nrf_drv_clock.h"
#include "components/libraries/experimental_log/nrf_log.h"

#include "sunrise_ble_sunriseService.h"

static nrf_atomic_flag_t timer_timeout_flag = 0;
static nrf_atomic_u32_t timer_seconds = START_DATE;

APP_TIMER_DEF(m_app_timer_id);

static void timer_event_handler(nrf_drv_clock_evt_type_t event) {
}

static void timer_timeout_handler(void* p_context) {
	// 	static unsigned angle = 0;
	// 	angle = (angle + 1) % 60;
	// 	rgbtimer_sethsv(angle * M_PI_F / 30, 1, 0.125f);
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
	#define BLE_UNIT_SECOND 0x2703

	static const char time_name_str[5] = "Time";
	static ble_dispatch_receiver_t dispatch_reciever = {
		.evt_hook = clock_ble_evt_handler,
		.next = NULL
	};

	return sunrise_ble_characteristic_init(0xBEEF, time_name_str, RW_BOTH, BLE_UNIT_SECOND, BLE_GATT_CPF_FORMAT_UINT32, 4, &timer_seconds, &dispatch_reciever);
}
