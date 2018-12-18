#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>

void clock_init(void);

unsigned clock_getseconds(void);
void clock_setseconds(unsigned);
void clock_offset(int);

bool clock_secondsflag(void);

#endif /* CLOCK_H */
