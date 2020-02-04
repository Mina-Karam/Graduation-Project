#include <Wire.h>
//#include <Adafruit_MLX90614.h>
//Adafruit_MLX90614 mlx = Adafruit_MLX90614();
//*************** cayenne *********************//
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h>

#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define PIN_RX    D7
#define PIN_TX    D8

// WiFi network info.
char ssid[] = "Areka co space2";
char wifiPassword[] = "2areka2020";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "17d3cf30-3be8-11ea-b73d-1be39589c6b2";
  
unsigned long lastMillis = 0;
//*************** SIM808 **********************//

SoftwareSerial mygps(PIN_RX,PIN_TX);
DFRobot_SIM808 sim808(&mygps);//Connect RX,TX,PWR
//***********************************///

void setup() {
 Serial.begin(9600); /* begin serial for debug */
 Cayenne.begin(username, password, clientID, ssid, wifiPassword);

 //Wire.begin(D2, D1); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
 //mlx.begin();
 
 ///********************///
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

void loop() {
 //Wire.beginTransmission(8); /* begin with device address 8 */
 //Wire.write("Hello Arduino");  /* sends hello string */
 //Wire.endTransmission();    /* stop transmitting */
 
//*********From Arduino *********//
 //Wire.requestFrom(8, 13); /* request & read data of size 13 from slave */
 //while(Wire.available()){
    //char c = Wire.read();
  //Serial.print(c);
 //}
 //Serial.println();
 //delay(1000);

 //********From MLX *********//
  Serial.print("Ambient = "); 
  //Serial.println(mlx.readAmbientTempC());

  //************* get GPS *************//
  if (sim808.getGPS()) {
    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat,6);

    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon,6);
  }
//*************** cayenne *********************//
Cayenne.loop();

  /// CHANNEL 0   
  //Cayenne.virtualWrite(0, "5555");
  /// CHANNEL 1  
  Cayenne.virtualWrite(1, sim808.GPSdata.lat );
  /// CHANNEL 2  
  Cayenne.virtualWrite(2, sim808.GPSdata.lon );
  delay(500);
}
