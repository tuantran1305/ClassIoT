#include "lib.h"

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
  xTaskCreate(taskBlinkLed, "taskCheckButton", 8000, NULL, 1, NULL);
  xTaskCreate(taskSendData, "taskCheckButton", 8000, NULL, 1, NULL);
  xTaskCreate(taskCheckButton, "taskCheckButton", 8000, NULL, 1, NULL);
}

void loop() {
  // FreeRTOS sẽ xử lý các task, không cần code trong loop()
}