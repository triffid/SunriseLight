#ifndef RGBTIMER_H
#define RGBTIMER_H

#include <stdint.h>

void rgbtimer_init(void);

void rgbtimer_setrgb(float r, float g, float b);
void rgbtimer_sethsv(float h, float s, float v);

void rgbtimer_start(void);
void rgbtimer_stop(void);

#endif /* RGBTIMER_H */
