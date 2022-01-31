#include <Ticker.h>
#include <millisDelay.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h>


void setup() {
  Serial.begin(9600);
  Cayenne_Setup();
  MLX_Setup();
  pulse_Setup();
  Motion_Setup();
} 



void loop() {  
  MLX_Loop();
  pulse_Loop();
  Motion_Loop();
  Cayenne_Loop();
}
