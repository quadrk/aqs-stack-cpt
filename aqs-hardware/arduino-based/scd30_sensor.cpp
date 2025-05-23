#include "scd30_sensor.h"
#include <Wire.h>

SCD30Sensor::SCD30Sensor() : scd30() {}

void SCD30Sensor::init() {
    Wire.begin();
    scd30.initialize();
    delay(1000);
}

bool SCD30Sensor::read(float* data, int dataSize) {
    if (dataSize < _dataCount) return false;

    if (scd30.isAvailable()) {
        float result[_dataCount];
        scd30.getCarbonDioxideConcentration(result);
        for (int i = 0; i < _dataCount; i++) {
            data[i] = result[i];
        }
        return true;
    }
    return false;
}

const char** SCD30Sensor::dataLabels() {
    return _labels;
}

int SCD30Sensor::dataCount() {
    return _dataCount;
}
