#include "oled.h"
#include "freeRTOS.h"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
void setup() {
  Serial.begin(9600);
 if (!display.begin(SSD1306_SWITCHCAPVCC, 0X3C)) {
    Serial.println(" Khong ket noi duoc voi oled ");
    while(1);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  xTaskCreatePinnedToCore(display_name, "Hien thi ten ", 2000, NULL, 1, &Task1_p, 1);
}
void display_name(void *pvParameters)
{
  while(1)
  {
    display_name();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
}
