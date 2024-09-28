#include "lcdspi.h"
#include "freeRTOS.h"
void setup() {
  Serial.begin(9600);
  setupGFX();  // Setup the GFX display
  // xTaskCreatePinnedToCore(display_lcd, "Hien du lieu ", 4000, NULL, 1, &Task1_p, 1);
  displayStaticText();
}
// void display_lcd(void *parameter) {
//   while (1) {
//     if (millis() - previous_data > 1000) {
//       displayScrollingText();  // Scroll text with a speed of 50ms
//       previous_data = millis();
//     }
//   }
//   vTaskDelay(50 / portTICK_PERIOD_MS);
// }
void loop() {
}
