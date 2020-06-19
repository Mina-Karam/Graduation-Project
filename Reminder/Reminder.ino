
// cayenne debug
#define CAYENNE_PRINT Serial
#define CAYENNE_DEBUG

#include <CayenneMQTTESP8266.h>

#include <SPI.h>               // include SPI library
#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 (Nokia 5110) library

// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D6, D5, D0);

// WiFi network info.
char ssid[] = "Areka co space2";
char wifiPassword[] = "2areka2020";


// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "4db0b0d0-49cd-11ea-84bb-8f71124cfdfb";



void setup() {
  Serial.begin(9600);  
  pinMode(D1,OUTPUT);
  digitalWrite(D1,LOW);

  Cayenne.begin(username, password, clientID, ssid, wifiPassword);

  // initialize the display
  display.begin();
 
  display.setContrast(50);
 
  display.clearDisplay();   
  display.display();
 
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(3, 0);
  display.println("omega 6 clock");
  display.print("for elzahimer");
  display.display();
}

void loop() {
 
       
      
        Cayenne.celsiusWrite(1, 30.0);
        Cayenne.luxWrite(2, 100);
        Cayenne.virtualWrite(7, 0, "digital_sensor", "d");
        
        Cayenne.loop();

    }
    

    // This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(8)
{
  // Read data from the sensor and send it to the virtual channel here.
  // For example, to send a digital value you can use the following:
  //int value = digitalRead(SENSOR_PIN);
  Cayenne.virtualWrite(8, 0, "digital_sensor", "d");
}

CAYENNE_IN(4)
{
  CAYENNE_LOG("Got a value: %d", getValue.asInt());
  digitalWrite(D1,HIGH);
  display.clearDisplay();   
  display.setCursor(15, 15);
  display.println("LED ON");
  display.display();
  delay(5000);
  digitalWrite(D1,LOW);
  display.clearDisplay();   
  display.setCursor(40, 40);
  display.println("LED OFF");
  display.display();
  
  
  // You can also use:
  // int i = getValue.asInt() or
  // double d = getValue.asDouble()
}
