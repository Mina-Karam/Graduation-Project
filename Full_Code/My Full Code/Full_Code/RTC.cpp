/*
 * RTC.cpp
 *
 *  Created on: Dec 8, 2019
 *      Author: MinaK
 */
#include <arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "RTC.h"

#define button1   9
#define button2   8

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

char Time[] = "  :  :  ";
char Date[] = "  /  /20  ";
unsigned short i, second, minute, hour, w_day, day, month, year;
//long time1,time2;

bool debounce (void)
  {
    byte count = 0;
    for(byte i = 0; i < 5; i++)
      {
        if (!digitalRead(button1))
          count++;
        delay(10);
      }

    if(count > 2)  return 1;
    else           return 0;
  }


// function for display day of the week
void day_display(void)
{
  switch(w_day)
  {
    case 1:  draw_text(12, 16, "  SUNDAY  "); break;
    case 2:  draw_text(12, 16, "  MONDAY  "); break;
    case 3:  draw_text(15, 16, " TUESDAY ");  break;
    case 4:  draw_text(15, 16, "WEDNESDAY");  break;
    case 5:  draw_text(12, 16, " THURSDAY "); break;
    case 6:  draw_text(12, 16, "  FRIDAY  "); break;
    default : draw_text(12, 16, " SATURDAY ");
  }
}

void RTC_display(void)
{
  // convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour   >> 4) * 10 + (hour   & 0x0F);
  day    = (day    >> 4) * 10 + (day    & 0x0F);
  month  = (month  >> 4) * 10 + (month  & 0x0F);
  year   = (year   >> 4) * 10 + (year   & 0x0F);
  // end conversion

  // update time array
  Time[7] = second % 10 + '0';
  Time[6] = second / 10 + '0';
  Time[4] = minute % 10 + '0';
  Time[3] = minute / 10 + '0';
  Time[1] = hour   % 10 + '0';
  Time[0] = hour   / 10 + '0';

  // update date array
  Date[9] = year   % 10 + '0';
  Date[8] = year   / 10 + '0';
  Date[4] = month  % 10 + '0';
  Date[3] = month  / 10 + '0';
  Date[1] = day    % 10 + '0';
  Date[0] = day    / 10 + '0';

  draw_text(12,  24, Date);  // print date
  draw_text(18, 40, Time);   // print time
}

void blink_parameter(void)
{
  byte j = 0;
  while(j < 100 && digitalRead(button1) && digitalRead(button2))
  {
    j++;
    delay(5);
  }
}

byte edit(byte x_pos, byte y_pos, byte parameter)
{
  char text[3];
  sprintf(text,"%02u", parameter);
  while(debounce());  // call debounce function (wait for B1 to be released)

  while(1)
  {
    while(!digitalRead(button2))
    {
      parameter++;
      if(i == 0 && parameter > 31)   // if date > 31 ==> date = 1
        parameter = 1;
      if(i == 1 && parameter > 12)   // if month > 12 ==> month = 1
        parameter = 1;
      if(i == 2 && parameter > 99)   // if year > 99 ==> year = 0
        parameter = 0;
      if(i == 3 && parameter > 23)   // if hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 4 && parameter > 59)   // if minutes > 59 ==> minutes = 0
        parameter = 0;
      if(i == 5 && parameter >60)
      parameter =0 ;
      sprintf(text,"%02u", parameter);
      draw_text(x_pos, y_pos, text);

      delay(200);   // wait 200ms
    }

    draw_text(x_pos, y_pos, "  ");
    blink_parameter();

    draw_text(x_pos, y_pos, text);
    blink_parameter();

    if(!digitalRead(button1)) // if button B1 is pressed
    if(debounce())            // call debounce function (make sure if B1 is pressed)
    {
      i++;                // increment 'i' for the next parameter
      return parameter;   // return parameter value and exit
    }
  }
}

// print text on the LCD
void draw_text(byte x_pos, byte y_pos, char *text)
{
  display.setCursor(x_pos, y_pos);
  display.print(text);
  display.display();
}
