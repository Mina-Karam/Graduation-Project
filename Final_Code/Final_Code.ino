/******** PULSE SENSOR**********/
#include <Ticker.h>

/********* TIMER ************/
#include <millisDelay.h>

/******** MLX  ******/ 
#include <Wire.h>
#include <Adafruit_MLX90614.h>

/*********LCD NOKIA *******/
#include <SPI.h>               // include SPI library
#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 (Nokia 5110) library

/******** SIM808 *********/
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

/******** CAYENNE ESP2866 *******/
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

#define TINY_GSM_MODEM_SIM808
#include <CayenneMQTTGSM.h>
    
// WiFi network info.
char ssid[] = "Areka co space2";
char wifiPassword[] = "2areka2020";

// GSM connection info.
char apn[] = ""; // Access point name. Leave empty if it is not needed.
char gprsLogin[] = ""; // GPRS username. Leave empty if it is not needed.
char gprsPassword[] = ""; // GPRS password. Leave empty if it is not needed.
char pin[] = ""; // SIM pin number. Leave empty if it is not needed.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "4db0b0d0-49cd-11ea-84bb-8f71124cfdfb";

/******** SIM808 *********/
SoftwareSerial mySerial(D7, D8); // RX, TX
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR

/******** MLX  ******/ 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
millisDelay mlxDelay;

/*********LCD NOKIA *******/
// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D6, D5, D0);

/******** PULSE SENSOR**********/
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

/************** SIM808 *********/

void setup() {
    Serial.begin(9600); // Show in Serial 
    
    mySerial.begin(9600); // Sim808 in Serial RX,TX 
    
    delay(10);
    
    Cayenne_GSM.begin(username, password, clientID, mySerial, apn, gprsLogin, gprsPassword, pin);
//    WiFi.begin(ssid, wifiPassword);
//    if(WiFi.status() != WL_CONNECTED)
//    {
//      delay (500);
//      Serial.print (".");
//    }
//   else
//   {
//    /************* GPRS ************/    
//
//   }
//    if(WiFi.status() == WL_CONNECTED)
//    {
//    /************* WIFI ************/  
//    Cayenne_wifi.begin(username, password, clientID, ssid, wifiPassword);// CAYENNE CONNECTION WITH WIFI 
//    }
   

    

    
    mlx.begin(); //MLX CONFEGRATION WITH WIRE TO USE SCL, SDA 
    mlxDelay.start(1000); //MLX TIMER USED

    flipper.attach_ms(2, Test); // PULSE SENSOR FUNCTION
    sender.attach(2, senderfunc); // PULSE SENSOR FUNCTION 

    
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
void loop() {


    if(WiFi.status() == WL_CONNECTED)
    {
    /************* WIFI ************/  
      Cayenne_wifi.luxWrite(10, 800);
    }
    
    if(WiFi.status() != WL_CONNECTED){
    /************* GPRS ************/  
      Cayenne_GSM.luxWrite(15, 800);
    }
    delay(100);
    sim808.getGPS();  
    char buffer[100];
    
  if (sim808.getGPS()) {

    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat,9);
    
    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon,9);


   float  x = sim808.GPSdata.lat;
   float  y = sim808.GPSdata.lon;
   float  z = 20.0;

    
    buffer[0] = '[';
    size_t offset = 1;
    dtostrf(x, 1, 9, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(y, 1, 9, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(z, 1, 1, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ']';
    buffer[offset] = 0;
  
  } 
  
  if(WiFi.status() == WL_CONNECTED)
    {
    /************* WIFI ************/ 
      Cayenne_wifi.virtualWrite(11, buffer, "gps", "m");
    }
    
    if(WiFi.status() != WL_CONNECTED){
    /************* GPRS ************/  
      Cayenne_GSM.virtualWrite(16, buffer, "gps", "m");
    }
  Time();
  MLX();
  pulse_sensor();
}
