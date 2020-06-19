/*
This example shows how to connect to Cayenne using a GSM device and send/receive sample data.
The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.
Steps:
1. Install the TinyGSM library (https://github.com/vshymanskyy/TinyGSM) from the Arduino Library Manager.
2. Set the Cayenne authentication info to match the authentication info from the Dashboard.
3. Uncomment the correct GSM modem type and set the GSM connection info, if needed.
4. Compile and upload the sketch.
5. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space

// Uncomment your modem type:
// #define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE

#include <CayenneMQTTGSM.h>

// This sketch uses a software serial connection.
#include <SoftwareSerial.h>
//SoftwareSerial gsmSerial(2, 3); // RX, TX
#define gsmSerial Serial3

// If you are using a device that supports a hardware serial (Mega, Leonardo, etc.) and prefer to use
// that you can comment out the above lines and uncomment the one below.
//#define gsmSerial Serial1

// GSM connection info.
char apn[] = ""; // Access point name. Leave empty if it is not needed.
char gprsLogin[] = ""; // GPRS username. Leave empty if it is not needed.
char gprsPassword[] = ""; // GPRS password. Leave empty if it is not needed.
char pin[] = ""; // SIM pin number. Leave empty if it is not needed.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "17d3cf30-3be8-11ea-b73d-1be39589c6b2";

float x = -16.7;
float y = -64.6;
float z = 200.0;

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
  Cayenne.begin(username, password, clientID, gsmSerial, apn, gprsLogin, gprsPassword, pin);
}

void loop() {
  
    Cayenne.luxWrite(2, 500);
    char buffer[32];
    buffer[0] = '[';
    size_t offset = 1;
    dtostrf(x, 1, 6, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(y, 1, 6, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ',';
    dtostrf(z, 1, 1, &buffer[offset]);
    offset += strlen(&buffer[offset]);
    buffer[offset++] = ']';
    buffer[offset] = 0;

  Cayenne.virtualWrite(20, buffer, "gps", "m");
}
