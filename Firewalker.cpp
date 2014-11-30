#include <Arduino.h>
#include "Firewalker.h"


/* Default constructor */
Firewalker::Firewalker(){}

Firewalker::Firewalker(int _N_LEDS = 8, // TOTAL number of LEDs in strip
	     int _SHOE_LEN_LEDS = 8, // Number of LEDs down ONE SIDE of shoe
	     int _SHOE_LED_BACK = 5, // Index of REAR-MOST LED on shoe
	     int _STEP_PIN = A9, // Analog input for footstep
	     int _LED_PIN = 6, // NeoPixel strip is connected here
	     int _MAXSTEPS = 3): // Process (up to) this many concurrent steps
  N_LEDS(_N_LEDS), SHOE_LEN_LEDS(_SHOE_LEN_LEDS),
    SHOE_LED_BACK(_SHOE_LED_BACK), STEP_PIN(_STEP_PIN),
    LED_PIN(_LED_PIN), MAXSTEPS(_MAXSTEPS) {}
