//pitch shifter test

#define buzzer 9
#define buttonPin 2

#define pingPin 8

unsigned long previousTime = 0;
unsigned long delayTime = 100;

bool isBuzzing = false;
int currentTone = 100;

int buttonState=0;

int mode=1; //1,2,3; 0==off


void setup(){
 Serial.begin(9600);
 pinMode(buzzer, OUTPUT);
 pinMode(buttonPin, INPUT);
 
  
}


void loop(){
 
  buttonState= digitalRead(buttonPin);
  
  if(buttonState== HIGH){
      currentTone +=100;
    
    if(currentTone >=1200){
        currentTone =50;
    }
    
      
      delay(100);
  }else{
      //do nothing
  }
  playLoop();
  checkDistance()
}

void checkDistance(){

      long duration,cm;


  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);


  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  cm = microsecondsToCentimeters(duration);

  // Print the distance
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(100);
  
  //changes pitch
  
  currentTone = cm*5;

}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void playLoop(){
    
  if (mode ==0){
      noTone(buzzer);
  }else if (mode ==1){
      loopOne();
  }else if (mode ==2){
      loopTwo();
  }
    
}

void loopOne(){

      unsigned long timeNow = millis();
  
  if(timeNow - previousTime > delayTime){
    
  
    if(isBuzzing){
          isBuzzing=false;
    }else{
          isBuzzing = true;
    }
    
    previousTime = timeNow;
  
  }
  
  if(!isBuzzing){
      noTone(buzzer);
  }else{
      tone(buzzer, currentTone);
  }
}
