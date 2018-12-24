#include "sunrise_ble_sunriseService.h"

#include <strings.h>

#include "modules/nrfx/mdk/nrf.h"

#include "components/libraries/util/nordic_common.h"

#include "components/libraries/experimental_log/nrf_log.h"

#include "components/ble/common/ble_srv_common.h"

#include "components/softdevice/common/nrf_sdh.h"
#include "components/softdevice/common/nrf_sdh_ble.h"

#include "clock.h"

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

ret_code_t sunrise_ble_characteristic_init(uint16_t uuid, const char* name, rw_t rw, uint16_t unit, uint16_t format, uint16_t size, void* var, ble_dispatch_receiver_t* dispatch_receiver) {
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

	NRF_LOG_INFO("Svc handle is 0x%04x", p_service.service_handle);
	NRF_LOG_INFO("Chr handle is 0x%04x", dispatch_receiver->handles.value_handle);

	// prepend to LL
	dispatch_receiver->next = first;
	first = dispatch_receiver;

	return err_code;
}

static void sunrise_ble_sunriseService_on_ble_evt(ble_evt_t const * p_ble_evt, void* p_context) {
	NRF_LOG_INFO("Sunrise Event");

	switch (p_ble_evt->header.evt_id) {
		case BLE_GAP_EVT_CONNECTED: {
			p_service.conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
		} break;
		case BLE_GAP_EVT_DISCONNECTED: {
			p_service.conn_handle = BLE_CONN_HANDLE_INVALID;
		} break;
		case BLE_GATTS_EVT_WRITE: {
// 			if (p_evt_write->handle == p_service.timeservice_handle.value_handle) {
// 				const ble_gatts_evt_t* e       = &p_ble_evt->evt.gatts_evt;
// 				const ble_gatts_evt_write_t* w = &e->params.write;
//
// 				NRF_LOG_INFO("Sunrise write event: ch 0x%02x h 0x%02x offset %u len %u data %u", e->conn_handle, w->handle, w->offset, w->len, *((uint32_t*) w->data));
//
// 				clock_setseconds(*((uint32_t*) w->data));
// 			}
			const ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
			ble_dispatch_receiver_t* dr = first;
			while (dr) {
				if (dr->evt_hook) {
					if (dr->handles.value_handle == p_evt_write->handle) {
						NRF_LOG_INFO("Found hook for handle 0x%04x", p_evt_write->handle)
						dr->evt_hook(p_ble_evt, p_context);
					}
				}
				dr = dr->next;
			}
		} break;
		default:
			break;
	};
}

// static uint32_t sunrise_ble_sunriseService_add_characteristic() {
// 	ret_code_t err_code;
//
// 	sunrise_ble_characteristic_t time_service;
//
// 	static uint32_t time;
// 	time = clock_getseconds();
//
// 	static const char time_name_str[5] = "Time";
//
// 	err_code = sunrise_ble_characteristic_init(&time_service, 0xBEEF, time_name_str, RW_BOTH, BLE_UNIT_SECOND, BLE_GATT_CPF_FORMAT_UINT32, 4, &time, &p_service.timeservice_handle);
//
// // 	ble_uuid128_t base_uuid = SUNRISE_BLE_UUID128;
// //
// // 	ble_uuid_t char_uuid = {
// // 		.uuid = 0xBEEF
// // 	};
// //
// // 	err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
// // 	APP_ERROR_CHECK(err_code);
// //
// // 	ble_gatts_char_md_t char_md;
// // 	bzero(&char_md, sizeof(char_md));
// // 	char_md.char_props.read           = 1;
// // 	char_md.char_props.write          = 1;
// //
// // 	static const uint8_t sunrise_txt[4] = "Time";
// // 	char_md.p_char_user_desc = sunrise_txt;
// // 	char_md.char_user_desc_size = 4;
// // 	char_md.char_user_desc_max_size = 4;
// //
// // 	ble_gatts_char_pf_t char_pf;
// // 	bzero(&char_pf, sizeof(char_pf));
// // 	char_pf.unit   = BLE_UNIT_SECOND;
// // 	char_pf.format = BLE_GATT_CPF_FORMAT_UINT32;
// //
// // 	char_md.p_char_pf = &char_pf;
// //
// // // 	ble_gatts_attr_md_t cccd_md;
// // // 	bzero(&cccd_md, sizeof(cccd_md));
// // //
// // // 	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
// // // 	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
// // // 	cccd_md.vloc                = BLE_GATTS_VLOC_STACK;
// // //
// // // 	char_md.p_cccd_md           = &cccd_md;
// // // 	char_md.char_props.notify = 1;
// //
// // 	ble_gatts_attr_md_t attr_md;
// // 	bzero(&attr_md, sizeof(attr_md));
// // 	attr_md.vloc = BLE_GATTS_VLOC_STACK;
// // // 	attr_md.wr_auth = 1;
// //
// // 	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
// // 	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
// //
// // 	ble_gatts_attr_t    attr_char_value;
// // 	bzero(&attr_char_value, sizeof(attr_char_value));
// // 	attr_char_value.p_uuid      = &char_uuid;
// // 	attr_char_value.p_attr_md   = &attr_md;
// //
// // 	attr_char_value.max_len     = 4;
// // 	attr_char_value.init_len    = 4;
// // 	static uint32_t value;
// // 	value = clock_getseconds();
// // 	attr_char_value.p_value     = (uint8_t*) &value;
// //
// // 	err_code = sd_ble_gatts_characteristic_add(p_service.service_handle, &char_md, &attr_char_value, &p_service.char_handles);
// // 	APP_ERROR_CHECK(err_code);
// //
// // 	NRF_LOG_INFO("Svc handle is 0x%04x", p_service.service_handle);
// // 	NRF_LOG_INFO("Chr handle is 0x%04x", p_service.char_handles.value_handle);
//
// 	return err_code;
// }

void sunrise_ble_sunriseService_init() {
	ret_code_t err_code; // Variable to hold return codes from library and softdevice functions

	// FROM_SERVICE_TUTORIAL: Declare 16-bit service and 128-bit base UUIDs and add them to the BLE stack
	ble_uuid_t        service_uuid;
	ble_uuid128_t     base_uuid = SUNRISE_BLE_UUID128;
	service_uuid.uuid = 0xBEEE;
	err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
	APP_ERROR_CHECK(err_code);

	// OUR_JOB: Step 3.B, Set our service connection handle to default value. I.e. an invalid handle since we are not yet in a connection.
	p_service.conn_handle = BLE_CONN_HANDLE_INVALID;

	// FROM_SERVICE_TUTORIAL: Add our service
	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
										&service_uuid,
									 &p_service.service_handle);

	APP_ERROR_CHECK(err_code);

	// OUR_JOB: Call the function our_char_add() to add our new characteristic to the service.
// 	sunrise_ble_sunriseService_add_characteristic(&p_service);
	clock_ble_connect();

	NRF_SDH_BLE_OBSERVER(m_ble_observer, 3, sunrise_ble_sunriseService_on_ble_evt, NULL);
}
