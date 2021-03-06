PROJECT_NAME     := SunriseLight
TARGETS          := nrf52832_xxaa
OUTPUT_DIRECTORY := build

SDK_ROOT := $(HOME)/Projects/NRF/nRF5_SDK
PROJ_DIR := $(shell realpath .)
BOOTLOADER_HEX := $(PROJ_DIR)/../Bootloader/build/nrf52832_xxaa_s132.hex

GNU_PREFIX := arm-unknown-eabi

CONSOLE  := /dev/ttyUSB0

$(OUTPUT_DIRECTORY)/$(TARGETS).out: \
	LINKER_SCRIPT := SunriseLight.ld

# Source files common to all targets
SRC_FILES += \
	$(wildcard $(PROJ_DIR)/*.c) \
	$(PROJ_DIR)/redshift/src/colorramp.c \
	$(PROJ_DIR)/redshift/src/solar.c \
	$(PROJ_DIR)/redshift/src/systemtime.c \
	$(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52.S \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_rtt.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_uart.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_default_backends.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c \
	$(SDK_ROOT)/components/libraries/button/app_button.c \
	$(SDK_ROOT)/components/libraries/util/app_error.c \
	$(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c \
	$(SDK_ROOT)/components/libraries/util/app_error_weak.c \
	$(SDK_ROOT)/components/libraries/gpiote/app_gpiote.c \
	$(SDK_ROOT)/components/libraries/scheduler/app_scheduler.c \
	$(SDK_ROOT)/components/libraries/timer/app_timer.c \
	$(SDK_ROOT)/components/libraries/util/app_util_platform.c \
	$(SDK_ROOT)/components/libraries/crc16/crc16.c \
	$(SDK_ROOT)/components/libraries/fds/fds.c \
	$(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c \
	$(SDK_ROOT)/components/libraries/util/nrf_assert.c \
	$(SDK_ROOT)/components/libraries/atomic_fifo/nrf_atfifo.c \
	$(SDK_ROOT)/components/libraries/atomic_flags/nrf_atflags.c \
	$(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
	$(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
	$(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
	$(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c \
	$(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage.c \
	$(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage_sd.c \
	$(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
	$(SDK_ROOT)/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c \
	$(SDK_ROOT)/components/libraries/experimental_section_vars/nrf_section_iter.c \
	$(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
	$(SDK_ROOT)/components/libraries/sensorsim/sensorsim.c \
	$(SDK_ROOT)/modules/nrfx/mdk/system_nrf52.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_clock.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_uart.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_gpiote.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_power.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uart.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uarte.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
	$(SDK_ROOT)/components/ble/common/ble_advdata.c \
	$(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_params.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_state.c \
	$(SDK_ROOT)/components/ble/common/ble_srv_common.c \
	$(SDK_ROOT)/components/ble/peer_manager/gatt_cache_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/gatts_cache_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/id_manager.c \
	$(SDK_ROOT)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \
	$(SDK_ROOT)/components/ble/nrf_ble_qwr/nrf_ble_qwr.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_data_storage.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_database.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_id.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/pm_buffer.c \
	$(SDK_ROOT)/components/ble/peer_manager/security_dispatcher.c \
	$(SDK_ROOT)/components/ble/peer_manager/security_manager.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh_ble.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh_soc.c \
	$(SDK_ROOT)/components/boards/boards.c \
	$(SDK_ROOT)/components/libraries/bsp/bsp.c \
	$(SDK_ROOT)/components/libraries/bsp/bsp_btn_ble.c \
	$(SDK_ROOT)/components/ble/ble_services/ble_dfu/ble_dfu.c \
	$(SDK_ROOT)/components/ble/ble_services/ble_dfu/ble_dfu_bonded.c \
	$(SDK_ROOT)/components/ble/ble_services/ble_dfu/ble_dfu_unbonded.c \
	$(SDK_ROOT)/components/libraries/bootloader/dfu/nrf_dfu_svci.c \
	$(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c

# Include folders common to all targets
INC_FOLDERS += \
	$(PROJ_DIR) \
	$(PROJ_DIR)/redshift/src \
	$(SDK_ROOT) \
	$(SDK_ROOT)/components/nfc/ndef/generic/message \
	$(SDK_ROOT)/components/nfc/t2t_lib \
	$(SDK_ROOT)/components/nfc/t4t_parser/hl_detection_procedure \
	$(SDK_ROOT)/components/ble/ble_services/ble_ancs_c \
	$(SDK_ROOT)/components/ble/ble_services/ble_ias_c \
	$(SDK_ROOT)/components/libraries/pwm \
	$(SDK_ROOT)/components/softdevice/s132/headers/nrf52 \
	$(SDK_ROOT)/components/libraries/usbd/class/cdc/acm \
	$(SDK_ROOT)/components/libraries/usbd/class/hid/generic \
	$(SDK_ROOT)/components/libraries/usbd/class/msc \
	$(SDK_ROOT)/components/libraries/usbd/class/hid \
	$(SDK_ROOT)/modules/nrfx/hal \
	$(SDK_ROOT)/components/libraries/log \
	$(SDK_ROOT)/components/ble/ble_services/ble_gls \
	$(SDK_ROOT)/components/libraries/fstorage \
	$(SDK_ROOT)/components/nfc/ndef/text \
	$(SDK_ROOT)/components/libraries/mutex \
	$(SDK_ROOT)/components/libraries/gpiote \
	$(SDK_ROOT)/components/libraries/log/src \
	$(SDK_ROOT)/components/libraries/bootloader/ble_dfu \
	$(SDK_ROOT)/components/nfc/ndef/connection_handover/common \
	$(SDK_ROOT)/components/boards \
	$(SDK_ROOT)/components/nfc/ndef/generic/record \
	$(SDK_ROOT)/components/libraries/memobj \
	$(SDK_ROOT)/components/nfc/t4t_parser/cc_file \
	$(SDK_ROOT)/components/ble/ble_advertising \
	$(SDK_ROOT)/components/ble/ble_services/ble_bas_c \
	$(SDK_ROOT)/modules/nrfx/drivers/include \
	$(SDK_ROOT)/components/libraries/experimental_task_manager \
	$(SDK_ROOT)/components/ble/ble_services/ble_hrs_c \
	$(SDK_ROOT)/components/nfc/ndef/connection_handover/le_oob_rec \
	$(SDK_ROOT)/components/libraries/queue \
	$(SDK_ROOT)/components/libraries/pwr_mgmt \
	$(SDK_ROOT)/components/ble/ble_dtm \
	$(SDK_ROOT)/components/toolchain/cmsis/include \
	$(SDK_ROOT)/components/ble/ble_services/ble_rscs_c \
	$(SDK_ROOT)/components/ble/common \
	$(SDK_ROOT)/components/ble/ble_services/ble_lls \
	$(SDK_ROOT)/components/libraries/bsp \
	$(SDK_ROOT)/components/nfc/ndef/connection_handover/ac_rec \
	$(SDK_ROOT)/components/ble/ble_services/ble_bas \
	$(SDK_ROOT)/components/libraries/experimental_section_vars \
	$(SDK_ROOT)/components/softdevice/s132/headers \
	$(SDK_ROOT)/components/ble/ble_services/ble_ans_c \
	$(SDK_ROOT)/components/libraries/slip \
	$(SDK_ROOT)/components/libraries/delay \
	$(SDK_ROOT)/components/libraries/mem_manager \
	$(SDK_ROOT)/components/libraries/csense_drv \
	$(SDK_ROOT)/components/ble/ble_services/ble_nus_c \
	$(SDK_ROOT)/components/softdevice/common \
	$(SDK_ROOT)/components/ble/ble_services/ble_ias \
	$(SDK_ROOT)/components/libraries/usbd/class/hid/mouse \
	$(SDK_ROOT)/components/libraries/low_power_pwm \
	$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser/ble_oob_advdata_parser \
	$(SDK_ROOT)/components/ble/ble_services/ble_dfu \
	$(SDK_ROOT)/external/fprintf \
	$(SDK_ROOT)/components/libraries/atomic \
	$(SDK_ROOT)/components \
	$(SDK_ROOT)/components/libraries/scheduler \
	$(SDK_ROOT)/components/libraries/cli \
	$(SDK_ROOT)/components/ble/ble_services/ble_lbs \
	$(SDK_ROOT)/components/ble/ble_services/ble_hts \
	$(SDK_ROOT)/components/libraries/crc16 \
	$(SDK_ROOT)/components/nfc/t4t_parser/apdu \
	$(SDK_ROOT)/components/libraries/util \
	$(SDK_ROOT)/components/libraries/usbd/class/cdc \
	$(SDK_ROOT)/components/libraries/csense \
	$(SDK_ROOT)/components/libraries/balloc \
	$(SDK_ROOT)/components/libraries/ecc \
	$(SDK_ROOT)/components/libraries/hardfault \
	$(SDK_ROOT)/components/ble/ble_services/ble_cscs \
	$(SDK_ROOT)/components/libraries/hci \
	$(SDK_ROOT)/components/libraries/timer \
	$(SDK_ROOT)/integration/nrfx \
	$(SDK_ROOT)/components/nfc/t4t_parser/tlv \
	$(SDK_ROOT)/components/libraries/sortlist \
	$(SDK_ROOT)/components/libraries/spi_mngr \
	$(SDK_ROOT)/components/libraries/led_softblink \
	$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser \
	$(SDK_ROOT)/components/libraries/sdcard \
	$(SDK_ROOT)/components/nfc/ndef/parser/record \
	$(SDK_ROOT)/modules/nrfx/mdk \
	$(SDK_ROOT)/components/ble/ble_services/ble_cts_c \
	$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser/le_oob_rec_parser \
	$(SDK_ROOT)/components/ble/ble_services/ble_nus \
	$(SDK_ROOT)/components/libraries/twi_mngr \
	$(SDK_ROOT)/components/ble/ble_services/ble_hids \
	$(SDK_ROOT)/components/libraries/strerror \
	$(SDK_ROOT)/components/libraries/crc32 \
	$(SDK_ROOT)/components/nfc/ndef/connection_handover/ble_oob_advdata \
	$(SDK_ROOT)/components/nfc/t2t_parser \
	$(SDK_ROOT)/components/nfc/ndef/connection_handover/ble_pair_msg \
	$(SDK_ROOT)/components/libraries/usbd/class/audio \
	$(SDK_ROOT)/components/libraries/sensorsim \
	$(SDK_ROOT)/components/nfc/t4t_lib \
	$(SDK_ROOT)/components/ble/peer_manager \
	$(SDK_ROOT)/components/ble/ble_services/ble_tps \
	$(SDK_ROOT)/components/nfc/ndef/parser/message \
	$(SDK_ROOT)/components/ble/ble_services/ble_dis \
	$(SDK_ROOT)/components/nfc/ndef/uri \
	$(SDK_ROOT)/components/ble/nrf_ble_gatt \
	$(SDK_ROOT)/components/ble/nrf_ble_qwr \
	$(SDK_ROOT)/components/libraries/gfx \
	$(SDK_ROOT)/components/libraries/button \
	$(SDK_ROOT)/modules/nrfx \
	$(SDK_ROOT)/components/libraries/twi_sensor \
	$(SDK_ROOT)/integration/nrfx/legacy \
	$(SDK_ROOT)/components/libraries/usbd/class/hid/kbd \
	$(SDK_ROOT)/components/nfc/ndef/connection_handover/ep_oob_rec \
	$(SDK_ROOT)/external/segger_rtt \
	$(SDK_ROOT)/components/libraries/atomic_fifo \
	$(SDK_ROOT)/components/libraries/ringbuf \
	$(SDK_ROOT)/components/ble/ble_services/ble_lbs_c \
	$(SDK_ROOT)/components/nfc/ndef/connection_handover/ble_pair_lib \
	$(SDK_ROOT)/components/libraries/crypto \
	$(SDK_ROOT)/components/ble/ble_racp \
	$(SDK_ROOT)/components/libraries/fds \
	$(SDK_ROOT)/components/nfc/ndef/launchapp \
	$(SDK_ROOT)/components/libraries/atomic_flags \
	$(SDK_ROOT)/components/ble/ble_services/ble_rscs \
	$(SDK_ROOT)/components/nfc/ndef/connection_handover/hs_rec \
	$(SDK_ROOT)/components/libraries/usbd \
	$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser/ac_rec_parser \
	$(SDK_ROOT)/components/ble/ble_services/ble_hrs \
	$(SDK_ROOT)/components/libraries/svc \
	$(SDK_ROOT)/components/libraries/bootloader \
	$(SDK_ROOT)/components/libraries/bootloader/dfu \


# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -Os -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# flags common to all targets
FLAGS += -march=armv7e-m -mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-stack-protector

# definitions
DEFS += -DBOARD_CUSTOM
DEFS += -DCONFIG_GPIO_AS_PINRESET
DEFS += -DFLOAT_ABI_HARD
DEFS += -DNRF52
DEFS += -DNRF52832_XXAA
DEFS += -DNRF52_PAN_74
DEFS += -DNRF_DFU_SVCI_ENABLED
DEFS += -DNRF_DFU_TRANSPORT_BLE=1
DEFS += -DNRF_SD_BLE_API_VERSION=6
DEFS += -DS132
DEFS += -DSOFTDEVICE_PRESENT
DEFS += -DSWI_DISABLE0
# DEFS += -DDEBUG

# C flags
CFLAGS += $(FLAGS) $(OPT) $(DEFS)
CFLAGS += -std=gnu11
CFLAGS += -Wall
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums
# use compile date as reasonable start date during reset
CFLAGS += -DSTART_DATE=$(shell date +%s)
# silence a warning from redshift library
CFLAGS += -D"nanosleep(a, b)=do { if (a != b) do {} while (0);} while (0)"
# only for debugging
# CFLAGS += -save-temps

# C++ flags common to all targets
CXXFLAGS += $(CFLAGS)
CXXFLAGS += -std=gnu++11

# Assembler flags common to all targets
ASMFLAGS += $(FLAGS) $(DEFS)
ASMFLAGS += -g3

# Linker flags
LDFLAGS += $(FLAGS) $(OPT)
LDFLAGS += -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

SDK_CONFIG_FILE := sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar

nrf52832_xxaa: CFLAGS += -D__HEAP_SIZE=8192
nrf52832_xxaa: CFLAGS += -D__STACK_SIZE=8192
nrf52832_xxaa: ASMFLAGS += -D__HEAP_SIZE=8192
nrf52832_xxaa: ASMFLAGS += -D__STACK_SIZE=8192

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help flash flash_softdevice erase debug sdk_config console reset

# Default target - first one defined
default: nrf52832_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		nrf52832_xxaa
	@echo		flash_softdevice
	@echo		sdk_config - starting external tool for editing sdk_config.h
	@echo		flash      - flashing binary
	@echo		erase
	@echo		debug      - start gdb
	@echo		dfu.zip    - generate a zip suitable for DFU
	@echo		reset      - reset target board
	@echo		console    - start a debug console

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash flash_softdevice erase

$(OUTPUT_DIRECTORY)/bl-settings.hex:$(OUTPUT_DIRECTORY)/nrf52832_xxaa.hex
	nrfutil settings generate --family NRF52 --application $^ --application-version 1 --bootloader-version 1 --bl-settings-version 1 $@

$(OUTPUT_DIRECTORY)/merged.hex: $(OUTPUT_DIRECTORY)/nrf52832_xxaa.hex $(OUTPUT_DIRECTORY)/bl-settings.hex $(BOOTLOADER_HEX)
	@mergehex -o $@ -m $^

# Flash the program
flash: $(OUTPUT_DIRECTORY)/merged.hex
	@echo Flashing: $<
	nrfjprog -f nrf52 --program $< --sectorerase
	nrfjprog -f nrf52 --reset

# Flash softdevice
flash_softdevice:
	@echo Flashing: s132_nrf52_6.0.0_softdevice.hex
	nrfjprog -f nrf52 --program $(SDK_ROOT)/components/softdevice/s132/hex/s132_nrf52_6.0.0_softdevice.hex --sectorerase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall

debug: | $(OUTPUT_DIRECTORY)/nrf52832_xxaa.out
	arm-none-eabi-gdb -ex "file $(OUTPUT_DIRECTORY)/nrf52832_xxaa.out" -ex "target remote localhost:2331"

console:
	if arm-none-eabi-objdump -t build/nrf52832_xxaa.out | grep -q rtt_log_backend; \
	then \
		JLinkRTTClient; \
	elif arm-none-eabi-objdump -t build/nrf52832_xxaa.out | grep -q uart_log_backend; \
	then \
		stty raw ignbrk -echo 1000000 < $(CONSOLE); \
		cat $(CONSOLE); \
	else \
		echo "Don't know what type of log this binary uses:"; \
		arm-none-eabi-objdump -t build/nrf52832_xxaa.out | egrep '_log_backend\b'; \
	fi

reset:
	nrfjprog -r

dfu.zip: $(OUTPUT_DIRECTORY)/nrf52832_xxaa.hex ../private.key
	@echo $$(( $$(cat version 2>/dev/null) + 1 )) > version
	nrfutil pkg generate --application "$<" --key-file ../private.key --sd-req 0xA8 --hw-version 52 --application-version $(shell cat version) "$@"

sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
