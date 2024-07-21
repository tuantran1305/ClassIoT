#ifndef CONFIG_WIFI_H
#define CONFIG_WIFI_H

#include <WiFi.h>
#include <PubSubClient.h>

// WiFi and MQTT Configuration
extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const int mqtt_port;
extern const char* token;

// LED and Blink Configuration
extern int ledState;
extern int blinkInterval;
#define LED_PIN 2

// Function
void initWiFi();
void initMQTT();
void reconnectWiFi();
void reconnectMQTT();
bool isMQTTConnected();
void publishData(const char* payload);

#endif  // CONFIG_WIFI_H
