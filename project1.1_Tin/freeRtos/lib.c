void sendSensorData() {
  int value = analogRead(A0);  // Đọc giá trị cảm biến
  Serial.print("Sensor value: ");
  Serial.println(value);

  // Tạo payload JSON
  String payload = "{\"Soil\":";
  payload += value;
  payload += "}";

  // Gửi dữ liệu lên ThingsBoard
  client.publish("v1/devices/me/telemetry", payload.c_str());

  delay(10000);  // Gửi dữ liệu mỗi 10 giây
}

void connect_ThingsBoard() {
  client.begin(TB_SERVER,TB_PORT, net);
  Serial.print("connecting...");
  while (!client.connect(CLIENT_ID, USERNAME,PASSWORD)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connected!");
  client.subscribe("v1/devices/me/attributes");
}

void led_Control(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  deserializeJson(doc, payload);

  if (doc["method"] == "get_LED") {
    char buffer[50];
    sprintf(buffer, "{'method' : 'get_LED', 'params' : '%d'}", led_state);
    client.publish("v1/devices/me/telemetry", buffer);
  }

  if (doc["method"] == "set_LED") {
    bool state = doc["params"];
    Serial.print("Received command to set LED to ");
    Serial.println(state);
    digitalWrite(5, state);
    led_state = state;
  }
}

void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("Reconnected!");
  }
}
void messageHandlingTask(void *pvParameters) {
  while (1) {
    client.loop();                         // Kiểm tra các tin nhắn đến
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Kiểm tra tin nhắn mỗi 100ms
  }
}