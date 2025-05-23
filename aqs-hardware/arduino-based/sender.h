#ifndef SENDER_H
#define SENDER_H

#include <WiFiClient.h>
#include "Sensor.h"

void sendAllMeasurements(Sensor** sensors, int sensorCount, WiFiClient& wifi);
void printSensorData(Sensor** sensors, int sensorCount);

#endif