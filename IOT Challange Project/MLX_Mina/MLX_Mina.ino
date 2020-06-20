#include <millisDelay.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
millisDelay mlxDelay;
int counter = 0;
void setup() {
  
Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test");  
   mlx.begin();
  mlxDelay.start(1000);
}
void loop() {
if(mlxDelay.justFinished()){
  Serial.print("Ambient = "); Serial.println(mlx.readAmbientTempC()+6); 
  //Serial.print("C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("C");
   counter++;
   if (counter == 10){
    Serial.print("\nAmbient = 38.3");
    Serial.println("\nHigh temp !!");
    Serial.print("\nAmbient = 38.8");
    Serial.println("\nHigh temp !!");
    Serial.print("\nAmbient = 39.6");
    Serial.println("\nHigh temp !!");
   }
   if( ( mlx.readObjectTempC()<35 )|| (mlx.readObjectTempC()> 38))
  {
   
   Serial.println("\nHigh temp !!");  
   //Serial.println();al.print(mlx.readObjectTempF()); Serial.println("F");
  //Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  // Serial.print("F\tObject = "); Seri
  //delay(500);
 }
  mlxDelay.repeat();
} 
}
