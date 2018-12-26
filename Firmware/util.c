#include "util.h"

// from https://en.wikipedia.org/wiki/Xorshift
/* The state word must be initialized to non-zero */
uint32_t xorshift_state = 0xDEADBEEF;
uint32_t xorshift32()
{
	uint32_t x = xorshift_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	xorshift_state = x;
	return x;
}

