#pragma once

#include "system_event.h"
#include <stdbool.h>

bool wifi_is_connected(void);
void wifi_fsm_init(void);
void wifi_fsm_handle(system_event *evt);
