#include "network.h"
#include "freeRTOS.h"
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  initWiFi();
  initMQTT();

  xTaskCreatePinnedToCore(wifiAndThingsboardTask, "WiFi and MQTT Task", 4096, NULL, 1, &Task1_p, 0);
  xTaskCreatePinnedToCore(blinkTask, "Blink Task", 1024, NULL, 1, &Task2_p, 1);
  xTaskCreatePinnedToCore(publishTask, "Publish Task", 4096, NULL, 1, &Task3_p, 0);
  disableCore0WDT();
}

void loop() {
}

void wifiAndThingsboardTask(void *pvParameters) {
  while (1) {
    reconnectWiFi();
    reconnectMQTT();
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void blinkTask(void *pvParameters) {
  while (1) {
    digitalWrite(LED_PIN, ledState);
    ledState = !ledState;
    vTaskDelay(blinkInterval / portTICK_PERIOD_MS);
  }
}

void publishTask(void *pvParameters) {
  while (1) {
    if (isMQTTConnected()) {
      String payload = "{";
      payload += "\"interval\": blinkInterval,";
      payload += "}";

      Serial.print("Publishing payload: ");
      Serial.println(payload);

      publishData(payload.c_str());
    }
    vTaskDelay(30000 / portTICK_PERIOD_MS);
  }
}
