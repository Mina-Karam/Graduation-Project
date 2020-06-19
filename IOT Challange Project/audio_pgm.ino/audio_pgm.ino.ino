//TMRpcm library is needed

#include "SPI.h"
#include "SD.h"
#include "TMRpcm.h"


#define SD_ChipSelectPin 10
TMRpcm tmrpcm;
File root;

void setup()
{
Serial.begin(9600);
Serial.print("Initializing SD card...");
if(!SD.begin(SD_ChipSelectPin))
{
  Serial.println("SD fail");
  return;
}
  Serial.println("OK!");


tmrpcm.speakerPin = 9;
root = SD.open("/");      // open SD card main root
printDirectory(root, 0);  // print all files names and sizes

tmrpcm.setVolume(10);
tmrpcm.quality(1);      //  Set 1 for 2x oversampling Set 0 for normal

//tmrpcm.play("test.wav");


}

// main loop
void loop() {
 
  if ( !tmrpcm.isPlaying() ) {
    // no audio file is playing
      File entry =  root.openNextFile();  // open next file
      if (! entry) {
        // no more files
        root.rewindDirectory();  // go to start of the folder
        return;
      }
 
    uint8_t nameSize = String(entry.name()).length();  // get file name size
    String str1 = String(entry.name()).substring( nameSize - 4 );  // save the last 4 characters (file extension)
 
    if ( str1.equalsIgnoreCase(".wav") ) {
      // the opened file has '.wav' extension
      tmrpcm.play( entry.name() );      // play the audio file
      Serial.print("Playing file: ");
      Serial.println( entry.name() );
    }
 
    else {
      // not '.wav' format file
      entry.close();
      return;
    }
   }
 
}
 
 
void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
 
// end of code.
