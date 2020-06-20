void Get_GPS(){
  /********* Show in serial ***********/

  Serial.print("latitude :");
  latitude_new = (sim808.GPSdata.lat+latitude_erorr+latitude_old)/2;
  Serial.println(latitude_new,6);
  latitude_old = sim808.GPSdata.lat+latitude_erorr;
  
  Serial.print("longitude :");
  longitude_new = (sim808.GPSdata.lon+longitude_erorr+longitude_old)/2;
  Serial.println(longitude_new,6);
  longitude_old = sim808.GPSdata.lon+longitude_erorr;
  
  /********* Upload To Cayenne ********/
  char buffer[32];
  buffer[0] = '[';
  size_t offset = 1;
  dtostrf(latitude_new, 1, 9, &buffer[offset]);
  offset += strlen(&buffer[offset]);
  buffer[offset++] = ',';
  dtostrf(longitude_new, 1, 9, &buffer[offset]);
  offset += strlen(&buffer[offset]);
  buffer[offset++] = ',';
  dtostrf(200, 1, 1, &buffer[offset]);
  offset += strlen(&buffer[offset]);
  buffer[offset++] = ']';
  buffer[offset] = 0;

  Cayenne.virtualWrite(0, buffer, "gps", "m");

}

void Get_Time(){
    Serial.print(sim808.GPSdata.year);
    Serial.print("/");
    Serial.print(sim808.GPSdata.month);
    Serial.print("/");
    Serial.print(sim808.GPSdata.day);
    Serial.print(" ");
    Serial.print(sim808.GPSdata.hour);
    Serial.print(":");
    Serial.print(sim808.GPSdata.minute);
    Serial.print(":");
    Serial.print(sim808.GPSdata.second);
}
