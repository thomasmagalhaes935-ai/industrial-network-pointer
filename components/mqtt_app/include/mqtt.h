#ifndef MQTT_H
#define MQTT_H

#include <stdbool.h>

void mqtt_app_start(void);

bool mqtt_is_connected(void);

void mqtt_publish_count(int esteira_id, const char *tipo, int count);

#endif