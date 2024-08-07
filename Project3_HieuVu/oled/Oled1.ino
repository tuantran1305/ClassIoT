bool fallDetected = false;
unsigned long Millis = 0;
unsigned long previous_data = 0 ;
Adafruit_SSD1306 display;
Adafruit_MPU6050 mpu;
void setupOLED() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C
  display.clearDisplay();                     // Clear the buffer
  display.setTextColor(WHITE);                // Set color of the text
  display.setRotation(0);                     // Set orientation. Goes from 0, 1, 2 or 3
  display.setTextWrap(false);                 // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
  display.dim(0);                             //Set brightness (0 is maximun and 1 is a little dim)
}
void setupMPU() {

  if (!mpu.begin(0x68)) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  Serial.println("Found a MPU-6050 sensor");
}
void detectFalling() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float totalAcceleration = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z);
  Serial.print("Tong do lon gia toc : ");
  Serial.println(totalAcceleration);
  if (abs(totalAcceleration - 14) > 6) {
    fallDetected = true;
    Serial.println("Phat hien te nga !");
  }
}
void display_MPU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  display.clearDisplay();
  display.setTextSize(0);
  display.setCursor(0, 0);
  display.println("Accelerometer - m/s^2");
  display.print(a.acceleration.x, 1);
  display.print(", ");
  display.print(a.acceleration.y, 1);
  display.print(", ");
  display.print(a.acceleration.z, 1);
  display.println("");
  display.display();
}