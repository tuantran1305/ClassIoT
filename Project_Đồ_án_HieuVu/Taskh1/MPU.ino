String detectFall = "false";
const int MPU_addr = 0x68;  // Địa chỉ I2C của MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;
boolean fall = false; // Lưu trữ nếu đã xảy ra té ngã
boolean trigger1 = false; // Lưu trữ nếu đã kích hoạt trigger 1 (ngưỡng thấp)
boolean trigger2 = false; // Lưu trữ nếu đã kích hoạt trigger 2 (ngưỡng cao)
boolean trigger3 = false; // Lưu trữ nếu đã kích hoạt trigger 3 (thay đổi góc)
byte trigger1count = 0; // Đếm số vòng lặp kể từ khi trigger 1 được kích hoạt
byte trigger2count = 0; // Đếm số vòng lặp kể từ khi trigger 2 được kích hoạt
byte trigger3count = 0; // Đếm số vòng lặp kể từ khi trigger 3 được kích hoạt
int angleChange = 0;
 float temperature;
void setupMPU() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // Thanh ghi PWR_MGMT_1
  Wire.write(0);     // Đặt về 0 để đánh thức MPU-6050
  Wire.endTransmission(true);
  Serial.println("Đã viết lệnh vào IMU");
  }

void display_MPU() {
    mpu_read();
  ax = (AcX - 2050) / 16384.00;
  ay = (AcY - 77) / 16384.00;
  az = (AcZ - 1947) / 16384.00;
  gx = (GyX + 270) / 131.07;
  gy = (GyY - 351) / 131.07;
  gz = (GyZ + 136) / 131.07;
  temperature = Tmp / 340.00 + 36.53; 
  float Raw_Amp = pow(pow(ax, 2) + pow(ay, 2) + pow(az, 2), 0.5);
  int Amp = Raw_Amp * 10;  // Nhân với 10 vì giá trị nằm trong khoảng từ 0 đến 1
  Serial.print("Giá trị Amp");
  Serial.println(Amp);
if (Amp <= 2 && trigger2 == false) { // Nếu biên độ thấp hơn ngưỡng thấp (0.4g)
    trigger1 = true;
    Serial.println("KÍCH HOẠT TRIGGER 1");
  }
  
  if (trigger1 == true) {
    trigger1count++;
    if (Amp >= 12) { // Nếu biên độ vượt qua ngưỡng cao (3g)
      trigger2 = true;
      Serial.println("KÍCH HOẠT TRIGGER 2");
      trigger1 = false;
      trigger1count = 0;
    }
  }
  
  if (trigger2 == true) {
    trigger2count++;
    angleChange = pow(pow(gx, 2) + pow(gy, 2) + pow(gz, 2), 0.5);
    Serial.println(angleChange);
    if (angleChange >= 30 && angleChange <= 400) { // Nếu thay đổi góc trong khoảng từ 80 đến 100 độ
      trigger3 = true;
      trigger2 = false;
      trigger2count = 0;
      Serial.println(angleChange);
      Serial.println("KÍCH HOẠT TRIGGER 3");
    }
  }
  
  if (trigger3 == true) {
    trigger3count++;
    if (trigger3count >= 10) {
      angleChange = pow(pow(gx, 2) + pow(gy, 2) + pow(gz, 2), 0.5);
      Serial.println(angleChange);
      if ((angleChange >= 0) && (angleChange <= 10)) { // Nếu thay đổi góc nằm trong khoảng từ 0 đến 10 độ
        fall = true;
        trigger3 = false;
        trigger3count = 0;
        Serial.println(angleChange);
      } else { // Người dùng đã lấy lại tư thế bình thường
        trigger3 = false;
        trigger3count = 0;
        Serial.println("TRIGGER 3 HỦY KÍCH HOẠT");
      }
    }
  }
  
  if (fall == true) { // Trong trường hợp phát hiện té ngã
    Serial.println("PHÁT HIỆN TÉ NGÃ");
    fall = false;
    detectFall = "true";
  }
  
  if (trigger2count >= 6) { // Cho phép 0.5s để thay đổi tư thế
    trigger2 = false;
    trigger2count = 0;
    Serial.println("TRIGGER 2 HỦY KÍCH HOẠT");
  }
  
  if (trigger1count >= 6) { // Cho phép 0.5s để biên độ vượt qua ngưỡng cao
    trigger1 = false;
    trigger1count = 0;
    Serial.println("TRIGGER 1 HỦY KÍCH HOẠT");
  }
}
void mpu_read() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // Bắt đầu với thanh ghi 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);  // Yêu cầu tổng cộng 14 thanh ghi
  
  AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}
