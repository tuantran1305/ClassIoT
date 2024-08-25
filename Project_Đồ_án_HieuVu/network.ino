const char* ssid = "VU HIEU";
const char* pass = "0702394087";
const char* tb_server = "212.38.94.144";
const char* tb_token = "wVjNtkaRVv54Nndx9Zpo";
const int tb_port = 1884;
int ledState = LOW;
WiFiClient espClient;
PubSubClient client(espClient);
void initWifi() {
  Serial.println("Dang ket noi ...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Dang ket noi ...");
  }
  Serial.println("Da ket noi");
}
void reconnectWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Dang ket noi....");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
    }
  }
}
void init_tb()
{
  client.setServer(tb_server,tb_port);
  client.setCallback(on_message);
  Serial.println("Dang ket noi toi thingsboard");
}
void reconnected_tb()
{
  while(!client.connected())
  {
    if(client.connect("Hieu1",tb_token,NULL))
    {
      Serial.println("Da ket noi toi thingsboard");
      client.subscribe("v1/devices/me/attributes");
      Serial.println(" Da nhan du lieu tren share attributes ");
    }else{
      Serial.println("Khong duoc ket noi toi thingsboard");
    }
  }
}
void checkConnected_tb()
{
  if(!client.connected())
  {
    reconnected_tb();
  }
  client.loop();
}
bool isConnected_tb()
{
  return client.connected();
}
void publish_data(const char* payload1)
{
  if(client.connected()){
  client.publish("v1/devices/me/telemetry", payload1);
  }
  Serial.println(" Đã đưa được dữ liệu lên");
}
void set_gpio_status( int pin , boolean enabled) {
  if (pin == LED_PIN) {
    digitalWrite(LED_PIN, enabled ? HIGH : LOW);
    ledState = enabled;
  }
}
void on_message(const char* topic, byte* payload, unsigned int length) {

  Serial.println("On message");

  char json[length + 1];
  strncpy(json, (char*)payload, length);
  json[length] = '\0';

  // Serial.print("Topic: ");
  // Serial.println(topic);
  // Serial.print("Message: ");
  // Serial.println(json);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(json);
if (data.success()) {
    if (data.containsKey("phone")) {
      const char* phoneNumber = data["phone"];
      Serial.print(" So dien thoai lien he ");
      Serial.println(phoneNumber);
    }else if(data.containsKey("led"))
    {
      bool ledState = data["led"];
      set_gpio_status(LED_PIN, ledState);
    }
  }
} 