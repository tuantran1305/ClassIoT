#include "max30100_sensor.h"
#include <Adafruit_SSD1306.h>
#include "mpu6050_fall_detect.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DISPLAY_WIDTH (128)
#define DISPLAY_HEIGHT (64)
#define OLED_RESET (-1)
#define SCREEN_ADDRESS (0x3C)

max30100_sensor_t g_heart_rate_sensor;
mpu6050_fall_detect_t g_fall_detector;

Adafruit_SSD1306 g_display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);

void on_beat_detected(void)
{
    Serial.println("Beat!");
}

void update_display(void)
{
    g_display.clearDisplay();
    g_display.setTextSize(1);
    g_display.setTextColor(WHITE);
    
    // Display heart rate
    g_display.setCursor(0, 0);
    g_display.printf("Heart rate: %d", max30100_sensor_get_heart_rate(&g_heart_rate_sensor));
    
    // Display SpO2
    g_display.setCursor(0, 12);
    g_display.printf("SpO2: %d", max30100_sensor_get_spo2(&g_heart_rate_sensor));
    
    // Display falling state
    g_display.setCursor(0, 24);
    g_display.printf("Falling state: %s", mpu6050_fall_detect_is_falling(&g_fall_detector) ? "TRUE" : "FALSE");
    
    g_display.display();
}

void heart_rate_task(void *parameter)
{
    for(;;)
    {
        max30100_sensor_update(&g_heart_rate_sensor);
        vTaskDelay(10 / portTICK_PERIOD_MS);  // 10ms delay
    }
}

void fall_detection_task(void *parameter)
{
    for(;;)
    {
        mpu6050_fall_detect_run(&g_fall_detector);
        vTaskDelay(100 / portTICK_PERIOD_MS);  // 100ms delay
    }
}

void display_update_task(void *parameter)
{
    for(;;)
    {
        update_display();
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1 second delay
    }
}

void setup(void)
{
    Serial.begin(115200);
    mpu6050_fall_detect_init(&g_fall_detector);
    mpu6050_fall_detect_begin(&g_fall_detector);
    
    max30100_sensor_init(&g_heart_rate_sensor);
    max30100_sensor_begin(&g_heart_rate_sensor);
    max30100_sensor_set_on_beat_detected_callback(&g_heart_rate_sensor, on_beat_detected);

    if(!g_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    g_display.clearDisplay();

    xTaskCreate(
        heart_rate_task,        /* Task function. */
        "HeartRateTask",        /* String with name of task. */
        10000,                  /* Stack size in bytes. */
        NULL,                   /* Parameter passed as input of the task */
        1,                      /* Priority of the task. */
        NULL);                  /* Task handle. */

    xTaskCreate(
        fall_detection_task,
        "FallDetectionTask",
        10000,
        NULL,
        1,
        NULL);

    xTaskCreate(
        display_update_task,
        "DisplayUpdateTask",
        10000,
        NULL,
        1,
        NULL);
}

void loop(void)
{
    // Empty. Tasks are handled by FreeRTOS
}