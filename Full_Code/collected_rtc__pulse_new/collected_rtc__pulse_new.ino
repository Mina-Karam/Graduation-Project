#include<millisDelay.h>
#include <SoftwareSerial.h>
#include <Wire.h>              
#include <SPI.h>               
#include <Adafruit_GFX.h>      
#include <Adafruit_PCD8544.h>  
SoftwareSerial pulse(2,3);
 Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
 millisDelay heartDelay;

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
heartDelay.start(5);
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


 
HEARTBEAT();
if(heartDelay.justFinished()){
  Serial.print(BPM);
 Serial.print("omega6");
  }




  

  

RTCLCD();

}
