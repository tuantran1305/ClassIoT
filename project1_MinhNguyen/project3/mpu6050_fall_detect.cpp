#include "mpu6050_fall_detect.h"

MPU6050FallDetect::MPU6050FallDetect() : 
    fall(false), fallDetected(false), 
    trigger1(false), trigger2(false), trigger3(false),
    trigger1count(0), trigger2count(0), trigger3count(0),
    angleChange(0), resetCounter(0), previousMillis(0) {}

void MPU6050FallDetect::begin() {
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);

    Serial.println("MPU6050 initialized");
}

void MPU6050FallDetect::run() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 100) {
        previousMillis = currentMillis;

        mpu_read();
        
        // Convert raw values to meaningful units
        ax = (AcX-2050)/16384.00;
        ay = (AcY-77)/16384.00;
        az = (AcZ-1947)/16384.00;
        gx = (GyX+270)/131.07;
        gy = (GyY-351)/131.07;
        gz = (GyZ+136)/131.07;

        processFallDetection();
        printSensorData();
        
        // Reset fallDetected state after 5 seconds (50 loops)
        if (fallDetected) {
            resetCounter++;
            if (resetCounter >= 50) {
                fallDetected = false;
                resetCounter = 0;
            }
        }
    }
}

void MPU6050FallDetect::mpu_read() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);  // request a total of 14 registers
    AcX = Wire.read()<<8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY = Wire.read()<<8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read()<<8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read()<<8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read()<<8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read()<<8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read()<<8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void MPU6050FallDetect::processFallDetection() {
    // calculating Amplitude vector for 3 axis
    float raw_amplitude = pow(pow(ax,2)+pow(ay,2)+pow(az,2),0.5);
    int amplitude = raw_amplitude * 10;  // Multiplied by 10 because values are between 0 to 1
    
    if (amplitude <= 2 && trigger2 == false) { //if AM breaks lower threshold (0.4g)
        trigger1 = true;
    }
    if (trigger1 == true) {
        trigger1count++;
        if (amplitude >= 12) { //if AM breaks upper threshold (3g)
            trigger2 = true;
            trigger1 = false; trigger1count = 0;
        }
    }
    if (trigger2 == true) {
        trigger2count++;
        angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5); 
        if (angleChange >= 30 && angleChange <= 400) { //if orientation changes by between 80-100 degrees
            trigger3 = true; trigger2 = false; trigger2count = 0;
        }
    }
    if (trigger3 == true) {
        trigger3count++;
        if (trigger3count >= 10) { 
            angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5);
            if ((angleChange >= 0) && (angleChange <= 10)) { //if orientation changes remains between 0-10 degrees
                fall = true; trigger3 = false; trigger3count = 0;
            }
            else { //user regained normal orientation
                trigger3 = false; trigger3count = 0;
            }
        }
    }
    if (fall == true) { //in event of a fall detection
        fallDetected = true;
        fall = false;
    }
    if (trigger2count >= 6) { //allow 0.5s for orientation change
        trigger2 = false; trigger2count = 0;
    }
    if (trigger1count >= 6) { //allow 0.5s for AM to break upper threshold
        trigger1 = false; trigger1count = 0;
    }
}

void MPU6050FallDetect::printSensorData() {
    Serial.print("Accelerometer (g): ");
    Serial.print("X = "); Serial.print(ax);
    Serial.print(" | Y = "); Serial.print(ay);
    Serial.print(" | Z = "); Serial.println(az);
    
    Serial.print("Gyroscope (degrees/sec): ");
    Serial.print("X = "); Serial.print(gx);
    Serial.print(" | Y = "); Serial.print(gy);
    Serial.print(" | Z = "); Serial.println(gz);
    
    Serial.print("Falling state: ");
    Serial.println(fallDetected ? "TRUE" : "FALSE");
    
    Serial.println("--------------------");
}