#ifndef SENSORFUSION_H
#define SENSORFUSION_H

#include "RadarSensor.h"
#include "ThermalSensor.h"

enum class SystemState {
    ROOM_EMPTY,
    OCCUPIED_BED,
    DANGLING_EDGE,
    FALL_DETECTED
};

class SensorFusion {
public:
    SensorFusion(RadarSensor& radar, ThermalSensor& thermal);
    SystemState process();
    const char* getStateName(SystemState state);

private:
    RadarSensor& radar;
    ThermalSensor& thermal;
    SystemState currentState;
    
    // Temporal Analysis
    unsigned long stateEntryTime;
    bool isDangling();
    bool isFall();
};

#endif
