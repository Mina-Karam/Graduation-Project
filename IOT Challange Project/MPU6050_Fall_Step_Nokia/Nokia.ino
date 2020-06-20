#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 (Nokia 5110) library

// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(39, 37, 35, 33, 31);

void Nokia_Setup(void){
  // initialize the display
  display.begin();
 
  display.setContrast(65);

  display.clearDisplay();   
  display.display();
 
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(15, 0);
  display.println("omega 6 ");
  display.display();
}

void Nokia_Loop(void){
      display.setCursor(3, 15);
      display.println("Level 1\n");
      display.print("Step: ");
      display.println(distance);
      display.display();
      
  if(distance >= 20){
      display.clearDisplay();
      display.setCursor(3, 15);  
      display.println("Up tp level 2"); 
      display.display();
      delay(1000);
      display.clearDisplay();
      
  }
      
}

void draw_text(byte x_pos, byte y_pos, char *text)
{
  display.setCursor(x_pos, y_pos);
  display.print(text);
  display.display();
}
