#include <NewPing.h>

NewPing sideSonar(10, 2, 10);
NewPing frontSonar(5, 6, 3500);

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
long durF, durS; 
long distF, distS;
bool doneAlignment = false;
bool turned = false;

void setup() {
  Serial.begin(9600); // Starts the serial communication

  //Setup Channel A
  pinMode(motorADir, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakePinA, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(motorBDir, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakePinB, OUTPUT);  //Initiates Brake Channel A pin 
}

void loop(){
    //read front
    //once ground is reached, move straight
    if (!doneAlignment && !turned) {
        doneAlignment = destinationDetected();  
    }
    
    if (doneAlignment && !turned) {
        stopMotors();
        turnLeft();
        delay(1000); 
        stopMotors();
        delay(1000);
        turned = true;  
    }
    drive();
    delay(1000);
    //read side, once destination detected stop motors
    //turn right
    //move straight
    //once destination is reached, stop motors
}

bool destinationReached() {
    distF = frontSonar.ping_cm();
    return distF < 8 && distF > 3000;
}

bool destinationDetected() {
    distS = sideSonar.ping_cm();
    return distS != 0;
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
  analogWrite(motorAVal, 250);    //Spins the motor on Channel A at half speed
}

void moveLMtr(bool forward){
  if (forward){
    digitalWrite(motorBDir, HIGH); 
  }
  else{
    digitalWrite(motorBDir, LOW);  
  }
  digitalWrite(brakePinB, LOW);   //Disengage the Brake for Channel A
  analogWrite(motorBVal, 250);    //Spins the motor on Channel A at half speed
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

