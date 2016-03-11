#include <NewPing.h>

//The third argument is the distance range within which the sensor reads
NewPing sideSonar(10, 2, 140);
NewPing frontSonar(5, 6, 200);

// Motor A - right motor
// Motor B - left motor
// Initialixing motor pins
const int brakePinA = 9;
const int brakePinB = 8;
const int motorADir = 12;
const int motorBDir = 13;
const int motorAVal = 3;
const int motorBVal = 11;

// defines variables
long distF, distS;
//int margin = 0;
int vel = 175; // set the speed here
int distToBase = 0;
bool onGround = false; 
bool start = true;
bool doneDrop = false;
bool doneAlignment = false;
bool turned = false;
bool done = false;

void setup() {
  Serial.begin(9600); // Starts the serial communication

  //Setup Channel A
  pinMode(motorADir, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakePinA, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(motorBDir, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakePinB, OUTPUT);  //Initiates Brake Channel A pin 
}

void loop() {
     // Wait for robot drop
    if(!start){
      delay(10000);
      start = true;
    }
    if(onGround) {
        stopMotors();
        search();
    }
    else {
        checkForGround();
    }
}

void checkForGround() {
    if(start && !doneDrop){
      while (getAverageReadingF() >= 20 || getAverageReadingF() == 0) { }
      while(getAverageReadingF() <= 30){
          drive();
          delay(10);
      }  
      doneDrop = true;
    }
    if(start && doneDrop){
      //reverse if robot goes too far 
      while(getAverageReadingF() <= 150 || getAverageReadingF() == 0){
        vel = 100;
        reverse();
      } 
      stopMotors();
      delay(1000);
      onGround = true;
   }
}


double getAverageReadingF() {
    double total = 0;

    for (int i=0; i<5; i++) {
        total +=  frontSonar.ping_cm();
    }
    return total/5;
}

void search() {
    vel = 200;
    if (!doneAlignment && !turned) {
        if(sideDetects()){
          doneAlignment = true;
          distToBase = distS;
        }
    }
    if (!doneAlignment && !turned){
        drive();  
    }
    if(doneAlignment && !turned){
        stopMotors();
        delay(1000);
        turnLeft();
        delay(200);
        while(!frontDetects()){
          turnLeft(); 
          delay(10);
        }
        turned = true;
        stopMotors();
        delay(1000);
    }
    if(turned && !done){
      vel = 250;
      while(frontSonar.ping_cm() > 2){
        drive();  
      }
      if(frontSonar.ping_cm() <= 2){
          done = true;
      }
    }
    if(done){
      stopMotors();
      delay(5000);  
    }
}

bool sideDetects() {
    distS = sideSonar.ping_cm();
    return distS != 0;
}

bool frontDetects() {
    return frontSonar.ping_cm() <= distToBase && frontSonar.ping_cm() != 0; 
}

void stopMotors(){
  digitalWrite(brakePinA, HIGH);
  digitalWrite(brakePinB, HIGH);  
}

void moveRMtr(bool forward){
  if (forward){
    digitalWrite(motorADir, HIGH); 
  }
  else{
    digitalWrite(motorADir, LOW);  
  }
  digitalWrite(brakePinA, LOW);   //Disengage the Brake for Channel A
  analogWrite(motorAVal, vel);    //Spins the motor on Channel A at half speed
}

void moveLMtr(bool forward){
  if (forward){
    digitalWrite(motorBDir, HIGH); 
  }
  else{
    digitalWrite(motorBDir, LOW);  
  }
  digitalWrite(brakePinB, LOW);   //Disengage the Brake for Channel A
  analogWrite(motorBVal, vel);    //Spins the motor on Channel A at half speed
}

void drive() {
   //Motor A forward
   moveRMtr(true);
  
  //Motor B forward 
   moveLMtr(true);
}

void turnLeft() {
  // Left motor moves backward
  moveLMtr(false);
  // Right motor moves forward
  moveRMtr(true);
}

void turnRight() {
  //Right motor moves backward
  moveRMtr(false);
  //Left motor moves forward
  moveLMtr(true);
}

void reverse() {
  moveRMtr(false);
  moveLMtr(false); 
}

