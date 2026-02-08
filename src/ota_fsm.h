#pragma once 

#include "system_event.h"

void ota_fsm_init(void);
void ota_fsm_handle(system_event *evt);
