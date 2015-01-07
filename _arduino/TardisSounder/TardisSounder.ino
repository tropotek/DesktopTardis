/*
 Example: Control a WTV020-SD-16P module to play voices from an Arduino board.
 Created by Diego J. Arevalo, August 6th, 2012.
 Released into the public domain.
 */

#include <Wtv020sd16p.h>

int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;  // The pin number of the data pin.
int busyPin = 5;  // The pin number of the busy pin.

/*
Create an instance of the Wtv020sd16p class.
 1st parameter: Reset pin number.
 2nd parameter: Clock pin number.
 3rd parameter: Data pin number.
 4th parameter: Busy pin number.
 */
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

void setup() {
  Serial.begin(57600);
  Serial.println("Initalising: TARDIS Sounder v0.1 (02/01/2015)");
  Serial.println("Support: http://zap.tropotek.com.au/");
  Serial.println();
  
  //Initializes the module.
  wtv020sd16p.reset();
  
}

int i = 0;
boolean playing = false;

void loop() {

  
  if (!playing) {
      playing = true;
      Serial.print("Playing track: 000");
      Serial.print(i);
      Serial.println(".ad4");
      
      wtv020sd16p.asyncPlayVoice(i);
      delay(100);
      
      i++;
      if (i > 3) i = 0;
      //delay(100);
  } else {
    if (!wtv020sd16p.isBusy()) {
      playing = false;
      delay(100);
    }
  }
  
  
  /*
  //Plays synchronously an audio file. Busy pin is used for this method.
  wtv020sd16p.playVoice(0);
  //Plays asynchronously an audio file.
  wtv020sd16p.asyncPlayVoice(1);
  //Plays audio file number 1 during 2 seconds.
  delay(8000);
  //Pauses audio file number 1 during 2 seconds.  
  wtv020sd16p.pauseVoice();
  delay(8000);
  //Resumes audio file number 1 during 2 seconds.
  wtv020sd16p.pauseVoice();
  delay(5000);  
  //Stops current audio file playing.
  wtv020sd16p.stopVoice();
  //Plays synchronously an audio file. Busy pin is used for this method.  
  wtv020sd16p.asyncPlayVoice(2);
  delay(2000);   
  //Mutes audio file number 2 during 2 seconds.
  wtv020sd16p.mute();
  delay(2000);
  //Unmutes audio file number 2 during 2 seconds.
  wtv020sd16p.unmute();
  delay(2000);    
  //Stops current audio file playing.
  wtv020sd16p.stopVoice();
  */
}


