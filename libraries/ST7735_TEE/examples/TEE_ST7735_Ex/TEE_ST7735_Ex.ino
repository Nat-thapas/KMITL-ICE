#include "ST7735_TEE.h"

TEE_ST7735 lcd(0,13,12,14,2); //ESP8266 csk,sda,A0,rst,cs
//TEE_ST7735 lcd(8,9,10,11,12); //Arduino  csk,sda,A0,rst,cs

void setup()
{
 lcd.init(lcd.HORIZONTAL);
 //lcd.init(lcd.VERTICAL);
 lcd.fillScreen(BLACK);
 delay(500);
 lcd.fillScreen(GREEN);
 delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
 testlines(GREEN);
 delay(1000);
 lcd.fillScreen(BLUE);
 delay(500);
 lcd.Printstr(10,20,(char*)"สวัสดี Hello World",WHITE);
 lcd.Printstr(0,50,(char*)"ทดสอบด้วย ESP8266",YELLOW);
 delay(1000);
}

void testlines(uint16_t color) {
   lcd.fillScreen(BLACK);
   for (uint16_t x=0; x < lcd.width; x+=6) {
     lcd.drawLine(0, 0, x, lcd.height-1, color);
   }
   for (uint16_t y=0; y < lcd.height; y+=6) {
     lcd.drawLine(0, 0, lcd.width-1, y, color);
   }
}

