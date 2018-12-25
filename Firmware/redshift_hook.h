#ifndef REDSHIFT_H
#define REDSHIFT_H

#include "components/libraries/util/app_error.h"

void redshift_init(void);
void redshift_update(void);

ret_code_t redshift_ble_init(void);

#endif /* REDSHIFT_H */
