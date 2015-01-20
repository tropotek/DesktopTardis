/*
 * RgbLed.h - Library to control RGB LED's
 * Created by Michael Mifsud, January 6th, 2015.
 * 
 * It is recommended to use PWM pins to ensure the RGB LED works correctly.
 * 
 */
#ifndef RgbLed_h
#define RgbLed_h

#include "Arduino.h"

class RgbLed
{
public:
  RgbLed(int *ledPins);
  RgbLed(int rPin, int gPin, int bPin);

  void setRgb(int r, int g, int b);
  void setRgb(int *rgb);

  void setRed(int r);
  void setGreen(int g);
  void setBlue(int b);

  int getRed();
  int getGreen();
  int getBlue();

  void hsi2rgb(float H, float S, float I, int* rgb);
  void setTemperatureColor(float degC);
  
  void initPins();
  void writePins();
  void off();

private:
  int _r;
  int _g;
  int _b;
  int _rPin;
  int _gPin;
  int _bPin;

  int clamp(int i);
};

#endif


