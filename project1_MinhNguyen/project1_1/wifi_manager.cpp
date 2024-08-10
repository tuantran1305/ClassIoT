#include "wifi_manager.h"

bool is_wifi_connected = false;

void init_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(500));
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  is_wifi_connected = true;
}

bool reconnect_wifi() {
  unsigned long start_time = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start_time < 10000) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to reconnect to WiFi");
    return false;
  }
  Serial.println("Reconnected to WiFi");
  return true;
}

void check_wifi_status() {
  bool current_wifi_status = (WiFi.status() == WL_CONNECTED);
  if (current_wifi_status != is_wifi_connected) {
    is_wifi_connected = current_wifi_status;
    if (is_wifi_connected) {
      Serial.println("WiFi connected");
    } else {
      Serial.println("WiFi disconnected");
    }
  }
}

void task_wifi_connection(void *pv_parameters) {
  init_wifi();
  unsigned long last_reconnect_attempt = 0;
  for (;;) {
    unsigned long time_now = millis();
    if (WiFi.status() != WL_CONNECTED) {
      if (time_now - last_reconnect_attempt > 5000) {
        last_reconnect_attempt = time_now;
        if (reconnect_wifi()) {
          last_reconnect_attempt = 0;
        }
      }
    }
    check_wifi_status();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}