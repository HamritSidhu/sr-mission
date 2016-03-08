#include <NewPing.h>

//The third argument is the distance range within which the sensor reads
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
long distF, distS;
int vel = 100; // set the speed here
bool onGround = false;
bool doneAlignment = false;
bool turned = false;
bool atDestination = false;

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
    //if (onGround) {
        search();
    //}
    //else {
    //    checkForGround();
    //}
}

void search() {
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

//    keep checking if destination is reached or not
//    if (turned && !atDestination) {
//        checkForDestination();  
//    }
//    
//    Once destination is reached, stop robot from moving
//    if (atDestination) {
//        stopMotors();
//        delay(1000)
//        while (1) { }
//    }
      
    drive();
    delay(1000);
}

void checkForGround() {
    onGround = targetReached();  
}

void checkForDestination() {
    atDestination = targetReached();  
}

bool targetReached() {
    //need to adjust this function so it returns true when we hit something
    distF = frontSonar.ping_cm();
    return distF == 0;
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

