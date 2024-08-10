#include "max30100_sensor.h"

void max30100_sensor_init(max30100_sensor_t *sensor)
{
    sensor->ts_last_report = 0;
}

void max30100_sensor_begin(max30100_sensor_t *sensor)
{
    sensor->pox.begin();
}

void max30100_sensor_update(max30100_sensor_t *sensor)
{
    sensor->pox.update();
}

void max30100_sensor_display_data(max30100_sensor_t *sensor, Adafruit_SSD1306 *display)
{
    if (millis() - sensor->ts_last_report > REPORTING_PERIOD_MS)
    {
        int heart_rate = sensor->pox.getHeartRate();
        int spo2 = sensor->pox.getSpO2();

        Serial.print("Heart rate:");
        Serial.print(heart_rate);
        Serial.print("bpm / SpO2:");
        Serial.print(spo2);
        Serial.println("%");

        sensor->ts_last_report = millis();
    }
}

int max30100_sensor_get_heart_rate(max30100_sensor_t *sensor)
{
    return sensor->pox.getHeartRate();
}

int max30100_sensor_get_spo2(max30100_sensor_t *sensor)
{
    return sensor->pox.getSpO2();
}

void max30100_sensor_set_on_beat_detected_callback(max30100_sensor_t *sensor, void (*callback)())
{
    sensor->pox.setOnBeatDetectedCallback(callback);
}