uint32_t tsLastReport1 = 0;
// uint32_t tsLastReport2 = 0;
// uint32_t tsLastReport3 = 0;
void setupOled() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  Serial.println("kết nối được với oled");
}
void displayOled() {
    mpu_read();
    display.clearDisplay();
    display.setCursor(0, 0);

    display.println("Accelerometer - m/s^2");
    display.print(ax, 1);
    display.print(", ");
    display.print(ay, 1);
    display.print(", ");
    display.print(az, 1);
    display.println("");

    display.println("Gyroscope - rps");
    display.print(gx, 1);
    display.print(", ");
    display.print(gy, 1);
    display.print(", ");
    display.print(gz, 1);
    display.println("");


    display.print("Heart rate:");
    display.print(pox.getHeartRate());
    display.print("bpm / SpO2:");
    display.print(pox.getSpO2());
    display.println("%");
    display.display();
  }