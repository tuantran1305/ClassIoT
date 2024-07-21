#ifndef FREERTOS_ESP32_H
#define FREERTOS_ESP32_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Định nghĩa chân GPIO
#define LED_PIN 2
#define BUTTON_PIN 0

// Cấu hình WiFi
#define WIFI_SSID "Nha Mr Duc"
#define WIFI_PASSWORD "0901111488"

// Cấu hình ThingsBoard
#define TB_SERVER "thingsboard.srv524038.hstgr.cloud"
#define TB_PORT 1884
#define TB_TOKEN "hAtOjp2kejMeaJMFfpQE"

// Khai báo các hàm task
void task_blink_led(void *pvParameters);
void task_send_data(void *pvParameters);
void task_button_check(void *pvParameters);

// Khai báo hàm reconnect
void reconnect();

// Khai báo biến toàn cục
extern volatile int blink_interval;
extern volatile bool button_pressed;
extern PubSubClient client;

#endif // FREERTOS_ESP32_H