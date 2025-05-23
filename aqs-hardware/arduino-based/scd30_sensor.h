#ifndef SCD30_SENSOR_H
#define SCD30_SENSOR_H

#include "Sensor.h"
#include <SCD30.h>

class SCD30Sensor : public Sensor {
public:
    SCD30Sensor();
    void init() override;
    bool read(float* data, int dataSize) override;
    const char** dataLabels() override;
    int dataCount() override;

private:
    SCD30 scd30;
    static constexpr int _dataCount = 3;
    const char* _labels[_dataCount] = {"CO2", "T", "H"};
};

#endif
