#include "http_fsm.h"
#include "sensor_fsm.h"
#include "ota_fsm.h"
#include "wifi_fsm.h"

#include "system_event.h"

#include <zephyr/kernel.h>

int main(void) {

	system_event evt_data;
	system_event *evt = &evt_data;

	http_fsm_init();
	sensor_fsm_init();
	ota_fsm_init();
	wifi_fsm_init();

	//This is a mock event;
	evt->name = HTTP_REQUEST;
	evt->type = HTTP_FSM;
	k_msgq_put(&system_msgq, evt, K_NO_WAIT);
	while(1) {
		//Wait to get system event, if there is, base on its type, let that fsm handle
		k_msgq_get(&system_msgq, evt, K_FOREVER);
		switch(evt->type) {
			case HTTP_FSM:
				http_fsm_handle(evt);
				break;
			case SENSOR_FSM:
				sensor_fsm_handle(evt);
				break;
			case OTA_FSM:
				ota_fsm_handle(evt);
				break;
			case WIFI_FSM:
				wifi_fsm_handle(evt);
				break;
			default:
				break;
			}
	}
	return 0;
}
