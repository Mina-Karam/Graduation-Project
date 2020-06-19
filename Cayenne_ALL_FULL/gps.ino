//void gps() {
//   //************** Get GPS data *******************
//   if (sim808.getGPS()) {
//
//    Serial.print("latitude :");
//    Serial.println(sim808.GPSdata.lat,7);
//    
//    Serial.print("longitude :");
//    Serial.println(sim808.GPSdata.lon,7);
//
//    Cayenne.luxWrite(15, 800);
//
//   float  x = sim808.GPSdata.lat;
//   float  y = sim808.GPSdata.lon;
//   float z=200.0;
//    
//    char buffer[100];
//    buffer[0] = '[';
//    size_t offset = 1;
//    dtostrf(x, 1, 7, &buffer[offset]);
//    offset += strlen(&buffer[offset]);
//    buffer[offset++] = ',';
//    dtostrf(y, 4, 7, &buffer[offset]);
//    offset += strlen(&buffer[offset]);
//    buffer[offset++] = ',';
//    dtostrf(z, 4, 1, &buffer[offset]);
//    offset += strlen(&buffer[offset]);
//    buffer[offset++] = ']';
//    buffer[offset] = 0;
//
//    Cayenne.virtualWrite(20, buffer, "gps", "m");
//  }
//
//}
