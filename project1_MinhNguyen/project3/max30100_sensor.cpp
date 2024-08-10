#include "max30100_sensor.h"

MAX30100Sensor::MAX30100Sensor() : tsLastReport(0) {}

void MAX30100Sensor::begin() {
    pox.begin();
}

void MAX30100Sensor::update() {
    pox.update();
}

void MAX30100Sensor::displayData(Adafruit_SSD1306& display) {
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        int heartRate = pox.getHeartRate();
        int spo2 = pox.getSpO2();

        Serial.print("Heart rate:");
        Serial.print(heartRate);
        Serial.print("bpm / SpO2:");
        Serial.print(spo2);
        Serial.println("%");

        tsLastReport = millis();
    }
}

int MAX30100Sensor::getHeartRate() {
    return pox.getHeartRate();
}

int MAX30100Sensor::getSpO2() {
    return pox.getSpO2();
}

void MAX30100Sensor::setOnBeatDetectedCallback(void (*callback)()) {
    pox.setOnBeatDetectedCallback(callback);
}