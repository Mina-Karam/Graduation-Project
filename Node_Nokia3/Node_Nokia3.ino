#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>



Adafruit_PCD8544 display = Adafruit_PCD8544(D0, D1, D2);


void setup()

{

  Serial.begin(9600);

  display.begin();

  display.setContrast(50);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.println("Sistem pencocokan Fingerprint");
  display.display();

  Serial.println("Mina");


}

void loop()
{ 
    Serial.println("2");
    delay(500);
  display.setCursor(40, 0);
  display.println("Sistem pencocokan Fingerprint");
  display.display();

    Serial.println("6");
}
