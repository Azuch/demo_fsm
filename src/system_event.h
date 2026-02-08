#pragma once

typedef enum {
	// HTTP events
	HTTP_REQUEST,
	HTTP_RESPONSE,
	// SENSOR events
	SENSOR_DATA_READY,
	// OTA events,
	OTA_REQUEST_UPGRADE,
	// WIFI events,
	WIFI_EVENT_INIT,
	WIFI_EVENT_CONNECTED,
	WIFI_EVENT_DISCONNECTED,
} event_name;

typedef enum {
	HTTP_FSM,
	SENSOR_FSM,
	OTA_FSM,
	WIFI_FSM,
} event_type;

typedef struct {
	event_name name;
	event_type type;
} system_event;

extern struct k_msgq system_msgq;
