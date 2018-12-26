#ifndef SUNRISE_MODE_H
#define SUNRISE_MODE_H

#include <stddef.h>

#include "components/libraries/util/app_error.h"

typedef enum {
	SUNRISE_MODE_OFF,
	SUNRISE_MODE_SUNRISE,
	SUNRISE_MODE_MANUAL,
	SUNRISE_MODE_PARTY,
	SUNRISE_MODE_INVALID
} sunrise_mode_t;

struct _mode_change_notifier_hook_t;
typedef struct _mode_change_notifier_hook_t {
	void (*callback)(sunrise_mode_t);
	struct _mode_change_notifier_hook_t* next;
} mode_change_notifier_hook_t;


void sunrise_mode_init(void);

void           sunrise_mode_set(sunrise_mode_t);
sunrise_mode_t sunrise_mode_get(void          );

void sunrise_mode_register_callback(mode_change_notifier_hook_t*);
ret_code_t sunrise_mode_ble_connect(void);

#define SUNRISE_MODE_CALLBACK(func) \
	do { \
		static mode_change_notifier_hook_t func ## _mode_change_notifier_hook_t = { func, NULL }; \
		sunrise_mode_register_callback(& func ## _mode_change_notifier_hook_t); \
	} while (0)

#endif /* SUNRISE_MODE_H */
