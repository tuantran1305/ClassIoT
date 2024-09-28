// #include "lcdspi.h"  // Create an instance of Arduino_GFX
unsigned long Millis = 0;
unsigned long previous_data = 0;
void setupGFX() {
  gfx->begin();
  gfx->fillScreen(BACKGROUND);
#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif  // Initialize serial communication
  gfx->setTextColor(WHITE);  // Set text color to white
  gfx->setTextSize(3);       // Set text size
  gfx->setRotation(1);       // Set rotation if needed
}

void displayStaticText() {
  gfx->setCursor(0, 10/ 2); // Center vertically
  gfx->println("Vu Hieu");  // Display the text
}