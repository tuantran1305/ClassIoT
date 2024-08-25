#include "task1.h"
#include "freeRTOS.h"
void setup() {
  Wire.begin();
  Serial.begin(112500);
  pinMode(LED_PIN, OUTPUT);
  setupMPU();
  initWifi();
  set_MAX();
  setupOled();
  init_tb();
  xTaskCreatePinnedToCore(displayData, "Hien du lieu ", 4000, NULL, 1, &Task1_p, 1);
  xTaskCreatePinnedToCore(displayOled, "Hien du lieu ", 4000, NULL, 1, &Task2_p, 1);
 xTaskCreatePinnedToCore(WifiAndThingsBoard, "WiFi and MQTT", 4000, NULL, 1, &Task3_p, 1);
 xTaskCreatePinnedToCore(publishTask, "publish", 4000, NULL, 1, &Task4_p, 1);
}
void displayOled(void *pvparamerters) {
  while (1) {
    pox.update();
    if (millis() - tsLastReport1 > 1000) {
      displayOled();
      tsLastReport1 = millis(); 
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
void WifiAndThingsBoard(void *pvParameters) {
  while (1) {
    reconnectWifi();
    // reconnected_tb();
    checkConnected_tb();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void displayData(void *pvParameters) {
  // PulseOximeter pox;
  while (1) {
    pox.update();
    if (millis() - tsLastReport > 1000) {
      display_MPU();
      display_MAX();
      tsLastReport = millis();
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void publishTask(void *pvParameters) {
  while(1){
  if (isConnected_tb()) {
    String payload1 = "{\"heartRate\":\"" + String(Heart) + "\"," + "\"SPO2\":\"" + String(SPO2)  + "\"," + "\"temperature\":\"" + String(temperature) +  "\"," + "\"fallState\":\"" + String(detectFall) +  "\"}";
    Serial.print("Publishing payload: ");
    Serial.print(payload1);
    publish_data(payload1.c_str());
    Serial.println("Dữ liệu đã được đưa lên");
    }
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
}
