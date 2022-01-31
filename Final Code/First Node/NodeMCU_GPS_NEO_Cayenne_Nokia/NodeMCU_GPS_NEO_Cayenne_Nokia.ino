#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h>

#include <SPI.h>               // include SPI library
//#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 (Nokia 5110) library

static const int RXPin = 14, TXPin = 12; // 12 to Rx - 14 to Tx
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(4, 0, 2, 13, 15);
int gps_time = 0;
    
void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Cayenne_Setup();
  Nokia_Setup();

}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
      
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  //Nokia_Loop();
  
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    
    /********* Upload To Cayenne ********/
    char buffer[32];
    buffer[0] = '[';
    size_t offset = 1;
    dtostrf(gps.location.lat(), 1, 6, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(gps.location.lng(), 1, 6, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(200, 1, 1, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ']';
    buffer[offset] = 0;
    
    Cayenne.virtualWrite(0, buffer, "gps", "m");
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
    
    //draw_text(10, 10, "HELLO DATE");
    display.setCursor(15, 18);
    display.print(gps.date.month());
    display.print("/");
    display.print(gps.date.day());
    display.print("/");    
    display.print(gps.date.year());
    display.display();
  }

  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  //draw_text(10, 20, "HELLO Time");
  display.setCursor(15, 35);
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 0)
    {
      Serial.print(F("0"));
    }
    else
    {
      Serial.print(gps.time.hour());
      display.print(gps.time.hour());
    }
    
    Serial.print(F(":"));
    display.print(":");
    
    if (gps.time.minute() < 0) 
    {
      Serial.print(F("0"));
    }
    else
    {
      Serial.print(gps.time.minute());
      display.print(gps.time.minute());
    }
    Serial.print(F(":"));
    display.print(":");
    
    if (gps.time.second() < 0)
    {
      Serial.print(F("0"));
    }
    else
    {
      Serial.print(gps.time.second());
      display.print(gps.time.second());
    }

    
    gps_time = gps.time.second();
  }
  else
  {
    Serial.print(F("INVALID"));
  }
    Serial.println();
}
