#ifndef E18_SENSOR_H
#define E18_SENSOR_H

#include <stdbool.h>

void e18_sensor_init(void);

bool e18_sensor_bom_detected(void);
bool e18_sensor_ruim_detected(void);

#endif