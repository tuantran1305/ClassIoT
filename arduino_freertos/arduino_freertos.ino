#include "freertos.h"

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Cấu hình MQTT client
  client.setServer(TB_SERVER, TB_PORT);

  // Tạo các task
  xTaskCreate(task_blink_led, "blink_led", 2048, NULL, 1, NULL);
  xTaskCreate(task_send_data, "send_data", 4096, NULL, 1, NULL);
  xTaskCreate(task_button_check, "button_check", 2048, NULL, 1, NULL);
}

void loop() {
  // FreeRTOS sẽ xử lý các task, không cần code trong loop()
}