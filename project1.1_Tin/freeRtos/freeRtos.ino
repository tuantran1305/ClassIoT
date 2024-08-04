#include "lib.h"

void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(2, INPUT);
  pinMode(34, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  client.onMessage(Led_control)

  xTaskCreate(reconnectWiFi, "reconnectWiFi", 8000, NULL, 1, NULL);
  xTaskCreate(connect_ThingsBoard, "ThingsBoardTask", 8000, NULL, 1, NULL);
  xTaskCreate(sendSensorData, "sendSensorData", 8000, NULL, 1, NULL);
  xTaskCreate(messageHandlingTask, "messageHandlingTask", 8000, NULL, 1, NULL);


}



void loop() {

}