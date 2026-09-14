#ifndef MQTT_APP_H
#define MQTT_APP_H

#include "mqtt_client.h"

void mqtt_app_start(void);
void mqtt_publish_count(int esteira_id, int count);
bool mqtt_is_connected(void);

#endif