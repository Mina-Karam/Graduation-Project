void MLX() {
if(mlxDelay.justFinished()){
    Serial.print("Ambient = "); Serial.println(mlx.readAmbientTempC()); 
    if( ( mlx.readObjectTempC() < 30 )|| (mlx.readObjectTempC() > 38))
    {
      Serial.println("error");  
    }
  mlxDelay.repeat();
  } 
}
