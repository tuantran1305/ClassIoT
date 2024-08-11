#include "work.h"
#include "freeRTOS.h"

void setup() {
  Serial.begin(9600);
  //set GPIO pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(GPIO, OUTPUT);

  //cấu hình MQTT client
  initServer();
  reconnectMQTT();

  //khởi tạo Task
  xTaskCreatePinnedToCore(wifiAndMQTTTask, "WiFi and MQTT Task", 4096, NULL, 1, &Task1_p, 0);
  xTaskCreatePinnedToCore(LedTask, "Led Task", 1024, NULL, 1, &Task2_p, 0);
  xTaskCreatePinnedToCore(publishTask, "Publish Task", 4096, NULL, 1, &Task3_p, 0);
  xTaskCreatePinnedToCore(ReadSensorTask, "Read Sensor Task", 4096, NULL, 1, &Task4_p, 0);
  //disableCore0WDT();
}

void loop() {
}

void wifiAndMQTTTask(void *pvParameters) {
  while (1) {
    reconnectWiFi();
    reconnectMQTT();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void LedTask(void *pvParameters) {
  while (1) {
    digitalWrite(LED_PIN, ledStatus ? HIGH : LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void publishTask(void *pvParameters) {
  while (1) {
    if (isMQTTConnected()) {
      String ledStatusStr = ledStatus ? "true" : "false";
      String payload = "{\"hum\":" + String(value) + ",\"relay\":" + ledStatusStr + "}";

      Serial.print("Publishing payload: ");
      Serial.println(payload);

      publishData(payload.c_str());
    }
    vTaskDelay(30000 / portTICK_PERIOD_MS);
  }
}

void ReadSensorTask(void *pvParameters) {
  while (1) {
    val = analogRead(A0);  //đọc giá trị hiệu điện thế của cảm biến
    value = map(val, 0, 4096, 0, 100);
    Serial.print("giá trị cảm biến độ ẩm:");
    Serial.println(value);  //Xuất ra serial Monitor
    vTaskDelay(5500 / portTICK_PERIOD_MS);
  }
}
