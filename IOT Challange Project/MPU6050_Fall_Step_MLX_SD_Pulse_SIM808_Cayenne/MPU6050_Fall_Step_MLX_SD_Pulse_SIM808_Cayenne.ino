/******** Global library ***********/
#include <millisDelay.h>        
#include <Wire.h> 
#include "SPI.h"

void setup() 
{
  Serial.begin(9600);
  
  Cayenne_Setup();
  SIM808_Setup();
  GPS_Setup();
  MLX_Setup();
  MPU_Setup();
  SD_Setup();
  Pulse_Setup();
  
}

void loop()
{
  SIM808_Loop();
  MPU_Loop();
  MLX_Loop();
  SD_Loop();
  Pulse_Loop();
  
  delay(100);
}
