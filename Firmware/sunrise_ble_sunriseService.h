#ifndef SUNRISE_BLE_SUNRISESERVICE_H
#define SUNRISE_BLE_SUNRISESERVICE_H

#include "components/softdevice/s132/headers/ble.h"
#include "components/libraries/util/app_error.h"

typedef enum {
	RW_NONE,
	RW_READ,
	RW_WRITE,
	RW_BOTH
} rw_t;

struct _ble_dispatch_receiver_t;
typedef struct _ble_dispatch_receiver_t {
	ble_gatts_char_handles_t handles;
	void (*evt_hook)(ble_evt_t const *, void*);
	struct _ble_dispatch_receiver_t* next;
} ble_dispatch_receiver_t;

void sunrise_ble_sunriseService_init(void);

ret_code_t sunrise_ble_characteristic_init(uint16_t uuid, const char* name, rw_t rw, uint16_t unit, uint16_t format, int8_t exponent, uint16_t size, void* var, ble_dispatch_receiver_t* receiver);

#define ADD_CHARACTERISTIC(desc, uuid, name, rw, unit, format, exponent, size, var, hookfn) \
	static const char desc ## _name_str[] = name; \
	static ble_dispatch_receiver_t desc ## _dispatch_receiver = {.evt_hook = hookfn,}; \
	APP_ERROR_CHECK(sunrise_ble_characteristic_init(uuid, desc ## _name_str, rw, unit, format, exponent, size, &var, & desc ## _dispatch_receiver));

#endif /* SUNRISE_BLE_SUNRISESERVICE_H */
