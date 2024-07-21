#include "led.h"
WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);
int last_button_state = 0;
int button_state = 0;
void blink_led_3() {
  digitalWrite(LEDPIN, HIGH);
  Serial.println("Trạng thái đèn là 3s/lần");
  delay(3000);
  digitalWrite(LEDPIN, LOW);
  delay(3000);
  tb.sendTelemetryData("LedState", 3);
}
void blink_led_1() {
  digitalWrite(LEDPIN, HIGH);
  Serial.println("Trạng thái đèn là 1s/lần");
  delay(1000);
  digitalWrite(LEDPIN, LOW);
  delay(1000);
  tb.sendTelemetryData("LedState", 1);
}
void connectWifi() {
  Serial.print("Đang kết nối ...");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_ID, WIFI_PASS);
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n Đã kết nối được Wifi ");
}
void connectThingsBoard() {
  while (!tb.connected()) {
    Serial.println("Đang kết nối với ThingsBoard");
    if (!tb.connect("ESP32Client", TOKEN, NULL)) {
      Serial.println("Kết nối thất bại với ThingsBoard");
    }
   else {
    Serial.println(" Đã kêt nối với ThingsBoard");
    if (millis() - lastSend > 2000) {
      lastSend = millis();
      changeState();
    }
  }
}
}
void changeState() {
  last_button_state = button_state;  // save the last state
  button_state = digitalRead(BUTTON);
  if (last_button_state == HIGH && button_state == LOW) {
    blink_led_1();
    delay(50);
  } else {
    blink_led_3();
    delay(50);
  }
}
