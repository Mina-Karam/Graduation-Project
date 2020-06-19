/*
 * collected_rtc__pulse.cpp
 *
 *  Created on: Dec 4, 2019
 *      Author: MinaK
 */
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "RTC.h"
#include "Heart_Beat.h"




void RTCLCD(void);
void HEARTBEAT(void);

SoftwareSerial pulse(2,3); //Heart_Beat
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);



/********************************************/
/**************** Heart_Beat*****************/
/********************************************/

int ms = 2;    //for BPM
unsigned long milli0;
unsigned long milli1;
volatile int BPM;
volatile int Signal;
volatile int IBI = 600;
volatile bool Pulse = false;
volatile bool QS = false;

/**************************************/
/**************** RTC *****************/
/**************************************/

unsigned short i, second, minute, hour, w_day, day, month, year;
long time1,time2;

void setup() {

  pinMode(13, OUTPUT);
  pinMode(0, INPUT);
  Serial.begin(9600);
  pulse.begin(9600);


  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  delay(1000);   // wait 1 second
  Wire.begin();  // join I2C bus

  // initialize the display
  display.begin();


  display.setContrast(50);

  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(3, 0);
  display.println("omega 6 clock");
  display.print("for elzahimer");
  display.setCursor(28, 32);
  display.print("TIME:");
  display.display();

time1=millis();
time2=millis();

}

void loop() {




if (millis() - time1>=100)
  {

  time1=millis();
  HEARTBEAT();

  }
else if (millis()- time2>=5)
  {

  RTCLCD();
  time2=millis();

  }
}

// main loop of RTCLCD
void RTCLCD(void)
{
  if(!digitalRead(9))     // if button B1 is pressed
    if(debounce())   // call debounce function (make sure if B1 is pressed)
    {
      i = 0;
      while(debounce());  // call debounce function (wait for button B1 to be released)
      while(1)
      {
        while( !digitalRead(8) )    // while button B2 pressed
        {
          w_day++;         // increment w_day
          if(w_day > 7)
            w_day = 1;
          day_display();   // call day_display function
          delay(500);   // wait 500 ms
        }
        draw_text(15, 16, "         ");
        blink_parameter();   // call blink_parameter function

        day_display();      // call day_display function
        blink_parameter();  // call blink_parameter function
        if( !digitalRead(9) )  // if button B1 is pressed
          break;
      }

    day    = edit(12, 24, day);    // edit date
    month  = edit(30, 24, month);  // edit month
    year   = edit(60, 24, year);   // edit year
    hour   = edit(18, 40, hour);   // edit hours
    minute = edit(36, 40, minute); // edit minutes

    // convert decimal to BCD
    minute = ((minute / 10) << 4) + (minute % 10);
    hour   = ((hour   / 10) << 4) + (hour   % 10);
    day    = ((day    / 10) << 4) + (day    % 10);
    month  = ((month  / 10) << 4) + (month  % 10);
    year   = ((year   / 10) << 4) + (year   % 10);
    // end conversion

    while(debounce());  // call debounce function (wait for button B1 to be released)

    // write data to DS1307 RTC
    Wire.beginTransmission(0x68);  // start I2C protocol with DS1307 address
    Wire.write(0);       // send register address
    Wire.write(0);       // reset sesonds and start oscillator
    Wire.write(minute);  // write minute
    Wire.write(hour);    // write hour
    Wire.write(w_day);   // write day
    Wire.write(day);     // write date
    Wire.write(month);   // write month
    Wire.write(year);    // write year
    Wire.endTransmission();  // stop transmission and release the I2C bus

    delay(200); // wait 200ms
  }

  Wire.beginTransmission(0x68); // start I2C protocol with DS1307 address
  Wire.write(0);                // send register address
  Wire.endTransmission(false);  // I2C restart
  Wire.requestFrom(0x68, 7);    // request 7 bytes from DS1307 and release I2C bus at end of reading
  second = Wire.read();         // read seconds from register 0
  minute = Wire.read();         // read minuts from register 1
  hour   = Wire.read();         // read hour from register 2
  w_day  = Wire.read();         // read day from register 3
  day    = Wire.read();         // read date from register 4
  month  = Wire.read();         // read month from register 5
  year   = Wire.read();         // read year from register 6

  day_display();    // print day of the week
  RTC_display();    // print time & date

  delay(100);   // wait 100 ms
}

// main loop of HEARTBEAT
void HEARTBEAT(void)
{
  milli0 = millis();  //pewaent time in millisec
  //millis()( Returns the number of milliseconds passed since the Arduino board began running the current program   ...... give number of secound from start program to now)
  if(milli0 - milli1 >= ms)   //(required period passed)
  {
    milli1 = milli0;
   //(to save the current start time very important)
   //(if the condition which pass 2 msec is then find max)
    findMax();
   //(qs incidcate if there is a signal exists then go to serial)
    if(QS == true)
    {
      Serial.print(F("BPM: "));
      Serial.print(BPM);
      Serial.print(F("\n"));
         QS = false;

      if(BPM<60||BPM>150)
      {Serial.print("check person");
        Serial.print(BPM);
      Serial.print(F("\n"));

      //( to return it to initial state)
      }
    }
  }
  if(pulse.available()>0)
  {
    pulse.write(BPM);
  }
}
