#include "Wire.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h> 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
extern const char* ssid ;
extern const char* pass;
extern const char* tb_sever;
extern const char* tb_token;
extern const int tb_port;
extern unsigned long Millis ;
extern unsigned long previous_data ;
extern int count ;
extern float totalAcceleration ; 
extern bool fallDetected;
void display_MPU();
void detectFalling();
void init_tb();
void reconnected_tb();
void checkConnected_tb();
void publish_data(const char* payload);
void initWifi();
void reconnectWifi();
bool isConnected_tb();