#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor {
public:
    virtual ~Sensor() {}
    virtual bool begin() = 0;
    virtual bool update() = 0;
    virtual bool isDataAvailable() = 0;
    virtual const char* getName() = 0;
    
    // Data access
    virtual bool hasMotion() = 0;
    virtual bool hasHeatSignature() = 0;
};

#endif
