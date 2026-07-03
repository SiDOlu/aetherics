#include "SensorFusion.h"

SensorFusion::SensorFusion(RadarSensor& r, ThermalSensor& t) 
    : radar(r), thermal(t), currentState(SystemState::ROOM_EMPTY), stateEntryTime(millis()) {}

SystemState SensorFusion::process() {
    bool motion = radar.hasMotion();
    bool heat = thermal.hasHeatSignature();
    unsigned long now = millis();

    switch (currentState) {
        case SystemState::ROOM_EMPTY:
            if (motion || heat) {
                currentState = SystemState::OCCUPIED_BED;
                stateEntryTime = now;
            }
            break;
        case SystemState::OCCUPIED_BED:
            if (!motion && !heat) {
                currentState = SystemState::ROOM_EMPTY;
                stateEntryTime = now;
            } else if (isDangling()) {
                currentState = SystemState::DANGLING_EDGE;
                stateEntryTime = now;
            }
            break;
        case SystemState::DANGLING_EDGE:
            if (isFall()) {
                currentState = SystemState::FALL_DETECTED;
                stateEntryTime = now;
            } else if (!motion && !heat) {
                currentState = SystemState::ROOM_EMPTY;
                stateEntryTime = now;
            }
            break;
        case SystemState::FALL_DETECTED:
            // Alert persistence, requires manual reset
            break;
    }
    
    return currentState;
}

bool SensorFusion::isDangling() {
    // Example temporal logic: Occupied for > 5s, then motion stops
    return (currentState == SystemState::OCCUPIED_BED && 
            (millis() - stateEntryTime > 5000) && 
            !radar.hasMotion());
}

bool SensorFusion::isFall() {
    // Example temporal logic: Motion detected abruptly
    return radar.hasMotion(); 
}

const char* SensorFusion::getStateName(SystemState state) {
    switch (state) {
        case SystemState::ROOM_EMPTY: return "ROOM_EMPTY";
        case SystemState::OCCUPIED_BED: return "OCCUPIED_BED";
        case SystemState::DANGLING_EDGE: return "DANGLING_EDGE";
        case SystemState::FALL_DETECTED: return "FALL_DETECTED";
        default: return "UNKNOWN";
    }
}
