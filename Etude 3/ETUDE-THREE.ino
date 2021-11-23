//Defining the pins

#define PIN_LED_BUTTON_1  3   
#define PIN_LED_BUTTON_2  5   
#define PIN_LED_BUTTON_3  9 
#define PIN_LED_BUTTON_4  10  
#define PIN_PIEZO         11  

#define LED_BUTTON_COUNT  4   // Number of LEDs & buttons

static int buttonLookup[LED_BUTTON_COUNT] = { PIN_LED_BUTTON_1, PIN_LED_BUTTON_2, PIN_LED_BUTTON_3, PIN_LED_BUTTON_4 };
static long gameSeed;

// Returns the period in microseconds of the half-cycle tone
// for each button.
int ledButtonHalfPeriod(int button) {
  switch (button) {
    case PIN_LED_BUTTON_1:
      return 1911;
    case PIN_LED_BUTTON_2:
      return 1703;
    case PIN_LED_BUTTON_3:
      return 1517;
    case PIN_LED_BUTTON_4:
      return 1432;
  }
  return 3822;
}
//checking if the button is pressed
bool checkButtonPush(int button) {
  bool result = false;
  pinMode(button, INPUT);   
  digitalWrite(button, HIGH);
  
  int halfPeriod = ledButtonHalfPeriod(button);

  //when the button is being pushed
  while (digitalRead(button) == LOW) {
    // Button is being pushed
    result = true;
    digitalWrite(PIN_PIEZO, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(PIN_PIEZO, LOW);
    delayMicroseconds(halfPeriod);
  }
  return result;
}

int getButtonPush(void) {
  fo
  r (int i = 0; i < LED_BUTTON_COUNT; ++i) {
    if (checkButtonPush(buttonLookup[i])) {
      return i;
    }
  }
  return -1;
}
//displaying light and sound for a specific duration when the results are outputting 
void displayLightAndSound(int led, int duration) {
  if (led >= 0) {
    pinMode(led, OUTPUT);   // Set led cathode pin as output
    digitalWrite(led, LOW); 
  }
  //
  int halfPeriod = ledButtonHalfPeriod(led);
  long cycleCount = (long)duration * 500;
  cycleCount /= halfPeriod;
  for (; cycleCount > 0; --cycleCount) {
    digitalWrite(PIN_PIEZO, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(PIN_PIEZO, LOW);
    delayMicroseconds(halfPeriod);
  }
  
  if (led >= 0) {
    pinMode(led, INPUT);      // Set led cathode pin as input
    digitalWrite(led, HIGH);  
  }
}
//the function of powering up the light and sound 
void startUpLightsAndSound(void) {
  // Power-up lights and sound
  displayLightAndSound(PIN_LED_BUTTON_1, 100);
  displayLightAndSound(PIN_LED_BUTTON_2, 100);
  displayLightAndSound(PIN_LED_BUTTON_3, 100);
  displayLightAndSound(PIN_LED_BUTTON_4, 100);

  // Wait for button push to continue
  while (getButtonPush() < 0) {
    delay(100);
  }
  
  // Delay a bit before starting
  delay(500);
}


// the setup routine runs:
void setup() {
  // Set button pins as input
  pinMode(PIN_LED_BUTTON_1, INPUT);
  pinMode(PIN_LED_BUTTON_2, INPUT);
  pinMode(PIN_LED_BUTTON_3, INPUT);
  pinMode(PIN_LED_BUTTON_4, INPUT);
  
  // Set the piezo pin as output
  pinMode(PIN_PIEZO, OUTPUT);

  // Power-up lights and sound
  startUpLightsAndSound();

  // Randomize the first game
  gameSeed = micros();
}

// the loop routine runs over and over again forever:
void loop() {
  static int sequenceLength = 1;
  int i;
  
  // Display current sequence
  randomSeed(gameSeed);
  for (i = sequenceLength; i > 0; --i) {
    displayLightAndSound(buttonLookup[random(LED_BUTTON_COUNT)], 250);
    delay(250);
  }
  // starting a random generator
  randomSeed(gameSeed);
  for (int matches = 0; matches < sequenceLength; ++matches) {
    int button = -1;
    int timeout;
    // Wait for the player to press a button (with 3 second timeout)
    for (button = -1, timeout = 30; timeout && button < 0; --timeout) {
      button = getButtonPush();
      delay(100);
    }
  
    // Check if the correct button was pushed
    if (button != random(LED_BUTTON_COUNT)) {
      // Play sad sound and start a new game
      displayLightAndSound(-1, 1000);
      gameSeed = micros();
      delay(1000);
      sequenceLength = 0;
      startUpLightsAndSound();
    }
  }
  ++sequenceLength;
  delay(1000);
}
