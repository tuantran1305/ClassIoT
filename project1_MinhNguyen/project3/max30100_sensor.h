#ifndef MAX30100_SENSOR_H
#define MAX30100_SENSOR_H

#include "MAX30100_PulseOximeter.h"
#include <Adafruit_SSD1306.h>

class MAX30100Sensor {
public:
    MAX30100Sensor();
    void begin();
    void update();
    void displayData(Adafruit_SSD1306& display);
    int getHeartRate();
    int getSpO2();
    void setOnBeatDetectedCallback(void (*callback)());

private:
    PulseOximeter pox;
    uint32_t tsLastReport;
    static const uint32_t REPORTING_PERIOD_MS = 250;
};

#endif // MAX30100_SENSOR_H