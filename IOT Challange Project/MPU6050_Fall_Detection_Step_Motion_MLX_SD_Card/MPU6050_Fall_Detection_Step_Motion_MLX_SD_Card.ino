/******** Global library ***********/
#include <millisDelay.h>        
#include <Wire.h> 

void setup() 
{
  Serial.begin(9600);

  MLX_Setup();
  MPU_Setup();
}

void loop()
{
  MPU_Loop();
  MLX_Loop();
  
  delay(100);
}
