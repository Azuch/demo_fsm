#include "wifi_fsm.h"
#include "system_event.h"

#include <stdint.h>

#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>

#define WIFI_BACKOFF_MS 10000

#define SSID Sehna
#define PSK "9876543210"

LOG_MODULE_REGISTER(WIFI_FSM, LOG_LEVEL_INF);

enum wifi_state {
	WIFI_STATE_CONNECTED,
	WIFI_STATE_CONNECTING,
	WIFI_STATE_INIT,
	WIFI_STATE_BACKOFF,
};

static struct {
	enum wifi_state state;
	uint8_t retry_count;
	uint64_t backoff_untill;
	bool connected;
} wifi_ctx;

static struct net_mgmt_event_callback wifi_mgmt_cb;
static void set_state(enum wifi_state next);
static char *state_to_str(enum wifi_state s);
system_event evt_data;
system_event *evt = &evt_data;

static void backoff_handler(struct k_timer *timer) {
	evt->name = WIFI_EVENT_INIT;
	k_msgq_put(&system_event, evt, K_NO_WAIT);
	return;
}
K_TIMER_DEFINE(backoff_timer, backoff_handler, NULL);

static void wifi_mgmt_handler (struct net_mgmt_event_callback *cb,
				uint64_t mgmt_event,
				struct net_if *iface) {
	switch (mgmt_event) {
		case NET_EVENT_WIFI_CONNECT_RESULT:
			evt->name = WIFI_EVENT_CONNECTED;
			k_msgq_put(&system_msgq, evt, K_NO_WAIT);
			//set_state(WIFI_STATE_CONNECTED);
			break;
		case NET_EVENT_WIFI_DISCONNECT_RESULT:
			evt->name = WIFI_EVENT_DISCONNECTED;
			k_msg_put(&system_msgq, evt, K_NO_WAIT);
			//set_state(WIFI_STATE_BACKOFF);
			wifi_ctx.backoff_untill = k_uptime_get() + WIFI_BACKOFF_MS;
			break;
		default:
			break;
	}
}

static char* state_to_str(enum wifi_state s) {
	switch(s) {
		case WIFI_STATE_INIT: 		return "INIT";
		case WIFI_STATE_CONNECTING:	return "CONNECTING";
		case WIFI_STATE_CONNECTED:	return "CONNECTED";
		case WIFI_STATE_BACKOFF: 	return "BACKOFF";
		default:			return "UNKNOWN";

	}
}

void set_state(enum wifi_state next) {
	if (wifi_ctx.state != next) {
		LOG_INF("State %s -> %s", state_to_str(wifi_ctx.state), state_to_str(next));
		wifi_ctx.state = next;
	}
}


bool wifi_is_connected(void) {
	return wifi_ctx.connected;
}

void wifi_fsm_init(void) {
	evt->type = WIFI_FSM;
	evt->name = WIFI_EVENT_INIT;
	wifi_ctx.connected = false;
	k_msgq_put(&system_event, evt, K_NO_WAIT);
	
	//Create network callback on wifi CONNECT and DISCONNECT event
	net_mgmt_init_event_callback(&wifi_mgmt_cb, wifi_mgmt_handler, NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
	//Assign new callback
	net_mgmt_add_event_callback(&wifi_mgmt_cb);
	//LOG it
	LOG_INF("WIFI FSM is initialized.");
}

void wifi_fsm_handle(system_event *evt) {

	switch(evt->name) {
		case WIFI_EVENT_INIT:
			set_state(WIFI_STATE_INIT);
			break;
		case WIFI_EVENT_CONNECTED:
			set_state(WIFI_STATE_CONNECTED);
			break;
		case WIFI_EVENT_DISCONNECTED:
			set_state(WIFI_STATE_BACKOFF);
			break;
		default:
			break;
	}
	switch(wifi_ctx.state) {
		case WIFI_STATE_INIT:
			//For send request
			struct net_if *iface = net_if_get_default();
			struct wifi_connect_req_params params;

			memset(&params, 0, sizeof(params));

			params.ssid = SSID;
			params.psk = PSK;
			params.ssid_length = sizeof(SSID);
			params.psk_length = sizeof(PSK);
			params.security = WIFI_SECURITY_TYPE_PSK;
			params.channel = WIFI_CHANNEL_ANY;
			params.band = WIFI_FREQ_BAND_2_4_GHZ;

			net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
			set_state(WIFI_STATE_CONNECTING);
			break;
		case WIFI_STATE_CONNECTING:
			//Keep waiting
			break;
		case WIFI_STATE_CONNECTED:
			wifi_ctx.connected = true;
			break;
		case WIFI_STATE_BACKOFF:
			wifi_ctx.connected = false;
			//Create a timer for a backoff time to send a wifi_event_init
			k_timer_start(&backoff_timer, WIFI_BACKOFF_MS, K_NO_WAIT);
			break;
		default:
			break;
	}
}




