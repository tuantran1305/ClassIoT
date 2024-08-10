#include "data_relay.h"

bool relay_state = false;

void task_led_control(void *pvParameters) {
  pinMode(RELAY_PIN, OUTPUT);
  for (;;) {
    digitalWrite(RELAY_PIN, relay_state);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void subcribe_data(const char* payload) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println("Failed to parse JSON");
    return;
  }

  if (doc.containsKey("relay_state")) {
    relay_state = doc["relay_state"].as<bool>();
    digitalWrite(RELAY_PIN, relay_state);
    Serial.printf("Relay state changed to: %s\n", relay_state ? "OFF" : "ON");
  }
}