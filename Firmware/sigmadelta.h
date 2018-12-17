#ifndef SIGMADELTA_H
#define SIGMADELTA_H

#include <stdint.h>

typedef struct {
	uint32_t pin;
	int value;
	int max;
	int accumulator;
} SigmaDelta_t;

void SigmaDelta_init(SigmaDelta_t*, uint32_t pin, int max, int value);

void SigmaDelta_set(SigmaDelta_t*, int value);
void SigmaDelta_run(SigmaDelta_t*);

#endif /* SIGMADELTA_H */
