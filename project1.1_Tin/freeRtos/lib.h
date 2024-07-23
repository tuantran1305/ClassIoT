#include <WiFi.h>
#include <MQTT.h>
#include <ArduinoJson.h>

#define WIFI_SSID "TRAN LONG"
#define WIFI_PASSWORD "congtytranlong"
#define TB_SERVER "thingsboard.srv524038.hstgr.cloud"
#define TB_PORT 1884
#define CLIENT_ID "tin"
#define USERNAME "trungtin"
#define PASSWORD "trungtin1"

WiFiClient net;
MQTTClient client;

StaticJsonDocument<200> doc;
void sendSensorData();
void connect_ThingsBoard();
void led_Control(String &topic, String &payload);
void reconnectWiFi();
void messageHandlingTask(void *pvParameters);