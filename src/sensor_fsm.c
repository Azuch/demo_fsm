#include "sensor_fsm.h"
#include "system_event.h"
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(SENSOR_FSM, LOG_LEVEL_INF);

void sensor_fsm_init(void) {
	LOG_INF("Hello, sensor fsm initialiazed");
	return;
}

void sensor_fsm_handle(system_event *evt) {
	LOG_INF("Hi this is my event handle");
	switch(evt->name) {
		case SENSOR_DATA_READY:
			LOG_INF("The event is: SENSOR_DATA_READY");
			break;
		default:
			LOG_WRN("I dont know this event");
			break;
	}
	return;
}

