#include "esp32_thingsboard.h"

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  xTaskCreate(task_led_control, "LED Control", 2048, NULL, 1, NULL);
  xTaskCreate(task_wifi_mqtt_connection, "WiFi MQTT Connection", 4096, NULL, 2, NULL);
  xTaskCreate(task_read_dht, "Read DHT", 2048, NULL, 1, NULL);
  xTaskCreate(task_publish_data, "Publish Data", 4096, NULL, 1, NULL);
}

void loop() {

}