#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
public:
    virtual void init() = 0;
    virtual bool read(float* data, int dataSize) = 0;
    virtual const char** dataLabels() = 0;
    virtual int dataCount() = 0;
    virtual ~Sensor() {}
};

#endif
