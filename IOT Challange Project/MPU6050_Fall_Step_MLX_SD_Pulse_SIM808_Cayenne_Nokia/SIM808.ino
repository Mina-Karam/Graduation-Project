#include <DFRobot_sim808.h>

DFRobot_SIM808 sim808(&Serial3);//Connect RX,TX,PWR

float latitude_erorr = 0.05891585413;
float longitude_erorr = 0.1298821647;

float latitude_old = 0.0;
float longitude_old = 0.0;

float latitude_new = 0.0;
float longitude_new = 0.0;

void SIM808_Setup(void){
  //******** Initialize sim808 module *************
  while (!sim808.init()) {
    delay(500);
    Serial.print("Sim808 init error\r\n");
  }
}
void GPS_Setup(void){

  //************* Turn on the GPS power************
  if ( sim808.attachGPS())
    Serial.println("Open the GPS power success");
  else
    Serial.println("Open the GPS power failure");
}

void SIM808_Loop(void){
  if (sim808.getGPS()) {
    Get_GPS();
    Get_Time();
  }
}

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
