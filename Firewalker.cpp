#include "Firewalker.h"

/* Default constructor */
Firewalker::Firewalker(int _N_LEDS = 10,
		       int _STEP_PIN = A9,
		       int _STEP_OFF_TRIGGER = 400) :
  N_LEDS(_N_LEDS), STEP_PIN(_STEP_PIN),
  STEP_OFF_TRIGGER(_STEP_OFF_TRIGGER), stepNum(0) {}


void Firewalker::begin() {
  pinMode(STEP_PIN, INPUT_PULLUP);
  
  memset(stepMag, 0, sizeof(stepMag));
  memset(stepX, 0, sizeof(stepX));

  readFirstStepValue();
}


void Firewalker::readFirstStepValue() {
  stepValue =  analogRead(STEP_PIN);
}


void Firewalker::readStepValue() {
  stepValue = ((stepValue*3) + analogRead(STEP_PIN)) >> 2;
}

int Firewalker::getStepValue() {
  return stepValue;
}


