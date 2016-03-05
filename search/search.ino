// Motor A - right motor
// Motor B - left motor
int counter = 5;

const int brakePinA = 9;
const int brakePinB = 8;
const int motorADir = 12;
const int motorBDir = 13;
const int motorAVal = 3;
const int motorBVal = 11;
// defines pins numbers
// side sensor
const int trigPinS = 10;
const int echoPinS = 2;

//front sensor
const int trigPinF = 5;
const int echoPinF = 6;

const int speedN = 250;
// defines variables
long durF, durS;
long distF, distS;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  //Setup Channel A
  pinMode(motorADir, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakePinA, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(motorBDir, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakePinB, OUTPUT);  //Initiates Brake Channel A pin 
  
  pinMode(trigPinS, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinS, INPUT); // Sets 
  pinMode(trigPinF, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinF, INPUT); // Sets 
}

void loop(){
 // Blue bottom, grey at top
  if (counter <= 5) {
     distF = distFront();
    Serial.println(distF);
    if(distF > 8 && distF < 3000){
      moveStraight();
      delay(50);  
      stopMotor();
      delay(200);
    }
    else{
      turnRight();
      delay(25);
    }
  }
  else {
    stopMotor();
    }

  counter += 1;

  
 /* moveStraight();
  delay(3000);

  stopMotor();
  delay(1000);

  turnRight();
  delay(1000);

  stopMotor();
  delay(1000);

  turnLeft();
  delay(1000);

  stopMotor();
  delay(1000);*/
}

void stopMotor(){
  digitalWrite(brakePinA, HIGH);
  digitalWrite(brakePinB, HIGH);  
}

long distSide(){
     // Clears the trigPin
    digitalWrite(trigPinS, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPinS, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinS, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long dur = pulseIn(echoPinS, HIGH);
    // Calculating the distance
    long dist= dur*0.034/2;
    return dist;
}

long distFront(){
     // Clears the trigPin
    digitalWrite(trigPinF, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPinF, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinF, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long dur = pulseIn(echoPinF, HIGH);
    // Calculating the distance
    long dist= dur*0.034/2;
    return dist;
}

void moveRMtr(bool forward){
  if (forward){
    digitalWrite(motorADir, HIGH); 
  }
  else{
    digitalWrite(motorADir, LOW);  
  }
  digitalWrite(brakePinA, LOW);   //Disengage the Brake for Channel A
  analogWrite(motorAVal, speedN);    //Spins the motor on Channel A at half speed
}

void moveLMtr(bool forward){
  if (forward){
    digitalWrite(motorBDir, HIGH); 
  }
  else{
    digitalWrite(motorBDir, LOW);  
  }
  digitalWrite(brakePinB, LOW);   //Disengage the Brake for Channel A
  analogWrite(motorBVal, speedN);    //Spins the motor on Channel A at half speed
}

void moveStraight() {
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

