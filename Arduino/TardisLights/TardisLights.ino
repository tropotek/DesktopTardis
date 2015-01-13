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

#include <Wtv020sd16p.h>

//  WTV020-SD-16P Sound module Pins
int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 4;  // The pin number of the clock pin.
int dataPin = 7;  // The pin number of the data pin.
int busyPin = 8;  // The pin number of the busy pin.

/*
 Create an instance of the Wtv020sd16p class.
 1st parameter: Reset pin number.
 2nd parameter: Clock pin number.
 3rd parameter: Data pin number.
 4th parameter: Busy pin number.
 */
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);


const int pins[] = {
  3, 5, 6, // Windows
  9, 10, 11 // Top Lamp
};

const int tempPin = A0; // 14


// Constants
const int NUM_CHANNELS = 2; 
const int MAX_INTENSITY = 255;
const int PALLET_SIZE = 3;
const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;

// Global vars
int imap[NUM_CHANNELS][PALLET_SIZE];

const int rawMax = 30;
float rawArray[rawMax];
int rawCnt = 0;


// Max animation count
const int AN_MODE_MAX = 2;
const int AN_REPEAT = 40; // Animation repeat before cycling to the next

void setup() {  
  Serial.begin(57600);
  Serial.println("Initalising: TARDIS  v0.1 [07/01/2015]");
  Serial.println("Support: http://zap.tropotek.com.au/");
  Serial.println();
  
  // Initialize audio the module.
  wtv020sd16p.reset();
  

  
  // Init the LED array
  for (int i = 0; i < rawMax; i++)  {
    rawArray[i] = 0.0;
  }
  initPins();
  fillMap(0);
  
}


int anCnt = 0;
void loop() {
   if (!wtv020sd16p.isBusy()) {
      // Play a tune
      randomSeed(analogRead(tempPin));
      //wtv020sd16p.asyncPlayVoice(random(0, 3));
    }


  if (anCnt == 0) {
    cycle();
  } else if (anCnt < 10) {
    setColorTemp(getTemp());
    flashLamp();
  } else if (anCnt < 40) {
    setColorTemp(getTemp());
    set(1, 0, 0, 0);
    writeMap();
    delay(1000);
  } else {
    fillMap(0);
    writeMap();
    delay(1000);
  }
  
  Serial.print("Frame: ");
  Serial.println(anCnt);
  
  // manage loop counter
  anCnt++;
  if (anCnt > AN_REPEAT) anCnt = 0;
}


void flashLamp() {
  set(1, 0, 0, 0);
  writeMap();
  for (int j = 1; j < 255; j++) {
    set(1, j-(floor(j*0.40)), j, j);
    writeMap();
    delay(5);
    if (j < 10) {
      delay(30);
    }
  }
  for (int j = 255; j > 0; j = j-2) {
    set(1, j, j, j);
    writeMap();
    delay(1);
  }
  set(1, 0, 0, 0);
  writeMap();
  delay(650);
}

void anTest() {
  int val = 200;
  set(0, val, 0, 0);
  set(1, val, 0, 0);
  writeMap();
  delay(1000);

  set(0, 0, val, 0);
  set(1, 0, val, 0);
  writeMap();
  delay(1000);

  set(0, 0, 0, val);
  set(1, 0, 0, val);
  writeMap();
  delay(1000);

  set(0, 0, 0, 0);
  set(1, 0, 0, 0);
  writeMap();
  delay(1000);
}

void cycle() {
  unsigned int rgbColour[3];

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;
  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;
    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      set(0, rgbColour[0], rgbColour[1], rgbColour[2]);
      set(1, rgbColour[0], rgbColour[1], rgbColour[2]);
      writeMap();
      delay(5);
    }
  }
} 


void setColorTemp(float degC) {
  float min = 0;
  float max = 35;

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
  // return pusher.color("hsv", h, 75, 90).hex6();s
  // Convert to RGB int values
  int rgb[3];
  hsi2rgb(h, 75,150, rgb);
  
  Serial.print("H: ");
  Serial.print(h);
  Serial.print(" - R: ");
  Serial.print(rgb[0]);
  Serial.print(" G: ");
  Serial.print(rgb[1]);
  Serial.print(" B: ");
  Serial.println(rgb[2]);
  
  set(0, rgb[0], rgb[1], rgb[2]);
}



