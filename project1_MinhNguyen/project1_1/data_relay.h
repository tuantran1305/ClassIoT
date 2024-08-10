#ifndef DATA_RELAY_H
#define DATA_RELAY_H

#include <Arduino.h>
#include "mqtt_manager.h"

// Define GPIO
#define RELAY_PIN 13

// Function declarations
void task_led_control(void *pvParameters);
void subcribe_data(const char* payload);

// Global variables
extern bool relay_state;

#endif