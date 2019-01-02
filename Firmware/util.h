#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define M_PIF    3.14159265358979f
#define M_TWOPIF 6.28318530717959f

#define __debugbreak()  do { __asm volatile ("bkpt #0"); } while (0)

extern uint32_t xorshift_state;
uint32_t xorshift32();

#endif /* UTIL_H */
