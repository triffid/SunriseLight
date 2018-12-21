#include "sunrise_ble_dfu.h"

#include "components/boards/boards.h"
#include "components/ble/ble_services/ble_dfu/ble_dfu.h"
#include "components/libraries/experimental_log/nrf_log.h"
#include "components/libraries/pwr_mgmt/nrf_pwr_mgmt.h"
#include "components/libraries/util/app_error.h"

static void ble_dfu_buttonless_evt_handler(ble_dfu_buttonless_evt_type_t event)
{
	switch (event)
	{
		case BLE_DFU_EVT_BOOTLOADER_ENTER_PREPARE:
			NRF_LOG_INFO("Device is preparing to enter bootloader mode\r\n");
			break;
		case BLE_DFU_EVT_BOOTLOADER_ENTER:
			NRF_LOG_INFO("Device will enter bootloader mode\r\n");
			break;
		case BLE_DFU_EVT_BOOTLOADER_ENTER_FAILED:
			NRF_LOG_ERROR("Device failed to enter bootloader mode\r\n");
			break;
		default:
			NRF_LOG_INFO("Unknown event from ble_dfu.\r\n");
			break;
	}
}

static bool app_shutdown_handler(nrf_pwr_mgmt_evt_t event)
{
	switch (event)
	{
		case NRF_PWR_MGMT_EVT_PREPARE_DFU:
			NRF_LOG_INFO("Power management wants to reset to DFU mode\r\n");

			// turn off LEDs
			bsp_board_leds_off();

			break;

		default:
			// Implement any of the other events available from the power management module:
			//      -NRF_PWR_MGMT_EVT_PREPARE_SYSOFF
			//      -NRF_PWR_MGMT_EVT_PREPARE_WAKEUP
			//      -NRF_PWR_MGMT_EVT_PREPARE_RESET
			return true;
	}
	NRF_LOG_INFO("Power management allowed to reset to DFU mode\r\n");
	return true;
}

void sunrise_ble_dfu_init() {
	ble_dfu_buttonless_init_t dfus_init =
	{
		.evt_handler = ble_dfu_buttonless_evt_handler
	};

	ret_code_t err_code;

	err_code = ble_dfu_buttonless_async_svci_init();
	APP_ERROR_CHECK(err_code);

	err_code = ble_dfu_buttonless_init(&dfus_init);
	APP_ERROR_CHECK(err_code);

	NRF_PWR_MGMT_HANDLER_REGISTER(app_shutdown_handler, 0);
}
