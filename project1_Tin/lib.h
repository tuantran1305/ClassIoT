

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Định nghĩa chân GPIO
#define LED_PIN 2
#define BUTTON_PIN 0

// Cấu hình WiFi
#define WIFI_SSID "TRAN LONG"
#define WIFI_PASSWORD "congtytranlong"

// Cấu hình ThingsBoard
#define TB_SERVER "thingsboard.srv524038.hstgr.cloud"
#define TB_PORT 1884
#define TB_TOKEN "BmL9PZSsyZOHUaRTZnxf"

// Khai báo các hàm task
void taskBlinkLed(void *pvParameters);
void taskSendData(void *pvParameters);
void taskCheckButton(void *pvParameters);

// Khai báo hàm reconnect
void reconnect();

// Khai báo biến toàn cục
extern volatile int blink_interval;
extern volatile bool button_pressed;
extern PubSubClient client;
