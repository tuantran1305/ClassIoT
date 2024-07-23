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
  reconnected_tb();
}
void reconnected_tb() {
  while (!client.connected()) {
    if (client.connect("Hieu", token, NULL)) {
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
  delay(1000);
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
  percent = map(value_earth, 0, 1023, 0, 100);
  Serial.print(percent);
  Serial.println("%");
}
String get_gpio_status() {
  // Prepare gpios JSON payload string
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data[String(LED_PIN)] = ledState ? true : false;
  char payload[256];
  data.printTo(payload, sizeof(payload));
  String strPayload = String(payload);
  Serial.print("Get gpio status: ");
  Serial.println(strPayload);
  return strPayload;
}
void set_gpio_status(int pin, boolean enabled) {
  if (pin == LED_PIN) {
    // Output GPIOs state
    digitalWrite(LED_PIN, enabled ? HIGH : LOW);
    // Update GPIOs state
    ledState = enabled;
  }
}
void on_message(const char* topic, byte* payload, unsigned int length) {

  Serial.println("On message");

  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(json);

  // Decode JSON request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject((char*)json);

  if (!data.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

  // Check request method
  String methodName = String((const char*)data["method"]);

  if (methodName.equals("getGpioStatus")) {
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
  } else if (methodName.equals("setGpioStatus")) {
    set_gpio_status(data["params"]["pin"], data["params"]["enabled"]);
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
  }
}