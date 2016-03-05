#include <NewPing.h>

NewPing sideSonar(10, 2, 40);
NewPing frontSonar(5, 6, 40);

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
    if (!doneAlignment && !turned) {
        doneAlignment = sideDetects();  
    }
    
    if (doneAlignment && !turned) {
        stopMotors();
        turnLeft();
        while (!frontDetects()) {
            delay(1); 
        }
        stopMotors();
        delay(1000);
        turned = true;  
    }
    drive();
    delay(1000);
}

bool destinationReached() {
    distF = frontSonar.ping_cm();
    return distF < 8 && distF > 3000;
}

bool sideDetects() {
    distS = sideSonar.ping_cm();
    return distS != 0;
}

bool frontDetects() {
    distF = frontSonar.ping_cm();
    return distF != 0;  
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
  analogWrite(motorAVal, 100);    //Spins the motor on Channel A at half speed
}

void moveLMtr(bool forward){
  if (forward){
    digitalWrite(motorBDir, HIGH); 
  }
  else{
    digitalWrite(motorBDir, LOW);  
  }
  digitalWrite(brakePinB, LOW);   //Disengage the Brake for Channel A
  analogWrite(motorBVal, 100);    //Spins the motor on Channel A at half speed
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

