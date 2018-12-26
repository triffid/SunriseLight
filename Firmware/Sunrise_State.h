#ifndef SUNRISE_STATE_H
#define SUNRISE_STATE_H

#include <stdbool.h>

typedef enum {
	OFF,
	ON
} State_t;

typedef void (*Sunrise_StateChangeHandler)(State_t newState) ;

void Sunrise_State_init(void);

void Sunrise_State_poll(void);

State_t Sunrise_State(void);

void Sunrise_State_RegisterStateChangeHandler(Sunrise_StateChangeHandler);

#endif /* SUNRISE_STATE_H */
