#pragma once

typedef enum {
	// HTTP events
	HTTP_REQUEST,
	HTTP_RESPONSE,
	// SENSOR events
	SENSOR_DATA_READY,
	// OTA events,
	OTA_REQUEST_UPGRADE,
} event_name;

typedef enum {
	HTTP_FSM,
	SENSOR_FSM,
	OTA_FSM,
} event_type;

typedef struct {
	event_name name;
	event_type type;
} system_event;
