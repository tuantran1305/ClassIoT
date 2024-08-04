#include "esp32_thingsboard.h"

WiFiClient esp_client;
PubSubClient client(esp_client);
DHTesp dht11;
bool led_state = false;
bool relay_state = false;

void init_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(500));
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

bool reconnect_wifi() {
  unsigned long start_time = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start_time < 10000) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    vTaskDelay(pdMS_TO_TICKS(500));
    client.loop();
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to reconnect to WiFi");
    return false;
  }
  Serial.println("Reconnected to WiFi");
  return true;
}

void init_mqtt() {
  client.setServer(THINGSBOARD_SERVER, THINGSBOARD_PORT);
  client.setCallback([](char* topic, byte* payload, unsigned int length) {
    Serial.printf("Message received on topic: %s\n", topic);
    char* message = new char[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';
    Serial.printf("Payload: %s\n", message);
    subcribe_data(message);
    delete[] message;
  });
}

bool reconnect_mqtt() {
  unsigned long start_time = millis();
  while (!client.connected() && millis() - start_time < 10000) {
    if (client.connect("ESP32Client", THINGSBOARD_TOKEN, NULL)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(MQTT_TOPIC_ATTRIBUTES);
      return true;
    } else {
      vTaskDelay(pdMS_TO_TICKS(500));
      client.loop();
    }
  }
  Serial.println("Failed to reconnect to MQTT broker");
  return false;
}

void publish_data(float temperature, float humidity, bool relay_state) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = round(temperature * 100) / 100.0;
  doc["humidity"] = round(humidity * 100) / 100.0;
  doc["relay state"] = relay_state;

  char buffer[256];
  serializeJson(doc, buffer);
  client.publish(MQTT_TOPIC_TELEMETRY, buffer);
}

void subcribe_data(const char* payload) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println("Failed to parse JSON");
    return;
  }

  if (doc.containsKey("led_state")) {
    led_state = doc["led_state"].as<bool>();
    digitalWrite(LED_PIN, led_state);
    Serial.printf("LED state changed to: %s\n", led_state ? "ON" : "OFF");
  }
}

void print_connection_status() {
  Serial.printf("WiFi status: %s\n", WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
  Serial.printf("MQTT status: %s\n", client.connected() ? "Connected" : "Disconnected");
}

//task freeRTOS

void task_led_control(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  for (;;) {
    digitalWrite(LED_PIN, led_state);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void task_wifi_mqtt_connection(void *pv_parameters) {
  init_wifi();
  init_mqtt();
  unsigned long last_reconnect_attempt = 0;
  unsigned long last_status_print = 0;
  for (;;) {
    unsigned long time_now = millis();
    if (WiFi.status() != WL_CONNECTED || !client.connected()) {
      if (time_now - last_reconnect_attempt > 5000) {
        last_reconnect_attempt = time_now;
        if (WiFi.status() != WL_CONNECTED) {
          if (reconnect_wifi()) {
            last_reconnect_attempt = 0;
          }
        } else if (!client.connected()) {
          if (reconnect_mqtt()) {
            last_reconnect_attempt = 0;
            if (client.subscribe(MQTT_TOPIC_ATTRIBUTES)) {
              Serial.println("Subscribed to shared attributes");
            } else {
              Serial.println("Failed to subscribe to shared attributes");
            }
          }
        }
      }
    }
    if (time_now - last_status_print > 10000) {
      last_status_print = time_now;
      print_connection_status();
    }
    client.loop();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void task_read_dht(void *pv_parameters) {
  dht11.setup(DHT_PIN, DHTesp::DHT11);
  TempAndHumidity data;
  unsigned long last_read_time = 0;
  for (;;) {
    unsigned long time_now = millis();
    if (time_now - last_read_time > 3000) 
    {
      last_read_time = time_now;
      data = dht11.getTempAndHumidity();
      if (!isnan(data.temperature) && !isnan(data.humidity)) {
        Serial.printf("DHT11 Read - Temperature: %.2f°C, Humidity: %.2f%%\n", data.temperature, data.humidity);
      } else {
        Serial.println("Failed to read from DHT11 sensor!");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void task_publish_data(void *pv_parameters) {
  pinMode(RELAY_PIN, OUTPUT);
  unsigned long last_publish_time = 0;
  TempAndHumidity data;
  for (;;) {
    unsigned long time_now = millis();
    if (time_now - last_publish_time > 5000) {
      last_publish_time = time_now;
      data = dht11.getTempAndHumidity();
      relay_state = digitalRead(RELAY_PIN);
      if (!isnan(data.temperature) && !isnan(data.humidity)) {
        Serial.printf("Publishing - Temperature: %.2f°C, Humidity: %.2f%%, Relay State: %s\n", 
                      data.temperature, data.humidity, relay_state ? "ON" : "OFF");
        publish_data(data.temperature, data.humidity, relay_state);
      } else {
        Serial.println("Failed to read from DHT11 sensor for publishing!");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
