#ifndef MPU6050_FALL_DETECT_H
#define MPU6050_FALL_DETECT_H

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDR 0x68

class MPU6050FallDetect {
public:
    MPU6050FallDetect();
    void begin();
    void run();
    bool isFalling() const { return fallDetected; }
    
private:
    int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
    float ax, ay, az, gx, gy, gz;
    boolean fall;
    boolean fallDetected;
    boolean trigger1, trigger2, trigger3;
    byte trigger1count, trigger2count, trigger3count;
    int angleChange;
    int resetCounter;
    unsigned long previousMillis;

    void mpu_read();
    void processFallDetection();
    void printSensorData();
};

#endif