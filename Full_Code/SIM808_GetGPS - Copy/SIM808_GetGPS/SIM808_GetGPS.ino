#include <Adafruit_GFX.h>      
#include <Adafruit_PCD8544.h> 
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <millisDelay.h>

#define PIN_RX    8
#define PIN_TX    9

millisDelay mlxDelay;

SoftwareSerial pulse(2,3);
SoftwareSerial mygps(PIN_RX,PIN_TX);
DFRobot_SIM808 sim808(&mygps);//Connect RX,TX,PWR,

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

volatile int rate[10];                    // array to hold last ten IBI values (INTER-BEATS INTERVAL)
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find IBI (unsigned long 32bit from 0 to 4294967295)
volatile int P =512;                      // used to find peak in pulse wave, seeded(el mo3dal el tabe3a bta3to ely btal3 mn el sensor we howa lesa ma2karsh 7aga )
volatile int T = 512;                     // used to find trough in pulse wave, seeded (b3d el threeshold byanzel taeye ll keyma el 3adaya bata3to  )
volatile int thresh = 520;                // used to find instant moment of heart beat, seeded (peak single elkamea el3ozma eley hayosala )(threeshold)
volatile int amp = 100;                   // used to hold amplitude of pulse waveform(limit leh), seeded
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM (boolean=true or false)
volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM
int var = 0;
unsigned long milli0;     //(value arduino give to us its know globaly...actually the number of milliseconds since the program started untill now it change after each reading is new start )  
unsigned long milli1;  //(value arduino give to us its know globaly......this one the initial start time)  

int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0 (A0)
int blinkPin = 13;                // pin to blink led at each beat(led in arduino blink every time we get a value)
int ms = 2;    //for BPM

// Volatile Variables, used in the interrupt service routine!
//dol el 7agat ely hattqury men el sensor
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS (el signal el hat2raha heya BPM kol 2 mes  el mowasat ll 10 karyat)
volatile int Signal;                // holds the incoming raw data (int hold analog signal data straightt from sensor updated every 2 ms)
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! (....int to hold time interval between beats,2ms resoultion)
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
//pulse is (boolean that is true when a heartbeat is sensed then false in time with pin13 goning out) 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.(boolean that is true whenever pulse is found and bpm is update .user must reset)

//char Time[] = "  :  :  ";

void setup() {
	Serial.begin(9600);
  pulse.begin(9600);
	mygps.begin(9600);
  mlx.begin();
  mlxDelay.start(1000);

  
  //mySerial.begin(9600);
  //Serial.begin(9600);

// initialize the display
  display.begin();
 
 
  display.setContrast(50);
 
  display.clearDisplay();   
  display.display();
 
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(3, 0);
  //display.println("omega 6 clock");
  //display.print("for elzahimer");
  display.display();
  
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
      //heartDelay.start(2000);

  
}

void loop() {
  
    HEARTBEAT();
    temp();

   //************** Get GPS data *******************
   if (sim808.getGPS()) {
    
    Serial.print(sim808.GPSdata.year);
    Serial.print("/");
    Serial.print(sim808.GPSdata.month);
    Serial.print("/");
    Serial.print(sim808.GPSdata.day);
    Serial.print(" ");
    Serial.println();
    
    //draw_text(12, 12, sim808.GPSdata.year);
    //draw_text(20, 17, "/");
    //draw_text(12, 18, sim808.GPSdata.month);
    //draw_text(20, 20, "/");
    //draw_text(12, 21, sim808.GPSdata.day);
    
    Serial.print(sim808.GPSdata.hour);
    Serial.print(":");
    Serial.print(sim808.GPSdata.minute);
    Serial.print(":");
    Serial.print(sim808.GPSdata.second);
    Serial.print(":");
    Serial.println();

    //draw_text(18, 12, sim808.GPSdata.hour);
    
    display.setCursor(2, 24);
    display.print(sim808.GPSdata.hour);
    display.display();
    draw_text(12, 24, "/");
    display.setCursor(24, 24);
    display.print(sim808.GPSdata.minute);
    display.display();
    draw_text(36, 24, "/");
    display.setCursor(48, 24);
    display.print(sim808.GPSdata.second);
    display.display();
    display.setCursor(30, 50);
    display.print(BPM);
    display.display();

    //Time[6] = "24";
    //Time[3] = "60";
    //Time[0] = "60";

    //draw_text(18, 40, Time);   // print time
    
    //************* Turn off the GPS power ************
   // sim808.detachGPS();
  }

}
void draw_text(byte x_pos, byte y_pos, char *text)
{
  display.setCursor(x_pos, y_pos);
  display.print(text);
  display.display();
}
