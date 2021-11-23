//pitch shifter test 2

//Buzzer
#define buzzer 9
//Button
#define buttonPin 2

//Force sensitive resistor (aka FSR)
#define FSRPin A0

//Music loop
bool isBuzzing = false;

//music tone
int currentTone = 100;

int pressure = 0;
//Variable to read the pressure
int pressureReading = abs(pressure);

//Sets pressures
int noPressure = 0;
//
int lightPressure = 100;
//
int mediumPressure = 300;
//
int highPressure = 500;

//Modes
int mode = 0; //1,2,3; 0==off

// distance and music duration variables will change:
int duration, cm;

//Sets the pins
void setup() {
  Serial.begin(9600);
  // Sets pins
  pinMode(buttonPin, INPUT);
  pinMode(FSRPin, INPUT);
  pinMode(buzzer, OUTPUT);

}

//Sets the voids
void loop() {
  Serial.println(pressureReading);
  pressureReading =  analogRead(FSRPin);
  chooseMode();
  selectMode();
}
void chooseMode() {
  //  Changes mode each time the mode button is pressed
  //Goes from mode 0(reset) to mode 1
  if (digitalRead(buttonPin) == HIGH) {
    if (mode == 0) {
      mode = 1;
      delay(100);
    }
    //Goes from mode 1 (live) to mode 2(record)
    else if (mode == 1) {
      mode = 0;
      delay(100);
    }
  }

}
void selectMode()
{
  //  Sets the void used in the modes
  if (mode == 0) {
    reset();
  }
  else if (mode == 1) {
    checkTone();
    checkPressure();
  }
}

void reset()
//Stops the music loop and sensor to work
{
  if ( pressureReading >= 0) {
    noTone(buzzer);
    isBuzzing = false;
    Serial.print("Working ");
  }
}
//Sets tone
void checkTone() {
  if (digitalRead(buttonPin) == HIGH) {
    currentTone += 100;

    if (currentTone >= 1200) {
      currentTone = 50;
    }
  }
}
//Check pressure
void checkPressure() {
  //Duration depends on FSR pin input
  //Converts distance to music duration
  //Converts tone depending on the pressure applied
  currentTone = pressureReading * 5;
  //changes pitch based on pressure
  if ( pressureReading >= noPressure && pressureReading <= lightPressure) {
    // Buzz
    tone(buzzer, currentTone);
    isBuzzing = true;
  } else if (pressureReading >= lightPressure && pressureReading <= mediumPressure) {
    // Buzz
    tone(buzzer, currentTone);
    isBuzzing = true;

  } else if (pressureReading >= mediumPressure && pressureReading <= highPressure) {
    // Buzz
    tone(buzzer, currentTone);
    isBuzzing = true;
  }
  else { // Don't buzz
    noTone(buzzer);
    isBuzzing = false;

  }

  // Print the pressure
  Serial.print("Pressure: ");
  Serial.print(pressureReading);
  Serial.print("weight");
  Serial.println();

  delay(200);

}
