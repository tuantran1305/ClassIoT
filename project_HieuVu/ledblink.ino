#include "led.h"
#include "freeRTOS.h"
void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  xTaskCreatePinnedToCore(blinkLed, "Task1", 4000, NULL, 1, &blinkLed_p, 1);
  xTaskCreatePinnedToCore(connect, "Task2", 4000, NULL, 1, &connect_p, 1);
  xTaskCreatePinnedToCore(connectThingsBoard, "Task3", 4000, NULL,1 , &connectThingsBoard_p, 1);
  disableCore1WDT();
}
void blinkLed(void* parameter) {
  while (1) {
    changeState();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
void connect(void* parameter)
{
  while(1)
  {
    connectWifi();
    vTaskDelay(50/ portTICK_PERIOD_MS);
  }
}
void connectThingsBoard(void* parameter)
{
  while(1)
  {
    connectThingsBoard(NULL);
    vTaskDelay(50/ portTICK_PERIOD_MS);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
}
