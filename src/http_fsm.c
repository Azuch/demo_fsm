#include "http_fsm.h"
#include "system_event.h"
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(HTTP_FSM, LOG_LEVEL_INF);

void http_fsm_init(void) {
	LOG_INF("Hello, http fsm initialiazed");
	return;
}

void http_fsm_handle(system_event *evt) {
	LOG_INF("Hi this is my event handle");
	switch(evt->name) {
		case HTTP_REQUEST:
			LOG_INF("The event is: HTTP_REQUEST");
			break;
		case HTTP_RESPONSE:
			LOG_INF("The event is: HTTP_RESPONSE");
			break;
		default:
			LOG_WRN("I dont know this event");
			break;
	}
	return;
}

