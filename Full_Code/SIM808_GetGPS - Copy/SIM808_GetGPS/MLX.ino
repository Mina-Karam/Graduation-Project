void temp() {
if(mlxDelay.justFinished()){
  Serial.print("Ambient = "); Serial.println(mlx.readAmbientTempC()); 
  //Serial.print("C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("C");
   if( ( mlx.readObjectTempC()<35 )|| (mlx.readObjectTempC()> 38))
  {
   Serial.println("error");  
   //Serial.println();al.print(mlx.readObjectTempF()); Serial.println("F");
  //Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  // Serial.print("F\tObject = "); Seri
  //delay(500);
 }
  mlxDelay.repeat();
} 
}
