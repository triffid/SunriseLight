/**
 * Copyright (c) 2014 - 2018, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/** @file
 *
 * @defgroup ble_sdk_app_template_main main.c
 * @{
 * @ingroup ble_sdk_app_template
 * @brief Template project main file.
 *
 * This file contains a template for creating a new application. It has the code necessary to wakeup
 * from button, advertise, get a connection restart advertising on disconnect and if no new
 * connection created go back to system-off mode.
 * It can easily be used as a starting point for creating a new application, the comments identified
 * with 'YOUR_JOB' indicates where and how you can customize.
 */

#include <stdbool.h>
#include <stdint.h>

#include <string.h>

#include "components/libraries/util/app_error.h"
#include "components/libraries/timer/app_timer.h"
#include "components/libraries/gpiote/app_gpiote.h"

#include "components/libraries/experimental_log/nrf_log.h"
#include "components/libraries/experimental_log/nrf_log_ctrl.h"
#include "components/libraries/experimental_log/nrf_log_default_backends.h"
#include "components/libraries/pwr_mgmt/nrf_pwr_mgmt.h"

#include "modules/nrfx/drivers/include/nrfx_power.h"

#include "sigmadelta.h"
#include "rgbtimer.h"
#include "redshift_hook.h"
#include "clock.h"
#include "sunrise_ble.h"
#include "sunrise_ble_dfu.h"
#include "Sunrise_State.h"

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void)
{
	// Initialize timer module.
	ret_code_t err_code = app_timer_init();
	APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

static void power_init(void)
{
	nrfx_power_config_t pwr = { 1 };
	nrfx_power_init(&pwr);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }

    if (clock_secondsflag()) {
		redshift_update();
		Sunrise_State_poll();
	}
}

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) {
	NRF_LOG_ERROR("id 0x%08x pc 0x%08x info 0x%08x\n", id, pc, info);
	for (;;);
}

/**@brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    log_init();

	power_init();

	Sunrise_State_init();

	APP_GPIOTE_INIT(8);
    timers_init();
	clock_init();
    power_management_init();

	sunrise_ble_init();
	sunrise_ble_dfu_init();

	rgbtimer_init();
	rgbtimer_start();

	redshift_init();

	// Start execution.
	NRF_LOG_INFO("Sunlight started.");

	// Enter main loop.
    for (;;)
		idle_state_handle();
}


/**
 * @}
 */
