#include <Servo.h>

Servo armServo;  
Servo pulleyServo; 

//button variables
const int buttonPin1 = 7;     // the number of the pushbutton pin
const int buttonPin2 = 2;     // the number of the pushbutton pin
const int buttonStart = 22;    // Start Program Pin

// motor one
int enA = 8;
int in1 = 9;
int in2 = 10;
// motor two
int enB = 13;
int in3 = 12;
int in4 = 11;

// Flags
bool startDrive = false;
bool donePulley = false;
bool doneArm = false;
bool doneDrive = false;
bool offBoard = false;


// Speed
int motorSpeed = 40;
int pulleySpeed = 0;

void setup() {
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonStart,  INPUT);

  armServo.attach(33);
  armServo.write(140); // Set arm to 180 position,
  pulleyServo.attach(31);
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
  // Motor A forward @ full speed
  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed, possible range 0~255
  analogWrite(enA, motorSpeed);
}

void stopChannelA() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW); 
}

void driveChannelB() {
  // Motor B forward @ full speed
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed, possible range 0~255
  analogWrite(enB, motorSpeed);
}

void stopChannelB() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void driveBoth() {
    driveChannelA();
    driveChannelB();
}

void stopBoth() {
    stopChannelA();
    stopChannelB();
}

void drive() {
  // Reached the wall
  if (digitalRead(buttonPin1) == HIGH || digitalRead(buttonPin2) == HIGH)
  {
//    stopBoth();
    doneDrive = true;
//    delay(500);
  }
  else {
    driveBoth();
    if (!offBoard) {
      delay(3600);
      offBoard = true;
    }
    // acceleration
    if (motorSpeed < 200) {
      motorSpeed += 10;
      delay(100);
    }
  }
}

void arm() {
  armServo.write(5); 
  doneArm = true;
}

void pulley() {
  delay(500);
  // Move forward by a bit
  driveBoth();
  delay(1000);
  stopBoth();
  delay(1000); 
  // Drop search robot  
  pulleyServo.write(pulleySpeed); 
  delay(1000);
  // Pull back arm
  armServo.write(20);
  delay(15000);
  pulleyServo.write(100);
  donePulley = true; 
}

 
