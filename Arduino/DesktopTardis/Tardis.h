/*
 Tardis.h - Library to control the Tropotek Desktop Tardis
 Created by Michael Mifsud, January 6th, 2015.
 */
#ifndef Tardis_h
#define Tardis_h

#include "Arduino.h"
#include "Wtv020sd16p.h"
#include "RgbLed.h"

// which analog pin to connect
#define T_THERMISTORPIN A0
// resistance at 25 degrees C (100K)
#define T_THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define T_TEMPERATURENOMINAL 25
// The beta coefficient of the thermistor (usually 3000-4000)
#define T_BCOEFFICIENT 3950
//#define BCOEFFICIENT 100000
// the value of the 'other' resistor (100K)
#define T_SERIESRESISTOR 10000 

// which analog pin to connect
#define T_TEMP_NORMALIZE -7.6

#define T_TEMP_LED_MIN 5
#define T_TEMP_LED_MAX 35


class Tardis
{
public:
  Tardis(int *topLedPins, int *winLedPins, int *audioPins, int tempPin);

  void setup();  
  // Animation loop
  void loop();  

  // Get the temprature in DegC from the Thermistor
  double getTemp();

  // ----------- Serial Command Functions ---------------
  // Get the system state string
  void sendState();

  void saveState(char **val);



private:
  int _tempPin;

  // counter for animation frames
  int unsigned _frame;

  bool _topLedEnabled;
  int _topDelay;
  RgbLed _topLed;

  bool _winLedEnabled;
  bool _winTempLed;
  RgbLed _winLed;

  int _trackId;
  Wtv020sd16p _sound;


};

#endif





