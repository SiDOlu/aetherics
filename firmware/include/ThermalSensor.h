#ifndef THERMALSENSOR_H
#define THERMALSENSOR_H

#include "Sensor.h"

class ThermalSensor : public Sensor {
public:
    ThermalSensor();
    bool begin() override;
    bool update() override;
    bool isDataAvailable() override;
    const char* getName() override;
    bool hasMotion() override;
    bool hasHeatSignature() override;

private:
    bool initialized;
};

#endif
