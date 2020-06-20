#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 (Nokia 5110) library

// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);

void Nokia_Setup(void){
  // initialize the LCD NOKIA
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(3, 0);
  display.println("Omega 6 Clock");
  display.print("For Elzahimer");
  display.display();
}

void Nokia_Loop(void){
  
}
