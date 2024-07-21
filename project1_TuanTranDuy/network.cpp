#include "network.h"

const char* ssid = "TRAN LONG";
const char* password = "congtytranlong";
const char* mqtt_server = "thingsboard.srv524038.hstgr.cloud";
const int mqtt_port = 1884;
const char* token = "";

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

int ledState = LOW;
int blinkInterval = 1000;

WiFiClient espClient;
PubSubClient client(espClient);

void initWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void initMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  reconnectMQTT();
}

void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", token, NULL)) {
      Serial.println("connected");
    } else {
      delay(5000);
    }
  }
}

bool isMQTTConnected() {
  return client.connected();
}

void publishData(const char* payload) {
  if (client.connected()) {
    client.publish("v1/devices/me/telemetry", payload);
  }
}
