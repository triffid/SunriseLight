#ifndef SUNRISE_BLE_SUNRISESERVICE_H
#define SUNRISE_BLE_SUNRISESERVICE_H

#include "components/ble/common/ble_srv_common.h"

void sunrise_ble_sunriseService_init(void);

void sunrise_ble_sunriseService_on_ble_evt(ble_evt_t const * p_ble_evt, void* p_context);

#endif /* SUNRISE_BLE_SUNRISESERVICE_H */
