#include "sunrise_ble_sunriseService.h"

#include <strings.h>

#include "modules/nrfx/mdk/nrf.h"

#include "components/libraries/util/nordic_common.h"

#include "components/libraries/log/nrf_log.h"

#include "components/ble/common/ble_srv_common.h"

#include "components/softdevice/common/nrf_sdh.h"
#include "components/softdevice/common/nrf_sdh_ble.h"

#include "sunrise_ble.h"

#include "clock.h"
#include "sunrise_mode.h"
#include "redshift_hook.h"
#include "redshift-location-sunrise.h"

#define SUNRISE_BLE_UUID128 {{ 0x86, 0xc6, 0xe6, 0x24, 0xbc, 0x3b, 0xcc, 0xcf, 0x3e, 0x1d, 0x57, 0xa2, 0x00, 0x00, 0x26, 0x78 }}

typedef struct {
	uint16_t conn_handle;
	uint16_t service_handle;
	ble_gatts_char_handles_t timeservice_handle;
} ble_os_t;

static ble_os_t p_service;

typedef struct {
	ble_uuid_t          char_uuid;
	ble_gatts_char_md_t char_md;
	ble_gatts_char_pf_t char_pf;
	ble_gatts_attr_md_t attr_md;
	ble_gatts_attr_t    attr_char_value;
} sunrise_ble_characteristic_t;

ble_dispatch_receiver_t* first = NULL;

ret_code_t sunrise_ble_characteristic_init(uint16_t uuid, const char* name, rw_t rw, uint16_t unit, uint16_t format, int8_t exponent, uint16_t size, void* var, ble_dispatch_receiver_t* dispatch_receiver) {
	ret_code_t err_code;

	ble_uuid128_t base_uuid = SUNRISE_BLE_UUID128;

	sunrise_ble_characteristic_t c;
	bzero(&c, sizeof(c));

	c.char_uuid.uuid = uuid;

	err_code = sd_ble_uuid_vs_add(&base_uuid, &c.char_uuid.type);
	APP_ERROR_CHECK(err_code);

	if ((rw == RW_READ) || (rw == RW_BOTH))
		c.char_md.char_props.read           = 1;
	if ((rw == RW_WRITE) || (rw == RW_BOTH))
		c.char_md.char_props.write          = 1;

	c.char_md.p_char_user_desc = (const uint8_t*) name;
	c.char_md.char_user_desc_size = strlen(name);
	c.char_md.char_user_desc_max_size = strlen(name);

	c.char_pf.unit   = unit;
	c.char_pf.format = format;
	c.char_pf.exponent = exponent;

	c.char_md.p_char_pf = &c.char_pf;

	// 	ble_gatts_attr_md_t cccd_md;
	// 	bzero(&cccd_md, sizeof(cccd_md));
	//
	// 	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	// 	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	// 	cccd_md.vloc                = BLE_GATTS_VLOC_STACK;
	//
	// 	char_md.p_cccd_md           = &cccd_md;
	// 	char_md.char_props.notify   = 1;

	c.attr_md.vloc = BLE_GATTS_VLOC_USER;

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&c.attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&c.attr_md.write_perm);

	c.attr_char_value.p_uuid      = &c.char_uuid;
	c.attr_char_value.p_attr_md   = &c.attr_md;

	c.attr_char_value.max_len     = size;
	c.attr_char_value.init_len    = size;
	c.attr_char_value.p_value     = (uint8_t*) var;

	err_code = sd_ble_gatts_characteristic_add(p_service.service_handle, &c.char_md, &c.attr_char_value, &dispatch_receiver->handles);
	APP_ERROR_CHECK(err_code);

// 	NRF_LOG_INFO("Chr handle is 0x%04x", dispatch_receiver->handles.value_handle);

	// Only store hook in LL if it's non-null
	// remove this condition if we decide to throw an error in the event handler for unknown handles
	if (dispatch_receiver->evt_hook) {
		if (dispatch_receiver != first) {
			// prepend to LL
			dispatch_receiver->next = first;
			first = dispatch_receiver;
		}
	}

	return err_code;
}

static void sunrise_ble_sunriseService_on_ble_evt(ble_evt_t const * p_ble_evt, void* p_context) {
// 	NRF_LOG_INFO("Sunrise Event");

	switch (p_ble_evt->header.evt_id) {
		case BLE_GAP_EVT_CONNECTED: {
			p_service.conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
		} break;
		case BLE_GAP_EVT_DISCONNECTED: {
			p_service.conn_handle = BLE_CONN_HANDLE_INVALID;
		} break;
		case BLE_GATTS_EVT_WRITE: {
			const ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
			ble_dispatch_receiver_t* dr = first;
			int timeout = 16;
			while (dr && (--timeout > 0)) {
// 				NRF_LOG_INFO("handle 0x%04x at %p has handler %p and next %p", dr->handles.value_handle, dr, dr->evt_hook, dr->next);
				if (dr->evt_hook) {
					if (dr->handles.value_handle == p_evt_write->handle) {
// 						NRF_LOG_INFO("Found hook for handle 0x%04x", p_evt_write->handle)
						dr->evt_hook(p_ble_evt, p_context);
						break;
					}
				}
				dr = dr->next;
			}
			if (timeout == 0)
				NRF_LOG_ERROR("Error! BLE dispatch LL is corrupted!");
		} break;
		default:
			break;
	};
}

void sunrise_ble_sunriseService_init() {
	ret_code_t err_code; // Variable to hold return codes from library and softdevice functions

	// FROM_SERVICE_TUTORIAL: Declare 16-bit service and 128-bit base UUIDs and add them to the BLE stack
	ble_uuid_t        service_uuid;
	ble_uuid128_t     base_uuid = SUNRISE_BLE_UUID128;
	service_uuid.uuid = SUNRISE_BLE_UUID_SERVICE;

	err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
	APP_ERROR_CHECK(err_code);

	// OUR_JOB: Step 3.B, Set our service connection handle to default value. I.e. an invalid handle since we are not yet in a connection.
	p_service.conn_handle = BLE_CONN_HANDLE_INVALID;

	// FROM_SERVICE_TUTORIAL: Add our service
	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
										&service_uuid,
									 &p_service.service_handle);
// 	NRF_LOG_INFO("Svc handle is 0x%04x", p_service.service_handle);

	APP_ERROR_CHECK(err_code);

/*
 * *** Invoke child characteristics ***
 */
	APP_ERROR_CHECK(clock_ble_connect());
	APP_ERROR_CHECK(sunrise_mode_ble_connect());
	APP_ERROR_CHECK(redshift_sunrise_ble_connect());
	APP_ERROR_CHECK(redshift_ble_init());

	NRF_SDH_BLE_OBSERVER(m_ble_observer, 3, sunrise_ble_sunriseService_on_ble_evt, NULL);
}
