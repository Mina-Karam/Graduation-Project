
#include <millisDelay.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>


#define PIN_RX    2
#define PIN_TX    3

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
millisDelay mlxDelay;
SoftwareSerial pulse(4,5);
SoftwareSerial mygps(PIN_RX,PIN_TX);
DFRobot_SIM808 sim808(&mygps);//Connect RX,TX,PWR,


/* Heartbeat program */
//volatile used to say this value will change alot so compiler donot confuse and everytime you need this value take it from the memory cause it chanaging donot take form any whereelse 
//dol variables ely ana 7tahmam yazbto el shogel

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

void setup() {
 Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test");  

   mlx.begin();
  mlxDelay.start(1000);
pinMode(blinkPin, OUTPUT);
  pinMode(pulsePin, INPUT);    //Heart Rate sensor             // we agree to talk fast!  //set baudrate
  pulse.begin(9600);
    //***************************//
  mygps.begin(9600);
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
  
}
//******************************************//


void loop() {
  heart();
  temp();
  
if (sim808.getGPS()) {
    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat,6);

    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon,6);
  }
}
