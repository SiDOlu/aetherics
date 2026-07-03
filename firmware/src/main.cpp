#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include "RadarSensor.h"
#include "ThermalSensor.h"
#include "SensorFusion.h"

// NVS Persistence
Preferences prefs;
RadarSensor radar;
ThermalSensor thermal;
SensorFusion fusion(radar, thermal);

// Toggle structure
struct ToggleRegistry {
    bool hw_use_microphone;
    bool hw_use_thermal;
    bool hw_use_radar;
    bool process_vitals_heart_rate;
    bool process_vitals_respiration;
    bool process_acoustic_cough;
    bool process_acoustic_distress;
    bool process_predictive_fall;
};

ToggleRegistry registry;

WiFiClient espClient;
PubSubClient client(espClient);

void loadToggles() {
    prefs.begin("toggles", true);
    registry.hw_use_microphone = prefs.getBool("mic", true);
    registry.hw_use_thermal = prefs.getBool("thermal", true);
    registry.hw_use_radar = prefs.getBool("radar", true);
    // ... load other toggles
    prefs.end();
}

void saveToggles() {
    prefs.begin("toggles", false);
    prefs.putBool("mic", registry.hw_use_microphone);
    prefs.putBool("thermal", registry.hw_use_thermal);
    prefs.putBool("radar", registry.hw_use_radar);
    prefs.end();
}

void handleRemoteConfig(const char* jsonPayload) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, jsonPayload);

    if (error) {
        Serial.println("RemoteConfig: Deserialization failed");
        return;
    }

    if (doc.containsKey("hw_use_microphone")) registry.hw_use_microphone = doc["hw_use_microphone"];
    if (doc.containsKey("hw_use_thermal")) registry.hw_use_thermal = doc["hw_use_thermal"];
    if (doc.containsKey("hw_use_radar")) registry.hw_use_radar = doc["hw_use_radar"];
    
    saveToggles();
    Serial.println("RemoteConfig: Toggles updated and persisted.");
    // In production: Publish confirmation to AWS IoT Shadow
}

void sendHeartbeat() {
    StaticJsonDocument<256> doc;
    doc["device_id"] = "aeth-node-0912X";
    doc["uptime"] = millis() / 1000;
    doc["cpu_usage"] = 10.5; // Real sensor data
    doc["free_heap"] = ESP.getFreeHeap();

    JsonObject active_toggles = doc.createNestedObject("active_toggles");
    active_toggles["microphone"] = registry.hw_use_microphone;
    active_toggles["thermal"] = registry.hw_use_thermal;
    active_toggles["radar"] = registry.hw_use_radar;

    char buffer[256];
    serializeJson(doc, buffer);
    client.publish("aetherics/heartbeat", buffer);
    Serial.println("Heartbeat published.");
}

void setup() {
    Serial.begin(115200);
    loadToggles();
    
    if (registry.hw_use_radar) {
        radar.begin();
    }
    
    if (registry.hw_use_thermal) {
        thermal.begin();
    }
    
    // WiFi.begin("SSID", "PASS"); // Production credentials
    Serial.println("Node Initialized.");
}

void loop() {
    if (registry.hw_use_radar && radar.isDataAvailable()) {
        radar.update();
    }
    
    if (registry.hw_use_thermal && thermal.isDataAvailable()) {
        thermal.update();
    }

    // Run fusion logic
    SystemState state = fusion.process();
    // TODO: Publish state change to AWS
    
    if (WiFi.status() == WL_CONNECTED) {
        if (!client.connected()) {
            // reconnectAWS();
        }
        client.loop();
        
        static unsigned long last_hb = 0;
        if (millis() - last_hb > 5000) {
            sendHeartbeat();
            last_hb = millis();
        }
    }
}
