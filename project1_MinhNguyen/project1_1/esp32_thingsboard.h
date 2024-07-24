#ifndef ESP32_THINGSBOARD_H
#define ESP32_THINGSBOARD_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHTesp.h>

// Define GPIO
#define LED_PIN 2
#define DHT_PIN 4
#define RELAY_PIN 5

// Info WiFi
#define WIFI_SSID "Nha Mr Duc"
#define WIFI_PASSWORD "0901111488"

// Info ThingsBoard
#define THINGSBOARD_SERVER "thingsboard.srv524038.hstgr.cloud"
#define THINGSBOARD_PORT 1884
#define THINGSBOARD_TOKEN "hAtOjp2kejMeaJMFfpQE"

// Define topic MQTT
#define MQTT_TOPIC_ATTRIBUTES "v1/devices/me/attributes"
#define MQTT_TOPIC_TELEMETRY "v1/devices/me/telemetry"

// Function
void init_wifi();
bool reconnect_wifi();
void init_mqtt();
bool reconnect_mqtt();
void publish_data(float temperature, float humidity, bool relay_state);
void subcribe_data(const char* payload);

//Task freeRTOS
void task_led_control(void *pv_parameters);
void task_wifi_mqtt_connection(void *pv_parameters);
void print_connection_status();
void task_read_dht(void *pv_parameters);
void task_publish_data(void *pv_parameters);

// Khai báo các biến toàn cục
extern WiFiClient esp_client;
extern PubSubClient client;
extern DHTesp dht11;
extern bool led_state;
extern bool relay_state;

#endif