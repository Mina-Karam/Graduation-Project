#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".

void Pulse_Setup(void){
  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(A0);         // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
  //pulseSensor.blinkOnPulse(13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(550);      // Determine which Signal to "count as a beat" and which to ignore.

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }
}

void Pulse_Loop(void){
  if (pulseSensor.sawStartOfBeat()) {
      Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
      Serial.print("BPM: ");                        // Print phrase "BPM: " 
      Serial.println(myBPM);                        // Print the value inside of myBPM. 
      //Cayenne.virtualWrite(2, myBPM);
      } 
}
