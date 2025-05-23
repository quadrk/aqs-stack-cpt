#include "sensors.h"
#include "scd30_sensor.h"

WiFiClient wifi;

Sensor* sensors[] = {
  new SCD30Sensor(),
};

const int sensorCount = sizeof(sensors) / sizeof(sensors[0]);
