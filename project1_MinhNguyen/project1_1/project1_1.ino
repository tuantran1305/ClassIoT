#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "data_relay.h"

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);

  xTaskCreate(task_led_control, "LED Control", 2048, NULL, 1, NULL);
  xTaskCreate(task_wifi_connection, "WiFi Connection", 4096, NULL, 1, NULL);
  xTaskCreate(task_mqtt_connection, "MQTT Connection", 4096, NULL, 1, NULL);  
}

void loop() {

}