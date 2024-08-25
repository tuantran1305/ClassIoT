#include <Adafruit_MPU6050.h>
#include "Wire.h"
#include <Adafruit_Sensor.h>
extern bool fallDetected;
extern float totalAcceleration;
// extern unsigned long Millis ;
// extern unsigned long previous_data ;
void display_MPU();
void setupMPU();
// #define SDA_1 33
// #define SCL_1 32
// MAX30100
#include "MAX30100_PulseOximeter.h"
#include "Arduino.h"
void set_MAX();
void display_MAX();
void onBeatDetected();
PulseOximeter pox;
Adafruit_MPU6050 mpu;
extern uint32_t tsLastReport;
extern uint32_t tsLastReport1;
extern uint8_t Heart;
extern uint8_t SPO2;
extern float totalacceleration;
extern float totalgyro;
extern String detectFall;
extern float temperature;
// extern uint32_t tsLastReport2;
// extern uint32_t tsLastReport3;
// oled
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setupOled();
void displayOled();
// network
#include "WiFi.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
extern const char* ssid;
extern const char* pass;
extern const char* tb_sever;
extern const char* tb_token;
extern const int tb_port;
#define LED_PIN 26
extern int ledState;
void initWifi();
void reconnectWifi();
void init_tb();
void reconnected_tb();
void checkConnected_tb();
void publish_data(const char* payload1);
bool isConnected_tb();
void on_message(const char* topic, byte* payload, unsigned int length);