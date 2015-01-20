/*
 *  TARDIS LIGHTS
 * Author: Michael Mifsud <http://www.tropotek.com/>
 *
 * This sketch assumes that pins 1-12 are used for 4 LEDS
 * Led 1 will be used as the light in the top Tardis lamp
 *
 * TODO: Map Pins -> LED here?????
 *
 */

// Mapped to PWM pins
#include "Arduino.h"
#include "Tardis.h"


// Top Lamp LED pins
int topLedPins[] = {
  9, 10, 11 // {R, G, B}
};
// Window LED pins
int winLedPins[] = {
  3, 5, 6 // {R, G, B}
};
//  WTV020-SD-16P Sound module Pins
int audioPins[4] = {
  2,    // The pin number of the reset pin.
  4,    // The pin number of the clock pin.
  7,    // The pin number of the data pin.
  8     // The pin number of the busy pin.
};
// Tempreture sensor (100k Termistor) pin
int tempPin = A0;



// System Code Below

Tardis tardis = Tardis(topLedPins, winLedPins, audioPins, tempPin);
int fps = 10; // Frames per second for the animation loop


void setup() {  
  Serial.begin(57600);
  Serial.println("# Initalising: TARDIS  v0.1 [07/01/2015]");
  Serial.println("# Support: http://zap.tropotek.com.au/");
  tardis.setup();
  
  delay(500);
  tardis.sendState();
}


void loop() {
  
  
  long unsigned time = millis();
  int frameDelay = floor(1000/fps);

  // receive any commands from the serial terminal
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    processCmd(input);
  }

  if ((time % frameDelay) == 0) {
    // Code to run on an animation frame
    tardis.loop();
  }

}

void processCmd(String s) {
  if (s.startsWith("S00")) {  // Write system state to Serial out
    tardis.sendState();
  } 
  else if (s.startsWith("S01")) {  // write state to Tardis object
    char cs[100];
    s.toCharArray(cs, 100);
    tardis.saveState(split(cs, " "));
  }
}





char** split(char str[], const char* delimiters) {
  int result_size = countDelimiters(str, delimiters);
  int i = 0;
  char* result[result_size];
  char* pch = strtok(str, delimiters);

  while (pch != NULL)
  {
    result[i] = pch;
    pch = strtok(NULL, delimiters);
    ++i;
  }

  return result;
}

int countDelimiters(char str[], const char* delimiters) {
  int i, j, result = 0;
  for (i = 0; i < strlen(str); ++i) {
    for (j = 0; j < strlen(delimiters); ++j) {
      if (str[i] == delimiters[j]) {
        ++result;
      }
    }
  }
  return (result + 1);
  
}


