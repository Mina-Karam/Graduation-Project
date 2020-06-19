//void SIM808(){
//  if (sim808.getGPS()) {
//
//    Serial.print("latitude :");
//    Serial.println(sim808.GPSdata.lat,9);
//    
//    Serial.print("longitude :");
//    Serial.println(sim808.GPSdata.lon,9);
//
//
//   float  x = sim808.GPSdata.lat;
//   float  y = sim808.GPSdata.lon;
//   float  z = 20.0;
//    
//    buffer[0] = '[';
//    size_t offset = 1;
//    dtostrf(x, 1, 9, &buffer[offset]);
//    offset += strlen(&buffer[offset]);
//    buffer[offset++] = ',';
//    dtostrf(y, 1, 9, &buffer[offset]);
//    offset += strlen(&buffer[offset]);
//    buffer[offset++] = ',';
//    dtostrf(z, 1, 1, &buffer[offset]);
//    offset += strlen(&buffer[offset]);
//    buffer[offset++] = ']';
//    buffer[offset] = 0;
//
//  }
//}
