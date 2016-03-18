#include <NewPing.h>
#include <QuickStats.h>

//The third argument is the distance range within which the sensor reads
NewPing sideSonar(10, 2, 120);
NewPing frontSonar(5, 6, 250);

QuickStats stats; 

// Motor A - right motor
// Motor B - left motor
// Initializing motor pins
const int brakePinA = 9;
const int brakePinB = 8;
const int motorADir = 12;
const int motorBDir = 13;
const int motorAVal = 3;
const int motorBVal = 11;

// defines variables
long distF, distS;
unsigned long startTime;
int numReadings = 20;
float readings[20];
int vel = 100; // set the initial speed here
int distToBase = 0;
bool onGround = false; 
bool start = true;
bool doneAlignment = false;
bool turned = false;
bool done = false;
int margin = 10;

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
    if(!start) {
        delay(11500);
        start = true; 
    }
    
    if(onGround) {
       search();
    }
    else {
        checkForGround();
    }
}

void checkForGround() {
    // wait until 30 cm away from ground 
    while(distFront() > 30 || distFront() <= 0){ }
    distF = distFront();

    // move wheels forward until ground is hit
    while(distF < 30) {
        drive(50);
        distF = distFront();
    }
    stopMotors(1000);
    distF = distFront();
    
    if(distF > 50) {
        vel = 120;
        //reverse motors to align wheels against wall and straight forward
        reverse(1250);
        stopMotors(1000);
        onGround = true;
    }
}

void search() {
    vel = 100;

    //check for side sensor alignment with destination
    if (!doneAlignment && !turned) {
        if(sideDetects()) {
            doneAlignment = true;
        }
        distF = distFront();
        if (distF <= 25 && distF != 0) {
            turnNinety();
            stopMotors(500);
            reverse(300);
            stopMotors(250);
        }
        drive(0);
    }

    // rotate search robot towards the base
    if(doneAlignment && !turned){
        stopMotors(1000);
        vel = 120;
  
        turnNinety();
        turned = true;
        stopMotors(1000);
    }

    // move search robot forward until it hits the base or a maximum of 3 seconds
    if(turned && !done){
        vel = 250;
        distF = distFront();
        startTime = millis();
        while((distF > 20 || distF == 0) && (millis() - startTime) < 3000){
            drive(0);  
            distF = distFront();
        }
        done = true;  
    }

    // once at destination, stop
    if(done){
        drive(700);
        stopMotors(0);
        while(1) { } 
    }
}


void turnNinety() {
    turnLeft(375);
}

int distFront(){
  return getMedian(frontSonar);
}

int distSide(){
  return getMedian(sideSonar);  
}

bool sideDetects() {
    distS = distSide();
    return distS != 0;
}

void stopMotors(int delayT) {
  digitalWrite(brakePinA, HIGH);
  digitalWrite(brakePinB, HIGH); 
  if(delayT != 0 ){
      delay(delayT);
  }
}

void moveRMtr(bool forward) {
  digitalWrite(brakePinA, LOW);   //Disengage the Brake for Channel A
  if (forward){
    digitalWrite(motorADir, HIGH); 
  }
  else{
    digitalWrite(motorADir, LOW);  
  }
  analogWrite(motorAVal, vel);    //Spins the motor on Channel A at half speed
}

void moveLMtr(bool forward){
  digitalWrite(brakePinB, LOW);   //Disengage the Brake for Channel A
  if (forward){
    digitalWrite(motorBDir, HIGH); 
  }
  else{
    digitalWrite(motorBDir, LOW);  
  }
  analogWrite(motorBVal, vel);    //Spins the motor on Channel A at half speed
}

void drive(int delayT) {
   //Motor A forward
   moveRMtr(true);
  
  //Motor B forward 
   moveLMtr(true);
   if(delayT != 0 ){
      delay(delayT);
   }
}

void turnLeft(int delayT) {
  // Left motor moves backward
  moveLMtr(false);
  // Right motor moves forward
  moveRMtr(true);
  if(delayT != 0 ){
      delay(delayT);
  }
}

void reverse(int delayT) {
  moveRMtr(false);
  moveLMtr(false); 
  if(delayT != 0 ){
      delay(delayT);
  }
}

//Median Filter
float getMedian(NewPing sensor) {
    for (int i=0; i<numReadings; i++) {
        readings[i] = sensor.ping_cm();
    }
    return stats.median(readings, numReadings);   
}

