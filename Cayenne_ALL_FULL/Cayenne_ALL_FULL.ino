#include <Ticker.h>
#include <millisDelay.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#include <SPI.h>               // include SPI library
#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 (Nokia 5110) library

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>



#include <DFRobot_sim808.h>
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(D7, D8); // RX, TX

DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,

//DFRobot_SIM808 sim808(&Serial);
// WiFi network info.
char ssid[] = "Areka co space2";
char wifiPassword[] = "2areka2020";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "4db0b0d0-49cd-11ea-84bb-8f71124cfdfb";

//float x = -16.7;
//float y = -64.6;
//float z = 20.0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"eg.pool.ntp.org",7200,60000);
//other timeserver at https://www.ntppool.org

int ntc[7],i;
unsigned long int timestamp;

// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D6, D5, D0);



Adafruit_MLX90614 mlx = Adafruit_MLX90614();

millisDelay mlxDelay;
Ticker flipper;
Ticker sender;
const int maxAvgSample = 20;
volatile int rate[maxAvgSample];                    // used to hold last ten IBI values
boolean sendok = false;
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find the inter beat interval
volatile int P =512;                      // used to find peak in pulse wave
volatile int T = 512;                     // used to find trough in pulse wave
volatile int thresh = 512;                // used to find instant moment of heart beat
volatile int amp = 100;                   // used to hold amplitude of pulse waveform
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = true;       // used to seed rate array so we startup with reasonable BPM
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;  

void setup() {
  Serial.begin(9600);

mySerial.begin(9600);
  
  delay(10);
  mlx.begin();
  mlxDelay.start(1000);
  flipper.attach_ms(2, Test);
  sender.attach(2, senderfunc);


Cayenne_wifi.begin(username, password, clientID, ssid, wifiPassword);

  //******** Initialize sim808 module *************
  while(!sim808.init()) { 
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }

  //************* Turn on the GPS power************
  if( sim808.attachGPS())
      Serial.println("Open the GPS power success");
  else 
      Serial.println("Open the GPS power failure");

  WiFi.begin(ssid, password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    delay (500);
    Serial.print (".");
  }

  timeClient.begin();

  // initialize the display
  display.begin();
 
  display.setContrast(50);
 
  display.clearDisplay();   
  display.display();
 
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(3, 0);
  display.println("omega 6 clock");
  //display.print("for elzahimer");
  display.display();
}

void senderfunc()
 {
  sendok=true;
  }


void loop() {  

     if(sendok) 

          {
       delay(100);

    

              Serial.println(BPM); // Take beat here :)) BPM variable 
              delay(10);
              sendok =false;
              flipper.attach_ms(2, Test);     }
     MLX();
//time1();

if (sim808.getGPS()) {

    Serial.print(sim808.GPSdata.year);
    Serial.print("/");
    Serial.print(sim808.GPSdata.month);
    Serial.print("/");
    Serial.print(sim808.GPSdata.day);
    Serial.print(" ");
    Serial.print(sim808.GPSdata.hour);
    Serial.print(":");
    Serial.print(sim808.GPSdata.minute);
    Serial.print(":");
    Serial.print(sim808.GPSdata.second);

    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat,7);
    
    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon,7);

    Cayenne_wifi.luxWrite(15, 800);

   float  x = sim808.GPSdata.lat;
   float  y = sim808.GPSdata.lon;
   float z=200.0;
    
    char buffer[100];
    buffer[0] = '[';
    size_t offset = 1;
    dtostrf(x, 1, 7, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(y, 4, 7, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(z, 4, 1, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ']';
    buffer[offset] = 0;

    Cayenne_wifi.virtualWrite(20, buffer, "gps", "m");
  }
}

//void sendtcp()
//{
         
              
//}


int count = 0;
void Test()
{
  count++;
  if(count ==1000)
  {
    flipper.detach();
    count =0;
    //sendtcp();
    sendok=true;
    
  }

      Signal = analogRead(A0);              // read the Pulse Sensor 
//      Serial.print("Signal= ");
//      Serial.println(Signal);
        sampleCounter += 2;                         // keep track of the time in mS with this variable
    int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
 // Serial.print("N= ");
 // Serial.println(N);
      if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T){                        // T is the trough
            T = Signal;                         // keep track of lowest point in pulse wave 
         }
       }
      
    if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
        P = Signal;                             // P is the peak
       }                                        // keep track of highest point in pulse wave
    
  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
if (N > 250){                                   // avoid high frequency noise
  if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
    Pulse = true;                               // set the Pulse flag when we think there is a pulse
    //digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
    IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
    lastBeatTime = sampleCounter;               // keep track of time for next pulse
         
         if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
             firstBeat = false;                 // clear firstBeat flag
             return;                            // IBI value is unreliable so discard it
            }   
         if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
            secondBeat = false;                 // clear secondBeat flag
               for(int i=0; i<=maxAvgSample-1; i++){         // seed the running total to get a realisitic BPM at startup
                    rate[i] = IBI;                      
                    }
            }
          
    // keep a running total of the last 10 IBI values
    word runningTotal = 0;                   // clear the runningTotal variable    

    for(int i=0; i<=(maxAvgSample-2); i++){                // shift data in the rate array
          rate[i] = rate[i+1];              // and drop the oldest IBI value 
          runningTotal += rate[i];          // add up the 9 oldest IBI values
        }
        
    rate[maxAvgSample-1] = IBI;                          // add the latest IBI to the rate array
    runningTotal += rate[maxAvgSample-1];                // add the latest IBI to runningTotal
    runningTotal /= maxAvgSample;                     // average the last 10 IBI values 
    BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
    QS = true;                              // set Quantified Self flag 
    // QS FLAG IS NOT CLEARED INSIDE THIS ISR
//    Serial.print("runningTotal: ");
//    Serial.println(runningTotal);
    }                       
}

  if (Signal < thresh && Pulse == true){     // when the values are going down, the beat is over
      //digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
      Pulse = false;                         // reset the Pulse flag so we can do it again
      amp = P - T;                           // get amplitude of the pulse wave
      thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
      P = thresh;                            // reset these for next time
      T = thresh;
     }
  
  if (N > 2500){                             // if 2.5 seconds go by without a beat
      thresh = 512;                          // set thresh default
      P = 512;                               // set P default
      T = 512;                               // set T default
      lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
      firstBeat = true;                      // set these to avoid noise
      secondBeat = true;                     // when we get the heartbeat back
     }
  
  //sei();                                     // enable interrupts when youre done!
}// end isr
