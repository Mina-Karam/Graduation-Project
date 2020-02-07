/**************************************************************
 *
 * TinyGSM Getting Started guide:
 *   https://tiny.cc/tinygsm-readme
 *
 * NOTE:
 * Some of the functions may be unavailable for your modem.
 * Just comment them out.
 *
 **************************************************************/

// Select your modem:
// #define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM868
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_SIM7000
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_SARAR4
// #define TINY_GSM_MODEM_M95
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_MC60
// #define TINY_GSM_MODEM_MC60E
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE
// #define TINY_GSM_MODEM_SEQUANS_MONARCH

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 9600

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3); // RX, TX

/*
 * Test enabled
 */
#define TINY_GSM_TEST_GPRS true
#define TINY_GSM_TEST_WIFI false
#define TINY_GSM_TEST_CALL true
#define TINY_GSM_TEST_SMS true
#define TINY_GSM_TEST_USSD true
#define TINY_GSM_TEST_BATTERY true
// powerdown modem after tests

// set GSM PIN, if any
#define GSM_PIN "0000"

// Set phone numbers, if you want to test SMS and Calls
//#define SMS_TARGET  "+380xxxxxxxxx"
//#define CALL_TARGET "+380xxxxxxxxx"

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "";
const char gprsUser[] = "";
const char gprsPass[] = "";
const char wifiSSID[]  = "";
const char wifiPass[] = "";

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

void setup() {
  // Set console baud rate
  SerialAT.begin(9600);
  SerialMon.begin(9600);
  delay(10);

  DBG("Wait...");

  
}

void loop() {

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  DBG("Initializing modem...");
  
  String name = modem.getModemName();
  DBG("Modem Name:", name);

  String modemInfo = modem.getModemInfo();
  DBG("Modem Info:", modemInfo);

#if TINY_GSM_TEST_GPRS
  // Unlock your SIM card with a PIN if needed
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
  }
#endif

#if TINY_GSM_TEST_GPRS && defined TINY_GSM_MODEM_XBEE
  // The XBee must run the gprsConnect function BEFORE waiting for network!
  modem.gprsConnect(apn, gprsUser, gprsPass);
#endif

  DBG("Waiting for network...");
  if (!modem.waitForNetwork()) {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected()) {
    DBG("Network connected");
  }

#if TINY_GSM_TEST_GPRS

  bool res = modem.isGprsConnected();
  DBG("GPRS status:", res ? "connected" : "not connected");

  String ccid = modem.getSimCCID();
  DBG("CCID:", ccid);

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  String cop = modem.getOperator();
  DBG("Operator:", cop);

  IPAddress local = modem.localIP();
  DBG("Local IP:", local);

  int csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);


#endif

  // Do nothing forevermore
  while (true) {
    modem.maintain();
  }
}
