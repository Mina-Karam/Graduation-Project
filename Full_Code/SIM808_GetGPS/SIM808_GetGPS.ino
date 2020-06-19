#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define GSMSerial Serial3

DFRobot_SIM808 sim808(&Serial3);//Connect RX,TX,PWR,

//DFRobot_SIM808 sim808(&Serial);

void setup() {
	Serial.begin(9600);
	Serial3.begin(9600);
  //mySerial.begin(9600);
  //Serial.begin(9600);

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
    Serial.println(sim808.GPSdata.lat,6);
    
    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon,6);

    //************* Turn off the GPS power ************
   // sim808.detachGPS();
  }

}
