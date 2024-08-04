#include "oled.h"
void display_name()
{
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
  display.clearDisplay();
  display.setCursor(0,10);
  display.setTextSize(2);
  display.print("Nguyen Vu Hieu");
  display.display();
}