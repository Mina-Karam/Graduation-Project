#include <Adafruit_MLX90614.h>  

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

millisDelay mlxDelay;

void MLX_Setup(void){
  mlx.begin();
  mlxDelay.start(1000);
}

void MLX_Loop(void){
  if(mlxDelay.justFinished()){
  Serial.print("Ambient = "); Serial.println(mlx.readAmbientTempC()); 
  Cayenne.virtualWrite(4, mlx.readAmbientTempC());

  if( ( mlx.readObjectTempC()<35 )|| (mlx.readObjectTempC()> 38))
  {
//   Serial.println("error");  
  }
  mlxDelay.repeat();
} 
}
