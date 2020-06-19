/******** Global library ***********/
#include <millisDelay.h>        
#include <Wire.h> 
#include "SPI.h"

void setup() 
{
  Serial.begin(9600);

  MLX_Setup();
  MPU_Setup();
  SD_Setup();
  
}

void loop()
{
  MPU_Loop();
  MLX_Loop();
  SD_Loop();
  
  delay(100);
}
