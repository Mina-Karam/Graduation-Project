#include <LWiFi.h>
#include <LWiFiClient.h>
#define CAYENNE_DEBUG         // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneLWiFiWpa.h>
#include <LGPS.h>
#include "utils.h"

char token[] = "2fm04460o2";
char ssid[] = "MOVLAU";
char password[] = "l4ur3nc3.w1";

// Custom Widget
#define CAYENNE_PRINT Serial
#define VIRTUAL_PIN 1
#define LED_DIGITAL_PIN 12

gpsSentenceInfoStruct info;
char buff[256];

//----------------------------------------------------------------------
//!\brief  return position of the comma number 'num' in the char array 'str'
//!\return  char
//----------------------------------------------------------------------
static unsigned char getComma(unsigned char num,const char *str){
  unsigned char i,j = 0;
  int len=strlen(str);
  for(i = 0;i < len;i ++){
    if(str[i] == ',')
      j++;
    if(j == num)
      return i + 1; 
    }
  return 0; 
}

//----------------------------------------------------------------------
//!\brief convert char buffer to float
//!\return  float
//----------------------------------------------------------------------
static float getFloatNumber(const char *s){
  char buf[10];
  unsigned char i;
  float rev;

  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atof(buf);
  return rev; 
}

//----------------------------------------------------------------------
//!\brief convert char buffer to int
//!\return  float
//----------------------------------------------------------------------
static float getIntNumber(const char *s){
  char buf[10];
  unsigned char i;
  float rev;

  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atoi(buf);
  return rev; 
}
void parseGPGGA(const char* GPGGAstr){
  /* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
   * Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
   * Where:
   *  GGA          Global Positioning System Fix Data
   *  123519       Fix taken at 12:35:19 UTC
   *  4807.038,N   Latitude 48 deg 07.038' N
   *  01131.000,E  Longitude 11 deg 31.000' E
   *  1            Fix quality: 0 = invalid
   *                            1 = GPS fix (SPS)
   *                            2 = DGPS fix
   *                            3 = PPS fix
   *                            4 = Real Time Kinematic
   *                            5 = Float RTK
   *                            6 = estimated (dead reckoning) (2.3 feature)
   *                            7 = Manual input mode
   *                            8 = Simulation mode
   *  08           Number of satellites being tracked
   *  0.9          Horizontal dilution of position
   *  545.4,M      Altitude, Meters, above mean sea level
   *  46.9,M       Height of geoid (mean sea level) above WGS84
   *                   ellipsoid
   *  (empty field) time in seconds since last DGPS update
   *  (empty field) DGPS station ID number
   *  *47          the checksum data, always begins with *
   */

  if(GPGGAstr[0] == '$'){
    int tmp;
    tmp = getComma(1, GPGGAstr);
    MyGPSPos.hour     = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
    MyGPSPos.minute   = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
    MyGPSPos.second    = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');

    //get time
    sprintf(buff, "UTC time %02d:%02d:%02d", MyGPSPos.hour, MyGPSPos.minute, MyGPSPos.second);
    Serial.print(buff);
    //get lat/lon coordinates
    float latitudetmp;
    float longitudetmp;
    tmp = getComma(2, GPGGAstr);
    latitudetmp = getFloatNumber(&GPGGAstr[tmp]);
    tmp = getComma(4, GPGGAstr);
    longitudetmp = getFloatNumber(&GPGGAstr[tmp]);
    // need to convert format
    convertCoords(latitudetmp, longitudetmp, MyGPSPos.latitude, MyGPSPos.longitude);
    //get lat/lon direction
    tmp = getComma(3, GPGGAstr);
    MyGPSPos.latitude_dir = (GPGGAstr[tmp]);
    tmp = getComma(5, GPGGAstr);
    MyGPSPos.longitude_dir = (GPGGAstr[tmp]);
    
    //sprintf(buff, "latitude = %10.4f-%c, longitude = %10.4f-%c", MyGPSPos.latitude, MyGPSPos.latitude_dir, MyGPSPos.longitude, MyGPSPos.longitude_dir);
    //Serial.println(buff); 
    
    //get GPS fix quality
    tmp = getComma(6, GPGGAstr);
    MyGPSPos.fix = getIntNumber(&GPGGAstr[tmp]);    
    sprintf(buff, "  -  GPS fix quality = %d", MyGPSPos.fix);
    Serial.print(buff);   
    //get satellites in view
    tmp = getComma(7, GPGGAstr);
    MyGPSPos.num = getIntNumber(&GPGGAstr[tmp]);    
    sprintf(buff, "  -  %d satellites", MyGPSPos.num);
    Serial.println(buff); 
  }
  else{
    Serial.println("No GPS data"); 
  }
}

