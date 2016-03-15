#include <NewPing.h>
#include <QuickStats.h>

//The third argument is the distance range within which the sensor reads
NewPing sideSonar(10, 2, 120);
NewPing frontSonar(5, 6, 120);

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
long distF, distS, wallDistance;
int numReadings = 20;
float readings[20];
int vel = 100; // set the initial speed here
int distToBase = 0;
bool onGround = false; 
bool start = false;
bool doneDrop = false;
bool doneAlignment = false;
bool turned = false;
bool done = false;
int margin = 4;

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
        delay(8000);
        start = true;
    }
    if(onGround) {
        search();
    }
    else {
        checkForGround();
    }
}

void checkForGround(){
    while(distFront() > 30 || distFront() <= 0){ }
    while(distFront() < 30){
      drive();
      delay(50);
    }

    stopMotors();
    delay(1000);
    if(distFront() > 50){
      vel = 100;
      reverse();
      delay(2000);
      stopMotors();
      delay(1000);
      onGround = true;
    }

}

void search() {
    vel = 200;
    if (!doneAlignment && !turned) {
        if(sideDetects()){
          doneAlignment = true;
          distToBase = distS;
          Serial.println(distToBase);
        }
        drive();
    }
      
    if(doneAlignment && !turned){
        stopMotors();
        delay(1000);
        vel = 80;
        reverse();
        delay(200);
        turnLeft();
        delay(200);
        
        while(!frontDetects()){
          turnLeft(); 
          delay(5);
        }
        turned = true;
        stopMotors();
        delay(1000);
    }
    if(turned && !done){
        vel = 250;
        
        while(distFront() > 5){
          drive();  
        }

        distF = distFront();
        if(distF <= 5){
            done = true;
            //delay(400);
        }
       
    }
    if(done){
        stopMotors();
        while(1) { } 
    }
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
bool frontDetects() {
    distF = distFront();
    return distF <= distToBase + margin && distF >= distToBase - margin;
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

//Median Filter
float getMedian(NewPing sensor) {
    for (int i=0; i<numReadings; i++) {
        readings[i] = sensor.ping_cm();
    }
    return stats.median(readings, numReadings);   
}

