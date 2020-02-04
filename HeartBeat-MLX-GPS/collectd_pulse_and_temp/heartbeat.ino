void heart()
{
  milli0 = millis();  //pewaent time in millisec
  //millis()( Returns the number of milliseconds passed since the Arduino board began running the current program   ...... give number of secound from start program to now)
  if(milli0 - milli1 >= ms)   //(required period passed)
  {
    milli1 = milli0;
   //(to save the current start time very important)
   //(if the condition which pass 2 msec is then find max)
    findMax();
   //(qs incidcate if there is a signal exists then go to serial)
    if(QS == true)
    {
      Serial.print(F("BPM: "));
      Serial.print(BPM);
      Serial.print(F("\n"));
         QS = false;

      if(BPM<60||BPM>150)
      {Serial.print("check person");
        Serial.print(BPM);
      Serial.print(F("\n"));
   
      //( to return it to initial state)
      }
    }
  }
  if(pulse.available()>0)
{
  pulse.write(BPM);
}
}

void findMax()
{
  Signal = analogRead(pulsePin);              // read the Pulse Sensor 
  sampleCounter += ms;                    // keep track of the time in mS with this variable 
  //(law kraa mn pin kda yba2a 3ada 2msec store them in sample counter to determine pulse counting)
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise 
  //(totalbeat counter--time between two beats to avoid noise ...so we get only required pulse)

    //  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI (t de ta7t fa 3asan kda momken td7ol 3al el noise)
    if (Signal < T){                        // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
  }                                        // keep track of highest point in pulse wave
  //(kda 7asab el kama wl ka3)

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS(total pulse-last one)
      lastBeatTime = sampleCounter;               // keep track of time for next pulse 

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;    //(haedel yakod pulse we y3malaha save we yemsa7a mn el flag 3ashan yakod wa7ada tany l7d ma ybko 10)                  
        }
      }

      if(firstBeat){                         // if it's the --first time-- we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        return;                              // IBI value is unreliable so discard it
        //(bya3mal kda 3ashan awal pulsa btege leh mmoken tkon roung awa edek mahzoza fa byakod el taya 3ashan el deka tebka a3ala0)
      }   


      // keep a running total of the last 10 IBI values
      word runningTotal = 0;                  // clear the runningTotal variable (word 16 bit unsigned int ya3ney any rakm)   

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR (isr interput to sent reading immediately)
      //(kda hasab drabat el alab )
    }                       
  }

  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
    digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
                                              T = thresh;
        //da ely hayo2fo lma ya2a we kalas hawo2f msec
                                                    
  }

  if (N > 2500){                           // if 2.5 seconds go by --without-- a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
    //(da ele hay02f0 lwa m7adsh 7atet sbo3a fa mefesh eraya)
  }
}
