#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Info ThingsBoard
#define THINGSBOARD_SERVER "thingsboard.srv524038.hstgr.cloud"
#define THINGSBOARD_PORT 1884
#define THINGSBOARD_TOKEN "iMVw4kssXJQDkPivTnZC"

// Define topic MQTT
#define MQTT_TOPIC_ATTRIBUTES "v1/devices/me/attributes"

// Function declarations
void init_mqtt();
bool reconnect_mqtt();
void check_mqtt_status();
void task_mqtt_connection(void *pv_parameters);
void subcribe_data(const char* payload);

// Global variables
extern PubSubClient client;
extern bool is_mqtt_connected;

#endif