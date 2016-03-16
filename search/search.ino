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
long distF, distS;
unsigned long startTime;
int numReadings = 20;
float readings[20];
int vel = 100; // set the initial speed here
int distToBase = 0;
bool onGround = false; 
bool start = false;
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
        delay(12000);
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
    distF = distFront();
    while(distF > 30 || distF <= 0){
        distF = distFront();
    }

    while(distF == 0 || distF > 15) {
        distF = distFront();
    }
    
    drive();
    delay(4000);
    
    stopMotors();
    delay(1000);
    
    reverse();    
    delay(1500);
    
    stopMotors();
    delay(1000);
    
    onGround = true; 
}

void search() {
    vel = 100;

    if (!doneAlignment && !turned) {
        if(sideDetects()){
          doneAlignment = true;
          distToBase = distS;
        }
        drive();
    }
      
    if(doneAlignment && !turned){
        delay(50);
        stopMotors();
        delay(1000);
        vel = 100;
        if (distToBase >= 50) {
            margin = 6;
        }
        else {
            margin = 10;  
        }    
        startTime = millis();
        turnLeft();
        delay(310);

        while(!frontDetects() && millis() - startTime <= 525) {
          turnLeft(); 
          delay(5);
        }
        turned = true;
        stopMotors();
        delay(1000);
    }
    
    if(turned && !done){
        vel = 250;
        distF = distFront();
        startTime = millis();
        while((distF > 20 || distF == 0) && (millis() - startTime) < 3000){
          drive();  
          distF = distFront();
        }
        done = true;  
    }
    
    if(done){
        drive();
        delay(700);
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
    return distF <= distToBase + margin && distF >= distToBase - margin && distF != 0;
}

void stopMotors() {
  digitalWrite(brakePinA, HIGH);
  digitalWrite(brakePinB, HIGH);  
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

