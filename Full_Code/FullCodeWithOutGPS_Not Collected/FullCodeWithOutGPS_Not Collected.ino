#include <SoftwareSerial.h>
#include <Wire.h>              
#include <SPI.h>               
#include <Adafruit_GFX.h>      
#include <Adafruit_PCD8544.h>  

SoftwareSerial pulse(2,3);
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

volatile int rate[10];                    
volatile unsigned long sampleCounter = 0;          
volatile unsigned long lastBeatTime = 0;           
volatile int P =512;                      
volatile int T = 512;                     
volatile int thresh = 520;                
volatile int amp = 100;                   
volatile boolean firstBeat = true;        
volatile boolean secondBeat = false;      
int var = 0;
unsigned long milli0;     
unsigned long milli1;   

int pulsePin = 0;                 
int blinkPin = 13;                
int ms = 2;    //for BPM


volatile int BPM;                   
volatile int Signal;                
volatile int IBI = 600;             
volatile boolean Pulse = false;    
 
volatile boolean QS = false;    
 
#define button1   9   
#define button2   8

char Time[] = "  :  :  ";
char Date[] = "  /  /20  ";
unsigned short i, second, minute, hour, w_day, day, month, year;
long time1,time2;
void setup() {  

  pinMode(blinkPin, OUTPUT);
  pinMode(pulsePin, INPUT);    
  Serial.begin(9600);             
  pulse.begin(9600);


  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
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

/**************************************************/
/******************* Heart_Beat *******************/
/**************************************************/

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
      {
    Serial.print("check person");
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

void findMax(void)
{
  Signal = analogRead(pulsePin);              // read the Pulse Sensor 
  sampleCounter += ms;                    // keep track of the time in mS with this variable 
  //(law kraa mn pin kda yba2a 3ada 2msec store them in sample counter to determine pulse counting)
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise 
  //(totalbeat counter--time between two beats to avoid noise ...so we get only required pulse)

    //  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3)
  {       // avoid dichrotic noise by waiting 3/5 of last IBI (t de ta7t fa 3asan kda momken td7ol 3al el noise)
    if (Signal < T)
    {                        // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > thresh && Signal > P)
  {          // thresh condition helps avoid noise
  P = Signal;                             // P is the peak
  }                                        // keep track of highest point in pulse wave
  //(kda 7asab el kama wl ka3)

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
    Pulse = true;                               // set the Pulse flag when we think there is a pulse
    digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
    IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS(total pulse-last one)
    lastBeatTime = sampleCounter;               // keep track of time for next pulse 

    if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
      secondBeat = false;                  // clear secondBeat flag
      for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
      rate[i] = IBI;    //(haedel yakod pulse we y3malaha save we yemsa7a mn el flag 3ashan yakod wa7ada tany l7d ma ybko 10)                  
      }
    }
  
    if(firstBeat){                         // if it's the --first time-- we found a beat, if firstBeat == TRUE
      firstBeat = false;                   // clear firstBeat flag
      secondBeat = true;                   // set the second beat flag
      return;                              // IBI value is unreliable so discard it
      //(bya3mal kda 3ashan awal pulsa btege leh mmoken tkon roung awa edek mahzoza fa byakod el taya 3ashan el deka tebka a3ala0)
    }   


    // keep a running total of the last 10 IBI values
    word runningTotal = 0;                  // clear the runningTotal variable (word 16 bit unsigned int ya3ney any rakm)   

    for(int i=0; i<=8; i++)
    {                // shift data in the rate array
      rate[i] = rate[i+1];                  // and drop the oldest IBI value 
      runningTotal += rate[i];              // add up the 9 oldest IBI values
    }

    rate[9] = IBI;                          // add the latest IBI to the rate array
    runningTotal += rate[9];                // add the latest IBI to runningTotal
    runningTotal /= 10;                     // average the last 10 IBI values 
    BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
    QS = true;                              // set Quantified Self flag 
    // QS FLAG IS NOT CLEARED INSIDE THIS ISR (isr interput to sent reading immediately)
    //(kda hasab drabat el alab )
    }                       
  }

if (Signal < thresh && Pulse == true)
{   // when the values are going down, the beat is over
    digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
                                              T = thresh;
        //da ely hayo2fo lma ya2a we kalas hawo2f msec
                                                    
  }

if (N > 2500)
{                           // if 2.5 seconds go by --without-- a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
    //(da ele hay02f0 lwa m7adsh 7atet sbo3a fa mefesh eraya)
  }
}

/**************************************************/
/******************* RTC_LCD **********************/
/**************************************************/


  bool debounce ()
{
  byte count = 0;
  for(byte i = 0; i < 5; i++)
  {
    if ( !digitalRead(button1) )
      count++;
    delay(10);
  }
 
  if(count > 2)  return 1;
  else           return 0;
}
 
// function for display day of the week
void day_display()
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
 
void RTC_display()
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
 
void blink_parameter()
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
 
// main loop
void RTCLCD()
{
  if(!digitalRead(button1))     // if button B1 is pressed
    if(debounce())   // call debounce function (make sure if B1 is pressed)
    {
      i = 0;
      while(debounce());  // call debounce function (wait for button B1 to be released)
      while(1)
      {
        while( !digitalRead(button2) )    // while button B2 pressed
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
        if( !digitalRead(button1) )  // if button B1 is pressed
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
