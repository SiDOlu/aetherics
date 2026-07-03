#include "RadarSensor.h"
#include <HardwareSerial.h>

// Define pins for IWR6843 UART communication
#define RADAR_RX_PIN 16
#define RADAR_TX_PIN 17
HardwareSerial radarSerial(1); // Using UART1 on ESP32

RadarSensor::RadarSensor() : initialized(false) {}

bool RadarSensor::begin() {
    Serial.println("RadarSensor: Initializing IWR6843 over UART1...");
    radarSerial.begin(115200, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN);
    
    // TODO: Send IWR6843 config command if needed
    initialized = true;
    return initialized;
}

// Buffer for incoming radar data
#define BUFFER_SIZE 512
uint8_t buffer[BUFFER_SIZE];
size_t bufferIndex = 0;

bool RadarSensor::update() {
    if (!initialized) return false;
    
    while (radarSerial.available() > 0) {
        if (bufferIndex < BUFFER_SIZE) {
            buffer[bufferIndex++] = radarSerial.read();
            
            // Simple parsing: Check for end-of-frame (example: '\n')
            // In a real IWR6843 implementation, this would involve checking
            // for the magic word and packet length.
            if (buffer[bufferIndex - 1] == '\n') {
                Serial.println("RadarSensor: Full frame received.");
                // TODO: Process buffer content
                bufferIndex = 0; 
            }
        } else {
            // Buffer overflow, reset
            bufferIndex = 0;
        }
    }
    return true;
}

bool RadarSensor::isDataAvailable() {
    return initialized && (radarSerial.available() > 0);
}

const char* RadarSensor::getName() {
    return "TI_IWR6843";
}

bool RadarSensor::hasMotion() {
    // TODO: Parse radar frame for motion detection
    return true; 
}

bool RadarSensor::hasHeatSignature() {
    return false; // Radar does not detect heat directly
}
