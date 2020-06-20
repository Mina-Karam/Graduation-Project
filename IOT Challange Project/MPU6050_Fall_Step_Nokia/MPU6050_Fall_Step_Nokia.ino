#include <Wire.h>
#include <SPI.h>

void setup() 
{
  Serial.begin(9600);
  Nokia_Setup();
  MPU_Setup();
}

void loop()
{
  MPU_Loop();
  Nokia_Loop();
  delay(50);
}
