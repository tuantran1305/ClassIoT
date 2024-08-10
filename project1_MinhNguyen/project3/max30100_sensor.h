#ifndef MAX30100_SENSOR_H
#define MAX30100_SENSOR_H

#include "MAX30100_PulseOximeter.h"
#include <Adafruit_SSD1306.h>

#define REPORTING_PERIOD_MS (200)

struct max30100_sensor_struct
{
    PulseOximeter pox;
    uint32_t ts_last_report;
};

typedef struct max30100_sensor_struct max30100_sensor_t;

void max30100_sensor_init(max30100_sensor_t *sensor);
void max30100_sensor_begin(max30100_sensor_t *sensor);
void max30100_sensor_update(max30100_sensor_t *sensor);
void max30100_sensor_display_data(max30100_sensor_t *sensor, Adafruit_SSD1306 *display);
int max30100_sensor_get_heart_rate(max30100_sensor_t *sensor);
int max30100_sensor_get_spo2(max30100_sensor_t *sensor);
void max30100_sensor_set_on_beat_detected_callback(max30100_sensor_t *sensor, void (*callback)());

#endif // MAX30100_SENSOR_H