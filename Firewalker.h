class Firewalker {
 public:
  const int N_LEDS, SHOE_LEN_LEDS, SHOE_LED_BACK,
    STEP_PIN, LED_PIN, MAXSTEPS;


 public:
  Firewalker();
  Firewalker(int, int, int,
	     int, int, int);
  const int getMaxSteps();
};
