#include "WiFi.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#define LED_PIN 25
#define BUTTON_PIN 27
#define EARTH_SENSOR 32
extern const char* tb_server;
extern const char* token;
extern const int tb_port;
extern int ledState;
extern int percent;
extern int buttonState;
extern int lastbuttonState;
extern const char* ssid;
extern const char* pass;
extern int  value_earth ;
void initWifi();
void reconnectWifi();
void init_tb();
void reconnected_tb();
bool isConnected_tb();
void check_connectTb();
void earth();
void publishData(const char* payload1);
//String get_gpio_status();
void set_gpio_status(int pin, boolean enabled);
void on_message(const char* topic, byte* payload, unsigned int length);