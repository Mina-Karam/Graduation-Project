#include <SoftwareSerial.h>
#include <Wire.h>              
#include <SPI.h>               
#include <Adafruit_GFX.h>      
#include <Adafruit_PCD8544.h>  
SoftwareSerial pulse(16,17);
Adafruit_PCD8544 display = Adafruit_PCD8544(12, 11, 10, 9, 8);

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

int pulsePin = 25;                 
int blinkPin = 24;                
int ms = 2;    //for BPM


volatile int BPM;                   
volatile int Signal;                
volatile int IBI = 600;             
volatile boolean Pulse = false;    
 
volatile boolean QS = false;     
#define button1   23   
#define button2   22

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
{ time1=millis();
HEARTBEAT();
}
else if (millis()- time2>=5)
{RTCLCD();
time2=millis();
  
  
  }



}
