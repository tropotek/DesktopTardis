/*
 RgbLed.cpp - Library to control RGB LED's
 Created by Michael Mifsud, January 6th, 2015.
 */

#include "RgbLed.h"

// Max color intensity
const unsigned int RGB_MAX_INTENSITY = 255;

// Pallet colors r, g, b
const unsigned int RGB_PALLET_SIZE = 3;

// Constants for the location of colors in a 3 element array
const unsigned int RGB_RED = 0;
const unsigned int RGB_GREEN = 1;
const unsigned int RGB_BLUE = 2;

RgbLed::RgbLed(int *ledPins)
{
  _rPin = ledPins[RGB_RED];
  _gPin = ledPins[RGB_GREEN];
  _bPin = ledPins[RGB_BLUE];
  _r = _g = _b = 0;
}
RgbLed::RgbLed(int rPin, int gPin, int bPin)
{
  _rPin = rPin;
  _gPin = gPin;
  _bPin = bPin;
  _r = _g = _b = 0;
}

void RgbLed::initPins()
{
  pinMode(_rPin, OUTPUT);
  pinMode(_gPin, OUTPUT);
  pinMode(_bPin, OUTPUT);
}
void RgbLed::writePins()
{
  initPins();
  analogWrite(_rPin, _r);
  analogWrite(_gPin, _g);
  analogWrite(_bPin, _b);
}
void RgbLed::off()
{
  initPins();
  analogWrite(_rPin, 0);
  analogWrite(_gPin, 0);
  analogWrite(_bPin, 0);
}


void RgbLed::setRgb(int r, int g, int b)
{
  setRed(r);
  setGreen(g);
  setBlue(b);

  writePins();
}
void RgbLed::setRgb(int *rgb) 
{
  //    setRed(rgb[RGB_RED]);
  //    setGreen(rgb[RGB_GREEN]);
  //    setBlue(rgb[RGB_BLUE]);
  // Try this using an incremental pointer
  setRed(*rgb);
  setGreen(*rgb+1);
  setBlue(*rgb+2);

  writePins();
}


void RgbLed::setRed(int r)
{
  _r = clamp(r);
}
void RgbLed::setGreen(int g)
{
  _g = clamp(g);
}
void RgbLed::setBlue(int b)
{
  _b = clamp(b);
}

int RgbLed::getRed()
{
  return _r;
}
int RgbLed::getGreen()
{
  return _g;
}
int RgbLed::getBlue()
{
  return _b;
}

void RgbLed::hsi2rgb(float H, float S, float I, int* rgb) {
  int r, g, b;
  H = fmod(H,360); // cycle H around to 0-360 degrees
  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):
  0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):
  0;

  // Math! Thanks in part to Kyle Miller.
  if(H < 2.09439) {
    r = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    b = 255*I/3*(1-S);
  } 
  else if(H < 4.188787) {
    H = H - 2.09439;
    g = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    r = 255*I/3*(1-S);
  } 
  else {
    H = H - 4.188787;
    b = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    g = 255*I/3*(1-S);
  }
  rgb[RGB_RED]=r;
  rgb[RGB_GREEN]=g;
  rgb[RGB_BLUE]=b;
}

/**
 * Set the color from a Temperature supplied in Deg Centigrade
 *
 *
 */
void RgbLed::setTemperatureColor(float degC)
{
  float min = 10;
  float max = 37;

  // Map the temperature to a 0-1 range
  float a = (degC + abs(min)) / (abs(min)+max);
  //float a = (degC + 30) / 60;
  a = (a < 0) ? 0 : ((a > 1) ? 1 : a);
  // Scrunch the green/cyan range in the middle
  float sign = (a < .5) ? -1 : 1;
  a = sign * pow(2 * abs(a - .5), .35)/2 + .5;
  // Linear interpolation between the cold and hot
  int h0 = 259;
  int h1 = 12;
  // Hue
  int h = (h0) * (1 - a) + (h1) * (a);
  // Convert to RGB int values
  int rgb[3];
  hsi2rgb(h, 75, 150, rgb);
  setRgb(rgb);
}




int RgbLed::clamp(int i)
{
  if (i > 255) i = 255;
  if (i < 0) i = 0;
  return i;
}





