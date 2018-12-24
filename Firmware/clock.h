#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>

#include "components/libraries/util/app_error.h"

void clock_init(void);

unsigned clock_getseconds(void);
void clock_setseconds(unsigned);
void clock_offset(int);

bool clock_secondsflag(void);

ret_code_t clock_ble_connect(void);

#endif /* CLOCK_H */