// ------------- LED Functions ---------------------
void add(int id, int red, int green, int blue) {
  addRed(id, red);
  addGreen(id, green);
  addBlue(id, blue);
}
void addRed(int id, int intensity)  {
  imap[id][RED] += intensity;
  if (imap[id][RED] > 255) imap[id][RED] = 255;
  if (imap[id][RED] < 0) imap[id][RED] = 0;
}
void addGreen(int id, int intensity)  {
  imap[id][GREEN] += intensity;
  if (imap[id][GREEN] > 255) imap[id][GREEN] = 255;
  if (imap[id][GREEN] < 0) imap[id][GREEN] = 0;
}
void addBlue(int id, int intensity)  {
  imap[id][BLUE] += intensity;
  if (imap[id][BLUE] >= 255) imap[id][BLUE] = 255;
  if (imap[id][BLUE] <= 0) imap[id][BLUE] = 0;
}

void set(int id, int red, int green, int blue) {
  setRed(id, red);
  setGreen(id, green);
  setBlue(id, blue);
}
void setRed(int id, int intensity)  {
  imap[id][RED] = intensity;
}
void setGreen(int id, int intensity)  {
  imap[id][GREEN] = intensity;
}
void setBlue(int id, int intensity)  {
  imap[id][BLUE] = intensity;
}


int getRed(int id)  {
  return imap[id][RED];
}
int getGreen(int id)  {
  return imap[id][GREEN];
}
int getBlue(int id)  {
  return imap[id][BLUE];
}

void hsi2rgb(float H, float S, float I, int* rgb) {
  int r, g, b;
  H = fmod(H,360); // cycle H around to 0-360 degrees
  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
  
  // Math! Thanks in part to Kyle Miller.
  if(H < 2.09439) {
    r = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    b = 255*I/3*(1-S);
  } else if(H < 4.188787) {
    H = H - 2.09439;
    g = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    r = 255*I/3*(1-S);
  } else {
    H = H - 4.188787;
    b = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    g = 255*I/3*(1-S);
  }
  rgb[0]=r;
  rgb[1]=g;
  rgb[2]=b;
}

// ------------ System functions --------------------
// which analog pin to connect
#define THERMISTORPIN A0
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
//#define BCOEFFICIENT 100000
// the value of the 'other' resistor
#define SERIESRESISTOR 10000 

double getTemp() {
  int sumRaw = 0;
  int rawvoltage = analogRead(tempPin);

  double kelvin = analogRead(tempPin);
  kelvin = 1023 / kelvin - 1;
  kelvin = SERIESRESISTOR / kelvin;
  //  Serial.print("Therm Resistance: ");
  //  Serial.println(kelvin);
  kelvin =  kelvin/ THERMISTORNOMINAL; // (R/Ro)
  kelvin = log(kelvin); // ln(R/Ro)
  kelvin /= BCOEFFICIENT; // 1/B * ln(R/Ro)
  kelvin += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  kelvin = 1.0 / kelvin; // Invert
  double celsius = kelvin - 273.15;

  Serial.print("Temp DegC: ");
  Serial.println(celsius);

  return celsius;
}

void printMap() {
  //Serial.println();
  for (int i = 0; i < NUM_CHANNELS; i++) {
    Serial.print(i);
    Serial.print(": ");
    for(int j = 0; j < PALLET_SIZE; j++) {
      Serial.print(imap[i][j]);
      if (j < PALLET_SIZE-1) {
        Serial.print(",");
      }
      Serial.print(" ");
    }
    Serial.println();
  }
}
void initPins()  {
  for (int i = 0; i < NUM_CHANNELS; i++) {
    for(int j = 0; j < PALLET_SIZE; j++) {
      pinMode(pins[((i*PALLET_SIZE)+(j+1))-1], OUTPUT);
    }
  }
}
void fillMap(int value)  {
  for (int i = 0; i < NUM_CHANNELS; i++) {
    set(i, value, value, value);
  }
}
void writeMap() {
  for (int i = 0; i < NUM_CHANNELS; i++) {
    for(int j = 0; j < PALLET_SIZE; j++) {
      analogWrite(pins[((i*PALLET_SIZE)+(j+1))-1], imap[i][j]);
    }
  }
}







