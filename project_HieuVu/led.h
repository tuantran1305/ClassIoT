#include "WiFi.h"
#include "ThingsBoard.h"
#include <Arduino_MQTT_Client.h>
#define WIFI_ID "VUHIEU"
#define WIFI_PASS ""
#define TB_SEVER "thingsboard.srv524038.hstgr.cloud"
#define TOKEN "n6hu3i1t7jh7c0w5cphj"
constexpr uint32_t MAX_MESSAGE_SIZE = 1024U;
const int LEDPIN = 10;
const int BUTTON = 16;
extern int last_button_state ;
extern int button_state ;
extern unsigned long lastSend;
void blink_led_3();
void blink_led_1();
void connectWifi();
void connectThingsBoard();
void changeState();