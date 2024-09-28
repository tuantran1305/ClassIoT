uint32_t tsLastReport = 0;
uint8_t Heart;
uint8_t SPO2;
void set_MAX() {
  Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
  }
}
void display_MAX() {
  SPO2 = int(pox.getSpO2());
  Heart = int(pox.getHeartRate());
  Serial.print("Heart rate:");
  Serial.print(pox.getHeartRate());
  Serial.print("bpm / SpO2:");
  Serial.print(pox.getSpO2());
  Serial.println("%");
}
