#include "ota_fsm.h"
#include "system_event.h"
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(OTA_FSM, LOG_LEVEL_INF);

void ota_fsm_init(void) {
	LOG_INF("Hello, ota fsm initialiazed");
	return;
}

void ota_fsm_handle(system_event *evt) {
	LOG_INF("Hi this is my event handle");
	switch(evt->name) {
		case OTA_REQUEST_UPGRADE:
			LOG_INF("The event is: OTA_REQUEST_UPGRADE");
			break;
		default:
			LOG_WRN("I dont know this event");
			break;
	}
	return;
}

