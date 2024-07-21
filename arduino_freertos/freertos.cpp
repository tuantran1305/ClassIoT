#include "freertos.h"

// Khởi tạo các biến toàn cục
volatile int blink_interval = 3000;
volatile bool button_pressed = false;

WiFiClient esp_client;
PubSubClient client(esp_client);

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    Serial.print("Server: ");
    Serial.println(TB_SERVER);
    Serial.print("Port: ");
    Serial.println(TB_PORT);
    Serial.print("Token: ");
    Serial.println(TB_TOKEN);
    
    if (client.connect("ESP32Client", TB_TOKEN, NULL)) {
      Serial.println("Connected to ThingsBoard");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
  }
}

void task_blink_led(void *pvParameters) {
  for (;;) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(blink_interval / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(blink_interval / portTICK_PERIOD_MS);
  }
}

void task_send_data(void *pvParameters) {
  for (;;) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    // Gửi dữ liệu lên ThingsBoard
    String payload = "{\"blink_interval\":" + String(blink_interval) + "}";
    client.publish("v1/devices/me/telemetry", payload.c_str());

    vTaskDelay(5000 / portTICK_PERIOD_MS); // Gửi dữ liệu mỗi 5 giây
  }
}

void task_button_check(void *pvParameters) {
  int last_button_state = HIGH;
  for (;;) {
    int button_state = digitalRead(BUTTON_PIN);
    if (button_state == LOW && last_button_state == HIGH) {
      button_pressed = true;
      blink_interval = (blink_interval == 3000) ? 1000 : 3000;
      Serial.println("Button pressed, new blink interval: " + String(blink_interval));
    }
    last_button_state = button_state;
    vTaskDelay(50 / portTICK_PERIOD_MS); // Debounce delay
  }
}