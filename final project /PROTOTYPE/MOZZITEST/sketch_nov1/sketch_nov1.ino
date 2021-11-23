/****************************
 * 
 * VIBECheck Music Controller
 */

//MOZZI STUFF

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
#define CONTROL_RATE 64
byte gain = 255;

//speaker
#define buzzer A1

//pitches 
#include "pitches.h"
int chordD[] = {NOTE_D4,NOTE_FS4,NOTE_A4,NOTE_D5};
int chordF[] = {NOTE_FS4,NOTE_A4,NOTE_D5,NOTE_FS5};
int chordG[] = {NOTE_G4,NOTE_B4,NOTE_D5,NOTE_G5};
int chordA[] = {NOTE_A4,NOTE_D5,NOTE_FS5,NOTE_A5};
int chordD2[] = {NOTE_D5,NOTE_FS5,NOTE_A5,NOTE_D6};
int chordF2[] = {NOTE_FS5,NOTE_A5,NOTE_D6,NOTE_FS6};
int chordG2[] = {NOTE_G5,NOTE_B5,NOTE_D6,NOTE_G6};
int chordC[] = {NOTE_C4,NOTE_E4,NOTE_G4,NOTE_C5};
int chordA2[] = {NOTE_A5,NOTE_D6,NOTE_FS6,NOTE_A6};
//to be changes to play
int note[] = {NOTE_G4,NOTE_B4,NOTE_D5,NOTE_G5};
int tempo =120; //this is the base duration of a note, tempo is an inaccurate term, but duration is already used to name the flex sensor

//flex sensor
#define flexPin A0 
int flex = 0;
int flexReading = abs(flex);

//echolocation
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

//modes
bool isPlaying = true;
int loopMode = 1; //different patterns 



void setup() {

  startMozzi(CONTROL_RATE);
  aSin.setFreq(150);
  
  pinMode(buzzer, OUTPUT);

  pinMode(flexPin, INPUT);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {

  audioHook();
  
  if(isPlaying ==true){
    play();
    //playMozzi();
    }else if (isPlaying ==false){
    pause();  
    }
}

void updateControl(){
  // as byte, this will automatically roll around to 255 when it passes 0
  gain -=3;
}

int updateAudio(){
  return aSin.next();
}

void play(){
  checkFlex();
  distanceChecker();
  updateTone();
  updatePattern();
  playTone();
}

void playMozzi(){
  updateControl();
  updateAudio();
  distanceChecker();
  checkFlex();
  tone(buzzer, aSin.next()*gain*distance*0.015*flexReading); 
    delay(120);        
    noTone(buzzer);   
    delay(10); 
  
  }

void pause(){
  noTone(buzzer);  
}

int checkFlex(){
  
  flexReading =  analogRead(flexPin);
  Serial.print("Flexion: ");
  Serial.print(flexReading);
  Serial.print("degree");
  Serial.println();
  delay(2);

  tempo = flexReading*4 +120;

  return flexReading;
  
  }

//linked to flex detection
void updatePattern(){

  distanceChecker();
  //v close to echo sensor
  if (distance <5){

    //temporarilt testing with only 2 modes
    if (loopMode>6){
        loopMode = 1;
        delay(5);
      }else{
        loopMode++;
        
        }
        Serial.println("Pattern:");
        Serial.println(loopMode);
    }
 }

//change the chord based on distance
void updateTone(){

  if(distance <6){
      note[0] = chordD[0];
      note[1] = chordD[1];
      note[2] = chordD[2];  
      note[3] = chordD[3]; 
    }else if(distance >6 &&distance <9){
      note[0] = chordF[0];
      note[1] = chordF[1];
      note[2] = chordF[2];  
      note[3] = chordF[3]; 
      }else if (distance >9 && distance <12){
      note[0] = chordG[0];
      note[1] = chordG[1];
      note[2] = chordG[2];  
      note[3] = chordG[3];    
      }else if (distance >12 && distance <15){
      note[0] = chordA[0];
      note[1] = chordA[1];
      note[2] = chordA[2];  
      note[3] = chordA[3];    
      }else if (distance >15 && distance <18){
      note[0] = chordD2[0];
      note[1] = chordD2[1];
      note[2] = chordD2[2];  
      note[3] = chordD2[3];    
      }else if (distance >18 && distance <21){
      note[0] = chordF2[0];
      note[1] = chordF2[1];
      note[2] = chordF2[2];  
      note[3] = chordF2[3];    
      }else if (distance >21 && distance <24){
      note[0] = chordG2[0];
      note[1] = chordG2[1];
      note[2] = chordG2[2];  
      note[3] = chordG2[3];    
      }else if (distance >24 && distance <27){
      note[0] = chordA2[0];
      note[1] = chordA2[1];
      note[2] = chordA2[2];  
      note[3] = chordA2[3];    
      }else{
        
      note[0] = chordC[0];
      note[1] = chordC[1];
      note[2] = chordC[2];  
      note[3] = chordC[3];    
        }
 
    delay(10);
  
  }

void playTone(){

  //acending pattern
  if (loopMode ==1){
    tone(buzzer, note[0]); 
    delay(tempo);        
    noTone(buzzer);   
    delay(tempo);      
    
    tone(buzzer, note[1]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
    
    tone(buzzer, note[2]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
  
    tone(buzzer, note[3]); 
    delay(tempo);  
    noTone(buzzer);   
    //delay(tempo);
    
    }else if (loopMode==2){
    //descending pattern
    tone(buzzer, note[3]); 
    delay(tempo);        
    noTone(buzzer);   
    delay(tempo);      
    
    tone(buzzer, note[2]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
    
    tone(buzzer, note[1]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
  
    tone(buzzer, note[0]); 
    delay(tempo);  
    noTone(buzzer);   
    //delay(tempo);
    
    }else if (loopMode==3){
    //descending pattern
    tone(buzzer, note[0]); 
    delay(tempo);        
    noTone(buzzer);   
    delay(tempo);      
    
    tone(buzzer, note[3]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
    
    tone(buzzer, note[0]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
  
    tone(buzzer, note[3]); 
    delay(tempo);  
    noTone(buzzer);   
    //delay(tempo);
    
    }else if (loopMode==4){
    //descending pattern
    tone(buzzer, note[0]); 
    delay(tempo);        
    noTone(buzzer);   
    delay(tempo);      
    
    tone(buzzer, note[2]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
    
    tone(buzzer, note[1]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
  
    tone(buzzer, note[3]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
    
    }else if (loopMode==5){

    tone(buzzer, note[1]); 
    delay(tempo);        
    noTone(buzzer);   
    delay(tempo);      
    
    tone(buzzer, note[3]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
    
    tone(buzzer, note[2]); 
    delay(tempo);  
    noTone(buzzer);   
    delay(tempo);
  
    tone(buzzer, note[3]); 
    delay(tempo);  
    noTone(buzzer);   
    //delay(tempo);
    
    }else if (loopMode ==6){

      playMozzi();
      
      
    }else{
        
      loopMode=1;    
        
    }
  
  }

//returns distance int
int distanceChecker(){
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
  
  }
