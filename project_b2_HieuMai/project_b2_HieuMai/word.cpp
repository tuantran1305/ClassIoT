#include "work.h"

const char* ssid = "P501";
const char* password = "88888888";
const char* mqtt_server = "thingsboard.srv524038.hstgr.cloud";
const int mqtt_port = 1884;
const char* token = "9qUdMYHALf0yUUgINrjZ";


int ledState = LOW;
int value = 0;
int val = 0;
boolean gpioState[] = { false, false };
bool ledStatus = false;

WiFiClient espClient;
PubSubClient client(espClient);

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
    if (client.connect("Iot_QuangHieu", token, NULL)) {
      Serial.println("connected Thingsboard");
      client.subscribe("v1/devices/me/attributes");
    } else {
      delay(5000);
    }
  }
  client.loop();
}

bool isMQTTConnected() {
  return client.connected();
}

void publishData(const char* payload) {
  if (client.connected()) {
    client.publish("v1/devices/me/telemetry", payload);
  }
}

void set_gpio_status(int pin, boolean enabled) {
  if (pin == GPIO_PIN) {
    // Output GPIOs state
    digitalWrite(GPIO, enabled ? HIGH : LOW);
    // Update GPIOs state
    gpioState[0] = enabled;
  }
}

String get_gpio_status() {
  // Prepare gpios JSON payload string
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data[String(GPIO_PIN)] = gpioState[0] ? true : false;
  char payload[256];
  data.printTo(payload, sizeof(payload));
  String strPayload = String(payload);
  Serial.print("Get gpio status: ");
  Serial.println(strPayload);
  return strPayload;
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

  // Decode JSON request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject((char*)json);

  if (!data.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  ledStatus = data["relay"];
  // Check request method
  String methodName = String((const char*)data["method"]);

  if (methodName.equals("getGpioStatus")) {
    // Reply with GPIO status
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
  } else if (methodName.equals("setGpioStatus")) {
    // Update GPIO status and reply
    set_gpio_status(data["params"]["pin"], data["params"]["enabled"]);
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
  }
}

void initServer() {
  //Connect WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(on_message);
}
