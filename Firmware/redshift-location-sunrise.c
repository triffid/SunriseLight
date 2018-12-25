#include "redshift-location-sunrise.h"

#include <math.h>

#include "components/libraries/experimental_log/nrf_log.h"

#include "sunrise_ble.h"
#include "sunrise_ble_sunriseService.h"

// *** From redshift.h ***
//
// typedef struct {
// 	float lat;
// 	float lon;
// } location_t;

typedef struct {
	location_t loc;
} location_sunrise_state_t;

static location_sunrise_state_t _state;

static int32_t _lat, _lon;

/*
 * *** BLE stuff ****
 */

static void redshift_sunrise_ble_evt_handler(ble_evt_t const * p_ble_evt, void* p_context) {
	NRF_LOG_INFO("Redshift Sunrise Location: BLE hook!");

	switch (p_ble_evt->header.evt_id) {
		case BLE_GATTS_EVT_WRITE: {
			const ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
			switch (p_evt_write->handle) {
				case SUNRISE_BLE_UUID_LATITUDE: {
					_state.loc.lat = *((int32_t*) p_evt_write->data) * 1e-7f;
					NRF_LOG_INFO("New Latitude: " NRF_LOG_FLOAT_MARKER " (%i)", NRF_LOG_FLOAT(_state.loc.lat), *((int32_t*) p_evt_write->data));
				} break;
				case SUNRISE_BLE_UUID_LONGITUDE: {
					_state.loc.lon = *((int32_t*) p_evt_write->data) * 1e-7f;
					NRF_LOG_INFO("New Longitude: " NRF_LOG_FLOAT_MARKER " (%i)", NRF_LOG_FLOAT(_state.loc.lon), *((int32_t*) p_evt_write->data));
				} break;
			}
		} break;
	}
}

ret_code_t redshift_sunrise_ble_connect() {
	ADD_CHARACTERISTIC(lat, SUNRISE_BLE_UUID_LATITUDE , "Latitude" , RW_BOTH, BLE_UNIT_PLANE_ANGLE_DEGREE, BLE_GATT_CPF_FORMAT_SINT32, -7, 4, _lat, redshift_sunrise_ble_evt_handler);
	ADD_CHARACTERISTIC(lon, SUNRISE_BLE_UUID_LONGITUDE, "Longitude", RW_BOTH, BLE_UNIT_PLANE_ANGLE_DEGREE, BLE_GATT_CPF_FORMAT_SINT32, -7, 4, _lon, redshift_sunrise_ble_evt_handler);

	return NRF_SUCCESS;
}

/*
 * *** redshift location stuff ***
 */

static int
location_sunrise_init(location_sunrise_state_t **p_state)
{
	*p_state = &_state;

	_state.loc.lat = NAN;
	_state.loc.lon = NAN;

	return 0;
}

static int
location_sunrise_start(location_sunrise_state_t *state)
{
	return 0;
}

static void
location_sunrise_free(location_sunrise_state_t *state)
{
	_state.loc.lat = NAN;
	_state.loc.lon = NAN;
}

static void
location_sunrise_print_help(FILE *f)
{
}

static int
location_sunrise_set_option(location_sunrise_state_t *state, const char *key, const char *value)
{
	return 0;
}

static int
location_sunrise_get_fd(location_sunrise_state_t *state)
{
	return -1;
}

static int
location_sunrise_handle(location_sunrise_state_t *state, location_t *location, int *available)
{
	if (location)
		*location = state->loc;
	if (available)
		*available = (isnan(state->loc.lat) && isnan(state->loc.lon))?1:0;

	return 0;
}

void location_sunrise_update(location_state_t *state, float lat, float lon) {
	location_sunrise_state_t* s = (location_sunrise_state_t*) state;
	if (!isnan(lat)) {
		s->loc.lat = lat;
		_lat = lat * 1e7f;
		NRF_LOG_INFO("Update Latitude: "  NRF_LOG_FLOAT_MARKER " (%i)", NRF_LOG_FLOAT(_state.loc.lat), _lat);
	}
	if (!isnan(lon)) {
		s->loc.lon = lon;
		_lon = lon * 1e7f;
		NRF_LOG_INFO("Update Longitude: " NRF_LOG_FLOAT_MARKER " (%i)", NRF_LOG_FLOAT(_state.loc.lon), _lon);
	}
}

const location_provider_t sunrise_location_provider = {
	"sunrise",
	(location_provider_init_func *)location_sunrise_init,
	(location_provider_start_func *)location_sunrise_start,
	(location_provider_free_func *)location_sunrise_free,
	(location_provider_print_help_func *)location_sunrise_print_help,
	(location_provider_set_option_func *)location_sunrise_set_option,
	(location_provider_get_fd_func *)location_sunrise_get_fd,
	(location_provider_handle_func *)location_sunrise_handle
};
