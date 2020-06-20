/******** Global library ***********/
#include <millisDelay.h>        
#include <Wire.h> 
#include <MPU6050.h>            
#include <Adafruit_MLX90614.h>  

MPU6050 mpu;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

millisDelay mlxDelay;

static long distance = 0;
boolean freefallDetected = false;

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
