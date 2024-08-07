#include "oled.h"
#include "freeRTOS.h"
void setup() {
  Serial.begin(115200);
  setupOLED();
  setupMPU();
  init_tb();
  initWifi();
  xTaskCreatePinnedToCore(display_MPU, "Hien thi ten ", 4096, NULL, 1, &Task1_p, 1);
  xTaskCreatePinnedToCore(detectFalling, "Trạng thái té ngã ", 4096, NULL, 1, &Task2_p, 1);
  xTaskCreatePinnedToCore(WifiAndThingsBoard, "WiFi and MQTT", 4000, NULL, 1, &Task3_p, 0);
  xTaskCreatePinnedToCore(publishTask, "publish", 4000, NULL, 1, &Task4_p, 0);
}
void WifiAndThingsBoard(void *pvParameters) {
  while (1) {
    checkConnected_tb();
    reconnectWifi();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
void publishTask(void *pvParameters) {
  while(1){
  if (isConnected_tb()) {
    String payload = "{\"Falling\":\"" + String(fallDetected) + "\"}";
    Serial.print("Publishing payload: ");
    Serial.print(payload);
    publish_data(payload.c_str());
    }
    vTaskDelay(30000 / portTICK_PERIOD_MS);
  }
}
void detectFalling(void *parameter)
{
while(1)
{
  detectFalling();
  vTaskDelay(2000 / portTICK_PERIOD_MS);
}
}
void display_MPU(void *parameter) {
  while (1) {
    if (millis() - previous_data > 2000) {
      display_MPU();
      previous_data = millis();
    }
  }
  vTaskDelay(50 / portTICK_PERIOD_MS);
}
void loop() {
  // put your main code here, to run repeatedly:
}
