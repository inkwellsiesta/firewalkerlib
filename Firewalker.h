#ifndef FIREWALKER_H
#define FIREWALKER_H

#include <Arduino.h>

// 32 bit representations of color, meant to be used 
// for debugging and error checking
#define WHITE 0xffffff
#define BLACK 0x000000


class Firewalker {

 public:
  Firewalker(int _STEP_PIN = A9,
	     int _N_LEDS = 10,
	     int _STEP_OFF_TRIGGER = 400);
  void begin();
  void updateSensorValue();
  int getSensorValue();
  void updateStep(boolean);
  uint32_t getLEDColor(uint8_t i);

private:
  void readFirstSensorValue();

  static const int MAXSTEPS = 3;
  static const int MAX_N_LEDS = 15;
  int N_LEDS;
  int STEP_PIN;
  int STEP_OFF_TRIGGER;
  int stepMag[MAXSTEPS], stepX[MAXSTEPS];
  int mag[MAX_N_LEDS];
  int stepValue;  int stepMin;
  uint8_t stepNum;
  boolean stepping;
  uint8_t r,g,b;
  int level;
};

#endif
