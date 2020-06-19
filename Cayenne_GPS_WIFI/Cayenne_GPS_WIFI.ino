#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

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

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  //mySerial.begin(9600);
  //Serial.begin(9600);

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
  
}

void loop() {
   //************** Get GPS data *******************
   if (sim808.getGPS()) {

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
    dtostrf(x, 4, 7, &buffer[offset]);
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
