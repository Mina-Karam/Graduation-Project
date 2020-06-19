/******** PULSE SENSOR**********/
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

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

/******** CAYENNE SIM808 *******/
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

#define TINY_GSM_MODEM_SIM808
#include <CayenneMQTTGSM.h>

// GSM connection info.
char apn[] = ""; // Access point name. Leave empty if it is not needed.
char gprsLogin[] = ""; // GPRS username. Leave empty if it is not needed.
char gprsPassword[] = ""; // GPRS password. Leave empty if it is not needed.
char pin[] = ""; // SIM pin number. Leave empty if it is not needed.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "7a2d2c30-abe6-11ea-b767-3f1a8f1211ba";

/******** SIM808 *********/
DFRobot_SIM808 sim808(&Serial3);//Connect RX,TX,PWR

float latitude_erorr = 0.05891585413;
float longitude_erorr = 0.1298821647;

float latitude_old = 0.0;
float longitude_old = 0.0;

float latitude_new = 0.0;
float longitude_new = 0.0;

/******** MLX  ******/
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
millisDelay mlxDelay;

/*********LCD NOKIA *******/
// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);

/******** PULSE SENSOR**********/
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".

/************** SIM808 *********/

void setup() {
  Serial.begin(9600); // Show in Serial

  Serial3.begin(9600); // Sim808 in Serial RX,TX

  delay(10);

  Cayenne.begin(username, password, clientID, Serial3, apn, gprsLogin, gprsPassword, pin);

  mlx.begin(); //MLX CONFEGRATION WITH WIRE TO USE SCL, SDA
  mlxDelay.start(10000); //MLX TIMER USED

  //******** Initialize sim808 module *************
  while (!sim808.init()) {
    delay(500);
    Serial.print("Sim808 init error\r\n");
  }

  //************* Turn on the GPS power************
  if ( sim808.attachGPS())
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

  // initialize the pulse sensor

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }
}
void loop() {
  if (sim808.getGPS()) {
    Get_GPS();
    Get_Time();
  }
  if (mlxDelay.justFinished()) {
    Get_Temp();

    if (( mlx.readObjectTempC() < 30 ) || (mlx.readObjectTempC() > 38))
    {
      Serial.println("error");
    }
  }
  if (pulseSensor.sawStartOfBeat()) {
      Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
      Serial.print("BPM: ");                        // Print phrase "BPM: " 
      Serial.println(myBPM);                        // Print the value inside of myBPM. 
      Cayenne.virtualWrite(2, myBPM);
      }
}
