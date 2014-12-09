#include "Firewalker.h"

/* Default constructor */
Firewalker::Firewalker(int _STEP_PIN /* = A9 */,
		       int _N_LEDS /* = 10 */,
		       int _STEP_OFF_TRIGGER /* = 400 */) :
  N_LEDS(_N_LEDS), STEP_PIN(_STEP_PIN),
  STEP_OFF_TRIGGER(_STEP_OFF_TRIGGER), stepNum(0) {}

/*
 * Initialization, meant to be called in setup()
 */
void Firewalker::begin() {
  pinMode(STEP_PIN, INPUT_PULLUP);
  
  memset(stepMag, 0, sizeof(stepMag));
  memset(stepX, 0, sizeof(stepX));

  readFirstStepValue();
}


/*
 * Updates stepValue by reading analog pin and doing
 * basic filtering, meant to be called in loop()
 */ 
void Firewalker::readStepValue() {
  stepValue = ((stepValue*3) + analogRead(STEP_PIN)) >> 2;
}

/* 
 * Returns private member variable stepValue
 */
int Firewalker::getStepValue() {
  return stepValue;
}

//--- Private Methods ---//

/*
 * Initializes stepValue, meant to be called in begin()
 */
void Firewalker::readFirstStepValue() {
  stepValue =  analogRead(STEP_PIN);
}

