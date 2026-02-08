#include "system_event.h"
#include <zephyr/kernel.h>

K_MSGQ_DEFINE(system_msgq, sizeof(system_event), 10, 4);

