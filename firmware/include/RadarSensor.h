#ifndef RADARSENSOR_H
#define RADARSENSOR_H

#include "Sensor.h"

class RadarSensor : public Sensor {
public:
    RadarSensor();
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
