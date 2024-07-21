#include "lib.h"

// Khởi tạo các biến toàn cục
volatile int blink_interval = 3000;
volatile bool button_pressed = false;

WiFiClient esp_client;
PubSubClient client(esp_client);

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("ESP32Client", mqttToken, NULL)) {
      Serial.println("Connected to ThingsBoard");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
  }
}

void taskBlinkLed(void *pvParameters) {
  for (;;) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(blinkInterval / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(blinkInterval / portTICK_PERIOD_MS);
  }
}

void taskSendData(void *pvParameters) {
  for (;;) {
    if (!mqttClient.connected()) {
      reconnect();
    }
    mqttClient.loop();

    // Gửi dữ liệu lên ThingsBoard
    String payload = "{\"blinkInterval\":" + String(blinkInterval) + "}";
    mqttClient.publish("v1/devices/me/telemetry", payload.c_str());

    vTaskDelay(5000 / portTICK_PERIOD_MS); // Gửi dữ liệu mỗi 5 giây
  }
}

void taskCheckButton(void *pvParameters) {
  int lastButtonState = HIGH;
  for (;;) {
    int buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == LOW && lastButtonState == HIGH) {
      buttonPressed = true;
      blinkInterval = (blinkInterval == 3000) ? 1000 : 3000;
      Serial.println("Button pressed, new blink interval: " + String(blinkInterval));
    }
    lastButtonState = buttonState;
    vTaskDelay(50 / portTICK_PERIOD_MS); // Debounce delay
  }
}