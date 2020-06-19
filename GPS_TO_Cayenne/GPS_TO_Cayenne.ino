
// cayenne debug
#define CAYENNE_PRINT Serial
#define CAYENNE_DEBUG

// gsm debug
#define TINY_GSM_DEBUG Serial

#define TINY_GSM_MODEM_SIM808
#include <CayenneMQTTGSM.h>

#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define PIN_RX    D7
#define PIN_TX    D8

SoftwareSerial mygps(PIN_RX,PIN_TX);
DFRobot_SIM808 sim808(&mygps);//Connect RX,TX,PWR,

// GSM connection info.
const char apn[] = ""; // Access point name. Leave empty if it is not needed.
const char gprsLogin[] = ""; // GPRS username. Leave empty if it is not needed.
const char gprsPassword[] = ""; // GPRS password. Leave empty if it is not needed.
const char pin[] = ""; // SIM pin number. Leave empty if it is not needed.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "4db0b0d0-49cd-11ea-84bb-8f71124cfdfb";


float x = -16.7;
float y = -64.6;
float z = 200.0;

//float latitude_erorr = 0.058909;
//float longitude_erorr = 0.129906;


void setup() {
    // put your setup code here, to run once:
  Serial.begin(9600);
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
  
  Cayenne_GSM.begin(username, password, clientID, mygps, apn, gprsLogin, gprsPassword, pin);
}

void loop() {
  if (sim808.getGPS()) {

    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat+0.058909,4);

    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon+0.129906,4);
    
    
    x = sim808.GPSdata.lat+0.058909;
    y = sim808.GPSdata.lon+0.129906;
    
    char buffer[32];  
    buffer[0] = '[';
    size_t offset = 1;
    dtostrf(x, 1, 4, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(y, 1, 4, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(z, 1, 1, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ']';
    buffer[offset] = 0;

  Cayenne_GSM.virtualWrite(20, buffer, "gps", "m");
  }
}
