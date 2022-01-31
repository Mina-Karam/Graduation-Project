Adafruit_MLX90614 mlx = Adafruit_MLX90614();
millisDelay mlxDelay;

void MLX_Setup()
{
  mlx.begin();
  mlxDelay.start(1000);
}
void MLX_Loop(void) 
{
  if(mlxDelay.justFinished()){
    Serial.print("Ambient = "); Serial.println(mlx.readAmbientTempC()); 
    
    mlxDelay.repeat();
  } 
Cayenne.virtualWrite(1, mlx.readAmbientTempC());
}