//----------------------------------------------------------------------
//!\brief  Convert GPGGA coordinates (degrees-mins-secs) to true decimal-degrees
//!\return  -
//----------------------------------------------------------------------
void convertCoords(float latitude, float longitude, float &lat_return, float &lon_return){
  int lat_deg_int = int(latitude/100);    //extract the first 2 chars to get the latitudinal degrees
  int lon_deg_int = int(longitude/100);   //extract first 3 chars to get the longitudinal degrees
    // must now take remainder/60
    // this is to convert from degrees-mins-secs to decimal degrees
    // so the coordinates are "google mappable"
    float latitude_float = latitude - lat_deg_int * 100;    //remove the degrees part of the coordinates - so we are left with only minutes-seconds part of the coordinates
    float longitude_float = longitude - lon_deg_int * 100;     
    lat_return = lat_deg_int + latitude_float / 60 ;      //add back on the degrees part, so it is decimal degrees
    lon_return = lon_deg_int + longitude_float / 60 ;
}

//----------------------------------------------------------------------
//!\brief  Grab GPS position from serial
//!\return  -
//----------------------------------------------------------------------
void GetGPSPos(void){
  // For one second we parse GPS data and report some key values   
    Serial.println("--- LGPS loop ---"); 
    LGPS.getData(&info);
    //Serial.print((char*)info.GPGGA); 
    parseGPGGA((const char*)info.GPGGA);
        
    //check fix 
    //if GPS fix is OK
    if ( MyGPSPos.fix == GPS || MyGPSPos.fix == DGPS || MyGPSPos.fix == PPS ){
      //set a flag
      MyFlag.fix3D = true;
    }
    else{
      //reset flag 
      MyFlag.fix3D = false;
    }
    
    sprintf(buff, "Current position is : https://www.google.com/maps?q=%2.6f%c,%3.6f%c", MyGPSPos.latitude, MyGPSPos.latitude_dir, MyGPSPos.longitude, MyGPSPos.longitude_dir);
    Serial.println(buff);
    Serial.println();
    int latdir;int londir;
    char ltd = MyGPSPos.latitude_dir;
    char lnd = MyGPSPos.longitude_dir;
    if( ltd == "N"[0] ){ latdir = 1;}else{ latdir=0;}
    if( lnd == "E"[0] ){ londir = 1;}else{ londir=0;}
    Cayenne.virtualWrite(2, MyGPSPos.latitude);
    Cayenne.virtualWrite(3, MyGPSPos.longitude);    
    Cayenne.virtualWrite(4, latdir); 
    Cayenne.virtualWrite(5, londir);
  
}
#define VIRTUAL_PIN 1
CAYENNE_IN(VIRTUAL_PIN){
  // get value sent from dashboard
  int currentValue = getValue.asInt(); // 0 to 1
  // assuming you wire your relay as normally open
  if (currentValue == 0) { digitalWrite(LED_DIGITAL_PIN, HIGH);} else { digitalWrite(LED_DIGITAL_PIN, LOW);}
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Cayenne.begin(token, ssid, password); 
  delay(2000);
  LGPS.powerOn();
  Serial.println("GPS Powered on.");
  MyGPSPos.fix = Error;
  //delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  GetGPSPos();
  Cayenne.run();
  delay(1000);
}