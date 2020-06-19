#include "SD.h"
#include "TMRpcm.h"

// Connection at Mega 
/*  50 == MISO
*   51 == MOSI
*   52 == SCK
*   53 == ss
*/
#define SD_ChipSelectPin 53
TMRpcm tmrpcm;
File root;

void SD_Setup(void){
  Serial.print("Initializing SD card...");
  if(!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
    Serial.println("OK!");


  tmrpcm.speakerPin = 9;   
      
  tmrpcm.setVolume(10);
  tmrpcm.quality(1);      //  Set 1 for 2x oversampling Set 0 for normal
  
}

void SD_Loop(void){
  if ( !tmrpcm.isPlaying() ) {
    // no audio file is playing
      File entry =  root.openNextFile();  // open next file
      if (! entry) {
        // no more files
        root.rewindDirectory();  // go to start of the folder
        return;
      }
     String str1 = String(entry.name()); 
 
    if ( str1.equalsIgnoreCase(".wav") ) {
      // the opened file has '.wav' extension
      tmrpcm.play( entry.name() );      // play the audio file
      Serial.print("Playing file: ");
      Serial.println( entry.name() );
    }

    }
   }
 
