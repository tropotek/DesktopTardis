/*
 Tardis.cpp - Library to control the Tropotek Desktop Tardis
 Created by Michael Mifsud, January 6th, 2015.
 */

#include "Tardis.h"

 
Tardis::Tardis(int *topLedPins, int *winLedPins, int *audioPins, int tempPin) :
  _topLed(topLedPins), _winLed(winLedPins), _sound(*audioPins, *audioPins+1, *audioPins+2, *audioPins+3)
{
  _tempPin = tempPin;
  _topLedEnabled = true;
  _topDelay = 500;
  _topLed.setRgb(255, 255, 255);
  
  _winLedEnabled = true;
  _winTempLed = true;
  _winLed.setRgb(255, 255, 255);
  
  _trackId = 0;
  _playTrack = false;
  
}
 
void Tardis::loop()
{
   // TODO: run requested animations
   
   
   _frame++;
}

  void Tardis::saveState(char **val)
  {
//    Serial.println("# Saving TARDIS state");
//    Serial.print("# ");
//    Serial.println((strcmp(val[2], "true") == 0) ? "true" : "false");

    _topLedEnabled = (strcmp(val[2], "true") == 0) ? true : false;
    
    
    _winLedEnabled = (strcmp(val[7], "true") == 0) ? true : false;
    delay(500);
    sendState();
  }
 
// Return the tardis state as a string
void Tardis::sendState()
{
  Serial.print("S00 ");

  Serial.print(getTemp(), 2);        // Temprature value in DegC
  Serial.print(" ");
  Serial.print(_topLedEnabled);      // Top Light Enabled
  Serial.print(" ");
  Serial.print(_topDelay);           // Top Light Flash Delay
  Serial.print(" ");
  Serial.print(_topLed.getRed());    // Top light red
  Serial.print(" ");
  Serial.print(_topLed.getGreen());  // Top light green
  Serial.print(" ");
  Serial.print(_topLed.getBlue());   // Top light blue
  Serial.print(" ");
  Serial.print(_winLedEnabled);      // Window Light Enabled
  Serial.print(" ");
  Serial.print(_winTempLed);         // Window temp display enabled
  Serial.print(" ");
  Serial.print(_winLed.getRed());    // Window light Red
  Serial.print(" ");
  Serial.print(_winLed.getGreen());  // window light Green
  Serial.print(" ");
  Serial.print(_winLed.getBlue());   // Window lisght Blue
  Serial.print(" ");
  Serial.print(_trackId);            // Current audio track ID
  Serial.print(" ");
  Serial.print(_sound.isBusy());     // Is audio currently playing
  Serial.print(" ");
  Serial.print("0");                 // play track
  Serial.print(" ");
  Serial.print("0");    // Reserved
  Serial.print(" ");
  Serial.print("0");    // Reserved
  Serial.print(" ");
  Serial.print("0");    // Reserved
  Serial.print(" ");
  Serial.print("0");    // Reserved
  Serial.print(" ");
  Serial.print("0");    // Reserved
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
 
 
 
