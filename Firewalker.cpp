#include "Firewalker.h"

uint8_t gamma[] PROGMEM = { // Gamma correction table for LED brightness
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

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

  stepping = false;
  readFirstSensorValue();
}


/*
 * Updates stepValue by reading analog pin and doing
 * basic filtering, meant to be called in loop()
 */ 
void Firewalker::updateSensorValue() {
  stepValue = ((stepValue*3) + analogRead(STEP_PIN)) >> 2;
}

/* 
 * Returns private member variable stepValue
 */
int Firewalker::getSensorValue() {
  return stepValue;
}

void Firewalker::updateStep(boolean steppingNew) {
  
  // Update stepping logic
  if (stepping) {
      if (steppingNew) { // continue to step
      stepMin = stepValue < stepMin ? stepValue : stepMin;
    }
    else { // step up      
      // Add new step to the step list (may be multiple in flight)
      stepMag[stepNum] = (STEP_OFF_TRIGGER - stepMin) * 6; // Step intensity
      stepX[stepNum]   = -80; // Position starts behind heel, moves forward
      if(++stepNum >= MAXSTEPS) stepNum = 0; // If many, overwrite oldest
      }
  }
  else {
    if (steppingNew) { // step down
      stepMin = stepValue;
    }
  }

  stepping = steppingNew; // update stepping state
  
  // Render a 'brightness map' for all steps in flight.  It's like
  // a grayscale image; there's no color yet, just intensities.
  uint8_t i, j;
  int mx1, px1, px2, m;
  memset(mag, 0, sizeof(mag));    // Clear magnitude buffer
  for(i=0; i<MAXSTEPS; i++) {     // For each step...
    if(stepMag[i] <= 0) continue; // Skip if inactive
    for(j=0; j<N_LEDS; j++) { // For each LED...
      // Each step has sort of a 'wave' that's part of the animation,
      // moving from heel to toe.  The wave position has sub-pixel
      // resolution (4X), and is up to 80 units (20 pixels) long.
      mx1 = (j << 2) - stepX[i]; // Position of LED along wave
      if((mx1 <= 0) || (mx1 >= 80)) continue; // Out of range
      if(mx1 > 64) { // Rising edge of wave; ramp up fast (4 px)
        m = ((long)stepMag[i] * (long)(80 - mx1)) >> 4;
      } else { // Falling edge of wave; fade slow (16 px)
        m = ((long)stepMag[i] * (long)mx1) >> 6;
      }
      mag[j] += m; // Add magnitude to buffered sum
    }
    stepX[i]++; // Update position of step wave
    if(stepX[i] >= (80 + (N_LEDS << 2)))
      stepMag[i] = 0; // Off end; disable step wave
    else
      stepMag[i] = ((long)stepMag[i] * 127L) >> 7; // Fade
  }

  // For a little visual interest, some 'sparkle' is added.
  // The cumulative step magnitude is added to one pixel at random.
  long sum = 0;
  for(i=0; i<MAXSTEPS; i++) sum += stepMag[i];
  if(sum > 0) {
    i = random(N_LEDS);
    mag[i] += sum / 4;
  }
  
}


uint32_t Firewalker::getLEDColor(uint8_t i) {
  if (i < N_LEDS) {
    level = mag[i];                // Pixel magnitude (brightness)
    if(level < 255) {              // 0-254 = black to red-1
      r = pgm_read_byte(&gamma[level]);
      g = b = 0;
    } else if(level < 510) {       // 255-509 = red to yellow-1
      r = 255;
      g = pgm_read_byte(&gamma[level - 255]);
      b = 0;
    } else if(level < 765) {       // 510-764 = yellow to white-1
      r = g = 255;
      b = pgm_read_byte(&gamma[level - 510]);
    } else {                       // 765+ = white
      r = g = b = 255;
    }
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }
  return BLACK;
}

//--- Private Methods ---//

/*
 * Initializes stepValue, meant to be called in begin()
 */
void Firewalker::readFirstSensorValue() {
  stepValue =  analogRead(STEP_PIN);
}

