#include <Servo.h>

Servo armServo;  
Servo pulleyServo; 

int pos = armServo.read();    // variable to store the servo position

//button variables
const int buttonPin1 = 2;     // the number of the pushbutton pin
const int buttonPin2 = 4;     // the number of the pushbutton pin

// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status

bool donePulley = false;
bool doneArm = false;
bool doneDrive = false;

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

  armServo.attach(7);  // attaches the servo on pin 9 to the servo object
  armServo.write(180); // Set arm to 180 position
  pulleyServo.attach(10);  // attaches the servo on pin 13 to a servo object
  pulleyServo.write(100); // Stop pulley motor

}

void loop(){
  if (!doneDrive) {
    drive();
  }
  if (!doneArm && doneDrive) {
    arm();
  }
  if (!donePulley && doneArm && doneDrive){
    pulley();
  }
}

void drive() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  //We at the wall
  if (buttonState1 == HIGH && buttonState2 == HIGH)
  {
    digitalWrite(9, HIGH);  //Engage the Brake for Channel A
    digitalWrite(8, HIGH);  //Engage the Brake for Channel B
    doneDrive = true;
    delay(2000);
  }

  //Motor A
  if (buttonState1 == LOW) 
  {
    //Motor A forward @ full speed
    digitalWrite(12, HIGH);  //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    analogWrite(3, 30);    //Spins the motor on Channel A at half speed
  }
  else
    digitalWrite(9, HIGH);  //Engage the Brake for Channel A

  //Motor B
  if (buttonState2 == LOW) 
  {
    //Motor B forward @ full speed
    digitalWrite(13, HIGH); //Establishes forward direction of Channel B
    digitalWrite(8, LOW);   //Disengage the Brake for Channel B
    analogWrite(11, 30);   //Spins the motor on Channel B at half speed
  }
  else
    digitalWrite(8, HIGH);  //Engage the Brake for Channel B
}

void arm() {
  armServo.write(20);
  delay(1000);
  doneArm = true;
}

void pulley() {   
  pulleyServo.write(45); 
  delay(7000);
  pulleyServo.write(100);
  donePulley = true; 
}

 
