#include "mqtt_manager.h"
#include "wifi_manager.h"

WiFiClient esp_client;
PubSubClient client(esp_client);
bool is_mqtt_connected = false;

void init_mqtt() {
  client.setServer(THINGSBOARD_SERVER, THINGSBOARD_PORT);
  client.setCallback([](char* topic, byte* payload, unsigned int length) {
    Serial.printf("Message received on topic: %s\n", topic);
    char* message = new char[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';
    Serial.printf("Payload: %s\n", message);
    subcribe_data(message);
    delete[] message;
  });
}

bool reconnect_mqtt() {
  unsigned long start_time = millis();
  while (!client.connected() && millis() - start_time < 10000) {
    if (client.connect("ESP32Client", THINGSBOARD_TOKEN, NULL)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(MQTT_TOPIC_ATTRIBUTES);
      return true;
    } else {
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
  Serial.println("Failed to reconnect to MQTT broker");
  return false;
}

void check_mqtt_status() {
  bool current_mqtt_status = client.connected();
  if (current_mqtt_status != is_mqtt_connected) {
    is_mqtt_connected = current_mqtt_status;
    if (is_mqtt_connected) {
      Serial.println("MQTT connected");
    } else {
      Serial.println("MQTT disconnected");
    }
  }
}

void task_mqtt_connection(void *pv_parameters) {
  init_mqtt();
  unsigned long last_reconnect_attempt = 0;
  for (;;) {
    unsigned long time_now = millis();
    if (is_wifi_connected && !client.connected()) {
      if (time_now - last_reconnect_attempt > 5000) {
        last_reconnect_attempt = time_now;
        if (reconnect_mqtt()) {
          last_reconnect_attempt = 0;
          if (client.subscribe(MQTT_TOPIC_ATTRIBUTES)) {
            Serial.println("Subscribed to shared attributes");
          } else {
            Serial.println("Failed to subscribe to shared attributes");
          }
        }
      }
    }
    check_mqtt_status();
    client.loop();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
