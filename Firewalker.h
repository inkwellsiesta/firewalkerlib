#include <Arduino.h>

#define WHITE 0xffffff
#define BLACK 0x000000

class Firewalker {
  
 public:
  Firewalker(int, int, int);
  void begin();
  void readStepValue();
  int getStepValue();

private:
  void readFirstStepValue();

  static const int MAXSTEPS = 3;
  const int N_LEDS;
  int STEP_PIN;
  int STEP_OFF_TRIGGER;
  int stepMag[MAXSTEPS], stepX[MAXSTEPS];
  int stepValue;
  int stepMin;
  uint8_t stepNum;
};
