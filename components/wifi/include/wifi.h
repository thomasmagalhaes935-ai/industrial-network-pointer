#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <stdbool.h>

void wifi_init(void);

void wifi_connect(void);

bool wifi_is_connected(void);

#endif