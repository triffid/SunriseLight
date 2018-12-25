#ifndef REDSHIFT_LOCATION_SUNRISE_H
#define REDSHIFT_LOCATION_SUNRISE_H

#include "redshift/src/redshift.h"

#include "components/libraries/util/app_error.h"

extern const location_provider_t sunrise_location_provider;

// redshift's manual location provider likes text parsing and doesn't have an update mechanism
void location_sunrise_update(location_state_t *state, float lat, float lon);

// initialize our characteristic once the service is ready
ret_code_t redshift_sunrise_ble_connect(void);

#endif /* REDSHIFT_LOCATION_SUNRISE_H */
