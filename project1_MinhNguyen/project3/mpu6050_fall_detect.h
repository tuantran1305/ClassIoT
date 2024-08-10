#ifndef MPU6050_FALL_DETECT_H
#define MPU6050_FALL_DETECT_H

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDR (0x68)

struct mpu6050_fall_detect_struct
{
    int16_t ac_x, ac_y, ac_z, tmp, gy_x, gy_y, gy_z;
    float ax, ay, az, gx, gy, gz;
    bool fall;
    bool fall_detected;
    bool trigger1, trigger2, trigger3;
    uint8_t trigger1_count, trigger2_count, trigger3_count;
    int angle_change;
    int reset_counter;
    uint32_t previous_millis;
};

typedef struct mpu6050_fall_detect_struct mpu6050_fall_detect_t;

void mpu6050_fall_detect_init(mpu6050_fall_detect_t *detector);
void mpu6050_fall_detect_begin(mpu6050_fall_detect_t *detector);
void mpu6050_fall_detect_run(mpu6050_fall_detect_t *detector);
bool mpu6050_fall_detect_is_falling(const mpu6050_fall_detect_t *detector);

#endif // MPU6050_FALL_DETECT_H