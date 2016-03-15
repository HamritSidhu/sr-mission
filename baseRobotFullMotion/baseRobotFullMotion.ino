#include <Servo.h>

Servo armServo;  
Servo pulleyServo; 

//button variables
const int buttonPin1 = 2;     // the number of the pushbutton pin
const int buttonPin2 = 4;     // the number of the pushbutton pin
const int buttonStart = 5;    // Start Program Pin

// Flags
bool startDrive = false;
bool donePulley = false;
bool doneArm = false;
bool doneDrive = false;
bool driving = false;

// Speed
int motorSpeed = 100;
int pulleySpeed = 0;

void setup() {
   //Setup Channel A motor
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B motor
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonStart,  INPUT);

  armServo.attach(7);  // attaches the servo on pin 9 to the servo object
  armServo.write(180); // Set arm to 180 position,
  pulleyServo.attach(10);  // attaches the servo on pin 13 to a servo object
  pulleyServo.write(100); // Stop pulley motor

}

void loop(){
  if(!startDrive)
      waitForStart();
  if (!doneDrive && startDrive)
    drive();
  if (!doneArm && doneDrive && startDrive)
    arm();
  if (!donePulley && doneArm && doneDrive && startDrive)
    pulley();
}

void waitForStart() {
  int state = digitalRead(buttonStart);
  if (state == HIGH) {
    startDrive = true;
  }
}

void driveChannelA() {
  //Motor A forward @ full speed
  digitalWrite(12, HIGH);  //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, motorSpeed);
}

void stopChannelA() {
  digitalWrite(9, HIGH);  //Engage the Brake for Channel A
}

void driveChannelB() {
  //Motor B forward @ full speed
  digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, motorSpeed);   //Spins the motor on Channel B at half speed
}

void stopChannelB() {
  digitalWrite(8, HIGH);  //Engage the Brake for Channel B
}

void driveBoth() {
  if (!driving) {
    driveChannelA();
    driveChannelB();
  }
  driving = true;
}

void stopBoth() {
  if (driving) {
    stopChannelA();
    stopChannelB();
  }
  driving = false;
}

void drive() {
  int state1 = digitalRead(buttonPin1);
  int state2 = digitalRead(buttonPin2);
  // Reached the wall
  if (digitalRead(buttonPin1) == HIGH || digitalRead(buttonPin2) == HIGH && driving)
  {
    stopBoth();
    doneDrive = true;
    delay(2000);
  }
  else
    driveBoth();
}

void arm() {
  armServo.write(5); 
  doneArm = true;
}

void pulley() {
  delay(1000);
  driveBoth();
  delay(1000);
  stopBoth();
  delay(3000);   
  pulleyServo.write(pulleySpeed); 
  delay(1000);
  armServo.write(20);
  delay(12000);
  pulleyServo.write(100);
  donePulley = true; 
}

 
