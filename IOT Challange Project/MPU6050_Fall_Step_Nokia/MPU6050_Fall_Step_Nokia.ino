#include <Wire.h>
#include <SPI.h>

void setup() 
{
  Serial.begin(9600);
  
  MPU_Setup();
}





void loop()
{
  MPU_Loop();
  delay(50);
}
