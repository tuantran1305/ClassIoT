#include "freeRTOS.h"
#include "led.h"
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(EARTH_SENSOR, OUTPUT);
  initWifi();
  init_tb();
  xTaskCreatePinnedToCore(turn_led, "Turn Led", 4000, NULL, 1, &Task1_p, 1);
  xTaskCreatePinnedToCore(WifiAndThingsBoard, "WiFi and MQTT", 4000, NULL, 1, &Task2_p, 0);
  xTaskCreatePinnedToCore(publishTask, "publish", 4000, NULL, 1, &Task3_p, 0);
  xTaskCreatePinnedToCore(earth, "Earth", 4000, NULL, 1, &Task4_p, 1);
  xTaskCreatePinnedToCore(callback, "Call Back Data", 4000, NULL, 1, &Task5_p, 1);
}
void turn_led(void *pvParameters) {
  while (1) {
    buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH && lastbuttonState == LOW) {
      digitalWrite(LED_PIN, ledState);
      ledState = !ledState;
      Serial.println(!ledState);
    }
    lastbuttonState = buttonState;
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
void WifiAndThingsBoard(void *pvParameters) {
  while (1) {
    check_connectTb();
    reconnectWifi();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void publishTask(void *pvParamaters) {
  while (1) {
    if (isConnected_tb()) {
      String payload1 = "{";
      payload1 += "\"Độ ẩm đất\": value_earth,";
      payload1 += String(percent);
      payload1 += "}";
      Serial.print("Publishing payload: ");
      Serial.println(payload1);
      publishData(payload1.c_str());
    }
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
void earth(void *pvParameters) {
  while (1) {
    earth();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void callback(void *pvParameters) {
  while (1) {
    if (isConnected_tb()) {
      char *topic;
      byte *payload;
      int length;
      on_message(topic, payload, length);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:
}
