#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

// Info WiFi
#define WIFI_SSID "Nha Mr Duc"
#define WIFI_PASSWORD "0901111488"

// Function declarations
void init_wifi();
bool reconnect_wifi();
void check_wifi_status();
void task_wifi_connection(void *pv_parameters);

// Global variables
extern bool is_wifi_connected;

#endif