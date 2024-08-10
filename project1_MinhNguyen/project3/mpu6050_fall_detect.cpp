#include "mpu6050_fall_detect.h"

static void mpu6050_fall_detect_mpu_read(mpu6050_fall_detect_t *detector);
static void mpu6050_fall_detect_process_fall_detection(mpu6050_fall_detect_t *detector);
static void mpu6050_fall_detect_print_sensor_data(mpu6050_fall_detect_t *detector);

void mpu6050_fall_detect_init(mpu6050_fall_detect_t *detector)
{
    detector->fall = false;
    detector->fall_detected = false;
    detector->trigger1 = false;
    detector->trigger2 = false;
    detector->trigger3 = false;
    detector->trigger1_count = 0;
    detector->trigger2_count = 0;
    detector->trigger3_count = 0;
    detector->angle_change = 0;
    detector->reset_counter = 0;
    detector->previous_millis = 0;
}

void mpu6050_fall_detect_begin(mpu6050_fall_detect_t *detector)
{
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);

    Serial.println("MPU6050 initialized");
}

void mpu6050_fall_detect_run(mpu6050_fall_detect_t *detector)
{
    uint32_t current_millis = millis();
    if (current_millis - detector->previous_millis >= 100)
    {
        detector->previous_millis = current_millis;

        mpu6050_fall_detect_mpu_read(detector);
        
        // Convert raw values to meaningful units
        detector->ax = (detector->ac_x - 2050) / 16384.00;
        detector->ay = (detector->ac_y - 77) / 16384.00;
        detector->az = (detector->ac_z - 1947) / 16384.00;
        detector->gx = (detector->gy_x + 270) / 131.07;
        detector->gy = (detector->gy_y - 351) / 131.07;
        detector->gz = (detector->gy_z + 136) / 131.07;

        mpu6050_fall_detect_process_fall_detection(detector);
        mpu6050_fall_detect_print_sensor_data(detector);
        
        // Reset fall_detected state after 5 seconds (50 loops)
        if (detector->fall_detected)
        {
            detector->reset_counter++;
            if (detector->reset_counter >= 50)
            {
                detector->fall_detected = false;
                detector->reset_counter = 0;
            }
        }
    }
}

bool mpu6050_fall_detect_is_falling(const mpu6050_fall_detect_t *detector)
{
    return detector->fall_detected;
}

static void mpu6050_fall_detect_mpu_read(mpu6050_fall_detect_t *detector)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);  // request a total of 14 registers
    detector->ac_x = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    detector->ac_y = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    detector->ac_z = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    detector->tmp = Wire.read() << 8 | Wire.read();   // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    detector->gy_x = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    detector->gy_y = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    detector->gy_z = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

static void mpu6050_fall_detect_process_fall_detection(mpu6050_fall_detect_t *detector)
{
    // calculating Amplitude vector for 3 axis
    float raw_amplitude = pow(pow(detector->ax, 2) + pow(detector->ay, 2) + pow(detector->az, 2), 0.5);
    int amplitude = raw_amplitude * 10;  // Multiplied by 10 because values are between 0 to 1
    
    if (amplitude <= 2 && detector->trigger2 == false)
    { //if AM breaks lower threshold (0.4g)
        detector->trigger1 = true;
    }
    if (detector->trigger1 == true)
    {
        detector->trigger1_count++;
        if (amplitude >= 12)
        { //if AM breaks upper threshold (3g)
            detector->trigger2 = true;
            detector->trigger1 = false;
            detector->trigger1_count = 0;
        }
    }
    if (detector->trigger2 == true)
    {
        detector->trigger2_count++;
        detector->angle_change = pow(pow(detector->gx, 2) + pow(detector->gy, 2) + pow(detector->gz, 2), 0.5); 
        if (detector->angle_change >= 30 && detector->angle_change <= 400)
        { //if orientation changes by between 80-100 degrees
            detector->trigger3 = true;
            detector->trigger2 = false;
            detector->trigger2_count = 0;
        }
    }
    if (detector->trigger3 == true)
    {
        detector->trigger3_count++;
        if (detector->trigger3_count >= 10)
        { 
            detector->angle_change = pow(pow(detector->gx, 2) + pow(detector->gy, 2) + pow(detector->gz, 2), 0.5);
            if ((detector->angle_change >= 0) && (detector->angle_change <= 10))
            { //if orientation changes remains between 0-10 degrees
                detector->fall = true;
                detector->trigger3 = false;
                detector->trigger3_count = 0;
            }
            else
            { //user regained normal orientation
                detector->trigger3 = false;
                detector->trigger3_count = 0;
            }
        }
    }
    if (detector->fall == true)
    { //in event of a fall detection
        detector->fall_detected = true;
        detector->fall = false;
    }
    if (detector->trigger2_count >= 6)
    { //allow 0.5s for orientation change
        detector->trigger2 = false;
        detector->trigger2_count = 0;
    }
    if (detector->trigger1_count >= 6)
    { //allow 0.5s for AM to break upper threshold
        detector->trigger1 = false;
        detector->trigger1_count = 0;
    }
}

static void mpu6050_fall_detect_print_sensor_data(mpu6050_fall_detect_t *detector)
{
    Serial.print("Accelerometer (g): ");
    Serial.print("X = "); Serial.print(detector->ax);
    Serial.print(" | Y = "); Serial.print(detector->ay);
    Serial.print(" | Z = "); Serial.println(detector->az);
    
    Serial.print("Gyroscope (degrees/sec): ");
    Serial.print("X = "); Serial.print(detector->gx);
    Serial.print(" | Y = "); Serial.print(detector->gy);
    Serial.print(" | Z = "); Serial.println(detector->gz);
    
    Serial.print("Falling state: ");
    Serial.println(detector->fall_detected ? "TRUE" : "FALSE");
    
    Serial.println("--------------------");
}