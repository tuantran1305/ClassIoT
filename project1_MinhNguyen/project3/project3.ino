#include "max30100_sensor.h"
#include <Adafruit_SSD1306.h>
#include "mpu6050_fall_detect.h"

MAX30100Sensor heartRateSensor;
MPU6050FallDetect fallDetector;

// OLED object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_UPDATE_INTERVAL = 1000; // Update every 1 second

void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);
    fallDetector.begin();
    
    heartRateSensor.begin();
    heartRateSensor.setOnBeatDetectedCallback(onBeatDetected);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while(1);
    }
    display.clearDisplay();
}

void updateDisplay()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    // Display heart rate
    display.setCursor(0, 0);
    display.printf("Heart rate: %d", heartRateSensor.getHeartRate());
    
    // Display SpO2
    display.setCursor(0, 12);
    display.printf("SpO2: %d", heartRateSensor.getSpO2());
    
    // Display falling state
    display.setCursor(0, 24);
    display.printf("Falling state: %s", fallDetector.isFalling() ? "TRUE" : "FALSE");
    
    display.display();
}

void loop()
{
    heartRateSensor.update();
    fallDetector.run();

    // Update display every DISPLAY_UPDATE_INTERVAL
    if (millis() - lastDisplayUpdate > DISPLAY_UPDATE_INTERVAL) {
        updateDisplay();
        lastDisplayUpdate = millis();
    }
}