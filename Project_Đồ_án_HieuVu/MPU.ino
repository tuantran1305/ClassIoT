String detectFall = "false";
float totalacceleration;
float totalgyro;
float temperature;
void setupMPU() {
  // Wire1.begin(SDA_1, SCL_1);
  Serial.println("Adafruit MPU6050 test!");
  // I2Cone.begin(SDA_1, SCL_1, 100000);
  bool status1 = mpu.begin(0x68, &Wire);
  if (!status1) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}
void display_MPU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  temperature = temp.temperature;
  totalacceleration = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z);
  Serial.print("Total Acceleration");
  Serial.println(totalacceleration);
  totalgyro = sqrt(g.gyro.x * g.gyro.x + g.gyro.y * g.gyro.y + g.gyro.z * g.gyro.z);
  Serial.print("Total gyroscope");
  Serial.println(totalgyro);
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");
  Serial.println("");
  //detect Fall
    if (abs( totalacceleration - 10) > 4) {
    detectFall = "true";
    Serial.println("Fall detected!");
  } else {
    detectFall = "false";
  }
}
