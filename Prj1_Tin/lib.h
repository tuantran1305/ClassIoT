

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define LED_PIN 2
#define BUTTON_PIN 0
#define WIFI_SSID "TRAN LONG"
#define WIFI_PASSWORD "congtytranlong"
#define TB_SERVER "thingsboard.srv524038.hstgr.cloud"
#define TB_PORT 1884
#define TB_TOKEN "BmL9PZSsyZOHUaRTZnxf"
void reconnect();
void taskBlinkLed(void *pvParameters);
void taskSendData(void *pvParameters);
void taskCheckButton(void *pvParameters);
extern volatile int blink_interval;
extern volatile bool button_pressed;
extern PubSubClient client;
