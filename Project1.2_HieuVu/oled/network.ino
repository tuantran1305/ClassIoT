WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "VU HIEU";
const char* pass = "0702394087";
const char* tb_server = "212.38.94.144:8081";
const char* tb_token = "VF6z1aKBrX66sphi3QAP";
const int tb_port = 1884;
void initWifi() {
  Serial.println("Dang ket noi ...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Dang ket noi ...");
    delay(500);
  }
  Serial.println("Da ket noi");
}
void reconnectWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Dang ket noi....");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}
void init_tb()
{
  client.setServer(tb_server,tb_port);
  reconnected_tb();
}
void reconnected_tb()
{
  while(!client.connected())
  {
    if(client.connect("HIEUVU",tb_token,NULL))
    {
      Serial.println("Da ket noi toi thingsboard");
    }else{
      delay(500);
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
void publish_data(const char* payload)
{
  if(client.connected()){
  client.publish("v1/devices/me/telemetry", payload);
  }
}
bool isConnected_tb()
{
  return client.connected();
}