//************GSM*********//
#define TINY_GSM_MODEM_SIM808
#define INFO_CONNECTION "GSM"
#include <TinyGsmClient.h>

//************ WIFI ******//
#include <CayenneMQTTESP8266.h>

#ifndef WRITE_CHUNK_SIZE
#define WRITE_CHUNK_SIZE 0 // The chunk size to use when sending data, 0 means data will not be sent in chunks.
#endif // !WRITE_CHUNK_SIZE

WiFiClient _wifiClient
void Connect_Cayenne(const char* username, const char* password, const char* clientID, const char* ssid, const char* wifiPassword,Stream& serial, const char* apn, const char* gprsLogin, const char* gprsPassword);

void Connect_Cayenne(const char* username, const char* password, const char* clientID, const char* ssid, const char* wifiPassword,Stream& serial, const char* apn, const char* gprsLogin, const char* gprsPassword)
{
  int status = WL_IDLE_STATUS;
  #if defined(ESP8266) || defined (ESP32)
    WiFi.mode(WIFI_STA);
    delay(500);
  #endif
  
  if (WiFi.status() == WL_NO_SHIELD) {
    CAYENNE_LOG("WiFi not available");
  }
  else{
  CAYENNE_LOG("Connecting to %s", ssid);
  if (wifiPassword && strlen(wifiPassword)) {
    WiFi.begin(ssid, wifiPassword);
  }
  else {
    WiFi.begin(ssid);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  CAYENNE_LOG("Connected to WiFi");
  
    IPAddress local_ip = WiFi.localIP();
    CAYENNE_LOG("IP: %d.%d.%d.%d", local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
    CayenneArduinoMQTTClient::begin(_wifiClient, username, password, clientID, WRITE_CHUNK_SIZE); 
  }
}
