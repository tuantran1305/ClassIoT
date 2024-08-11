#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//Conf WiFi and MQTT
extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const int mqtt_port;
extern const char* token;
//Conf Led and Sensor
extern int ledState;
extern int value;
extern int val;
extern bool ledStatus;
#define LED_PIN 5
#define A0 34
#define GPIO 0
#define GPIO_PIN 3
// Function
void initServer();
void initWiFi();
void initMQTT();
void reconnectWiFi();
void reconnectMQTT();
bool isMQTTConnected();
void publishData(const char* payload);