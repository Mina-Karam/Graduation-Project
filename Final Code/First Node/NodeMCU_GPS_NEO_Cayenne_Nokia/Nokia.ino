
void Nokia_Setup(void){
  // initialize the display
  display.begin();
 
  display.setContrast(50);
 
  display.clearDisplay();   
  display.display();
 
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(3, 0);
  display.println("Time & Date");
  display.display();
}

void Nokia_Loop(void){
   
}

void draw_text(byte x_pos, byte y_pos, char *text)
{
  display.setCursor(x_pos, y_pos);
  display.print(text);
  display.display();
}
