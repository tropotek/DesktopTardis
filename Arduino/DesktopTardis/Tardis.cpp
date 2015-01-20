/*
 Tardis.cpp - Library to control the Tropotek Desktop Tardis
 Created by Michael Mifsud, January 6th, 2015.
 */

#include "Tardis.h"


Tardis::Tardis(int *topLedPins, int *winLedPins, int *audioPins, int tempPin) :
_topLed(topLedPins), _winLed(winLedPins), _sound(audioPins[0], audioPins[1], audioPins[2], audioPins[3])
{
  _tempPin = tempPin;
  _topLedEnabled = true;
  _topDelay = 500;
  _topLed.setRgb(255, 255, 255);

  _winLedEnabled = true;
  _winTempLed = false;
  _winLed.setRgb(255, 255, 255);

  _trackId = 0;
}

void Tardis::setup()
{
  _topLed.initPins();
  _winLed.initPins();
  // This stops serial out for some reason?
  _sound.reset();
}

void Tardis::loop()
{
  // TODO: run requested animations
  if (_topLedEnabled) {
    _topLed.writePins();
  } else {
    _topLed.off();
  }
  
  if (_winLedEnabled){ 
    _winLed.writePins();
  } else {
    _winLed.off();
  }

  _frame++;
}

void Tardis::saveState(char **val)
{
  // val[0] = "S00"
  // val[1] = tempVal
  _topLedEnabled = (strcmp(val[2], "true") == 0) ? true : false;
  _topDelay = atoi(val[3]);
  
  if (_topLedEnabled) {
    _topLed.setRed(atoi(val[4]));
    _topLed.setGreen(atoi(val[5]));
    _topLed.setBlue(atoi(val[6]));
  } 

  _winLedEnabled = (strcmp(val[7], "true") == 0) ? true : false;
  _winTempLed = (strcmp(val[8], "true") == 0) ? true : false;
  
  if (_winLedEnabled) {
    _winLed.setRed(atoi(val[9]));
    _winLed.setGreen(atoi(val[10]));
    _winLed.setBlue(atoi(val[11]));
  } 
  
  _trackId = atoi(val[12]);
  // Should we start playing an audio track?
  if (strcmp(val[14], "true") == 0) {
    if (_sound.isBusy()) {
      _sound.stopVoice();
    }
    if (strcmp(val[15], "true") != 0) {
      _sound.asyncPlayVoice(_trackId);
    }
  }
  if (strcmp(val[15], "true") == 0) {
    _sound.stopVoice();
  }
  
  sendState();
}

// Return the tardis state as a string
void Tardis::sendState()
{
  Serial.print("S00 ");

  Serial.print(getTemp(), 2);        // 1: Temprature value in DegC
  Serial.print(" ");
  Serial.print(_topLedEnabled);      // 2: Top Light Enabled
  Serial.print(" ");
  Serial.print(_topDelay);           // 3: Top Light Flash Delay
  Serial.print(" ");
  Serial.print(_topLed.getRed());    // 4: Top light red
  Serial.print(" ");
  Serial.print(_topLed.getGreen());  // 5: Top light green
  Serial.print(" ");
  Serial.print(_topLed.getBlue());   // 6: Top light blue
  Serial.print(" ");
  Serial.print(_winLedEnabled);      // 7: Window Light Enabled
  Serial.print(" ");
  Serial.print(_winTempLed);         // 8: Window temp display enabled
  Serial.print(" ");
  Serial.print(_winLed.getRed());    // 9: Window light Red
  Serial.print(" ");
  Serial.print(_winLed.getGreen());  // 10: window light Green
  Serial.print(" ");
  Serial.print(_winLed.getBlue());   // 11: Window lisght Blue
  Serial.print(" ");
  Serial.print(_trackId);            // 12: Current audio track ID
  Serial.print(" ");
  Serial.print(_sound.isBusy());     // 13: Is audio currently playing
  Serial.print(" ");
  Serial.print("0");                 // 14: play track
  Serial.print(" ");
  Serial.print("0");    // 15: Reserved
  Serial.print(" ");
  Serial.print("0");    // 16: Reserved
  Serial.print(" ");
  Serial.print("0");    // 17: Reserved
  Serial.print(" ");
  Serial.print("0");    // 18: Reserved
  Serial.print(" ");
  Serial.print("0");    // 19: Reserved
  Serial.println();
}



// Get the temprature from a 100K Thermistor
double Tardis::getTemp()
{
  double kelvin = analogRead(_tempPin);

  kelvin = 1023 / kelvin - 1;
  kelvin = T_SERIESRESISTOR / kelvin;
  kelvin =  kelvin/ T_THERMISTORNOMINAL; // (R/Ro)
  kelvin = log(kelvin); // ln(R/Ro)
  kelvin /= T_BCOEFFICIENT; // 1/B * ln(R/Ro)
  kelvin += 1.0 / (T_TEMPERATURENOMINAL + 273.15); // + (1/To)
  kelvin = 1.0 / kelvin; // Invert
  return (kelvin - 273.15);
}




