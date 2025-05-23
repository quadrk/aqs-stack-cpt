#ifndef SENSORS_H
#define SENSORS_H

#include "Sensor.h"
#include <WiFiS3.h>

extern WiFiClient wifi;
extern Sensor* sensors[];
extern const int sensorCount;

#endif
