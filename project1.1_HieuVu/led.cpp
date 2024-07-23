#include "Arduino.h"
#include "led.h"
const char* ssid = "VU HIEU";
const char* pass = "0702394087";
const char* tb_server = "thingsboard.srv524038.hstgr.cloud";
const char* token = "wVjNtkaRVv54Nndx9Zpo";
const int tb_port = 1884;
int value_earth;
int percent;
int buttonState;
int ledState = LOW;
int lastbuttonState = LOW;
WiFiClient espClient;
PubSubClient client(espClient);
void init_tb() {
  client.setServer(tb_server, tb_port);
  client.setCallback(on_message);
  reconnected_tb();
}
void reconnected_tb() {
  while (!client.connected()) {
    if (client.connect("Hieu", token, NULL)) {
      client.subscribe("v1/devices/me/attributes");
      Serial.println("connected");
    } else {
      delay(500);
    }
  }
}
bool isConnected_tb() {
  return client.connected();
}
void check_connectTb() {
  if (!client.connected()) {
    reconnected_tb();
  }
  client.loop();
}
void initWifi() {
  Serial.println("Dang ket noi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Dang ket noi");
    delay(500);
  }
  Serial.println("Da ket noi");
}
void reconnectWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Dang ket noi...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}
void publishData(const char* payload1) {
  if (client.connected()) {
    client.publish("v1/devices/me/telemetry", payload1);
  }
}
void earth() {
  value_earth = analogRead(EARTH_SENSOR);
  percent = map(value_earth, 0, 4095, 0, 100);
  Serial.print(percent);
  Serial.println("%");
}
void set_gpio_status( int pin , boolean enabled) {
  if (pin == LED_PIN) {
    digitalWrite(LED_PIN, enabled ? HIGH : LOW);
    ledState = enabled;
  }
}
void on_message(const char* topic, byte* payload, unsigned int length) {

  Serial.println("On message");

  char json[length + 1];
  strncpy(json, (char*)payload, length);
  json[length] = '\0';

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(json);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(json);

  if (data.success()) {
    if (data.containsKey("led")) {
      bool ledState = data["led"];
      set_gpio_status(LED_PIN, ledState);
    }
  } else {
    Serial.println("Khong xu ly duoc du lieu");
  }
}