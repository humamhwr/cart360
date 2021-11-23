//pitch shifter test

//Buzzer
#define buzzer 9
//Button
#define buttonPin 2

//Ultrasonic sensor
#define trigPin 8
#define echoPin 7

//Music loop
bool isBuzzing = false;

//music tone
int currentTone = 100;

//Modes
int mode = 0; //1,2,3; 0==off

// distance and music duration variables will change:
int duration, cm;

//Sets the pins
void setup() {
  Serial.begin(9600);
  // Sets pins
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

}

//Sets the voids
void loop() {
  chooseMode();
  selectMode();
}


void chooseMode() {
  //  Changes mode each time the mode button is pressed
  //Goes from mode 0(reset) to mode 1
  if (digitalRead(buttonPin) == HIGH) {
    if (mode == 0) {
      mode = 1;
      delay(200);
    }
    //Goes from mode 1 (live) to mode 2(record)
    else if (mode == 1) {
      mode = 0;
      delay(200);
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
    checkDistance();
  }
}

void reset()
//Stops the music loop and sensor to work
{
  if ( cm >= 0) {
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
//Check distance
void checkDistance() {

  //Pulses wave
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  //Duration depends on echo pin input
  duration = pulseIn(echoPin, HIGH);

  //Converts distance to music duration
  cm = microsecondsToCentimeters(duration);

  //changes pitch
  currentTone = cm * 5;

  // if distance less than 0.5 meter and more than 0 (0 or less means over range)
  if (cm <= 50 && cm >= 0) {
    // Buzz
    tone(buzzer, currentTone);
    isBuzzing = true;
  } else if (cm > 50) {
    // Don't buzz
    noTone(buzzer);
    isBuzzing = false;
  }
  // Print the distance
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(200);

}
//Converts microseconds
long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
