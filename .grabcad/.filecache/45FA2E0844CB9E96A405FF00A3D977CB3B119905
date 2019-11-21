/*
        Default Robot Code for MiniFRC 2016

        Accepts input from the default MiniFRC Driver Station over Bluetooth
        to drive a robot with two motors: one for the left and one for the right.
        Requires the Adafruit Motor Shield v1 along with its corresponding library
        which can be found here: https://learn.adafruit.com/adafruit-motor-shield/library-install

        Default wiring for this program:
        * Bluetooth to analog 0 and 1 (corresponding to 14 and 15 in the code)
        * Left motor to motor port 1
        * Right motor to motor port 3

	Created 11/02/16
	By the TerrorBytes FRC 4561
*/

#include <SoftwareSerial.h>
#include <AFMotor.h>
#include <Servo.h>
const int leftMotorPort = 2; // Port number for the left motor on the Adafruit motor shield
const int rightMotorPort = 1; // Port number for the right motor on the Adafruit motor shield
const int bluetoothTxPin = 14; // Analog input for Bluetooth Tx pin
const int bluetoothRxPin = 15; // Analon input for Bluetooth Rx pin
const int intakeMotorPort = 4;
const int ArmServoPort = 9;
const int extendMotorPort = 3;

SoftwareSerial bluetooth(bluetoothTxPin, bluetoothRxPin); // Construct the bluetooth module
AF_DCMotor leftMotor(leftMotorPort); // Construct the left motor
AF_DCMotor rightMotor(rightMotorPort); // Construct the right motor
AF_DCMotor intakeMotor(intakeMotorPort);
AF_DCMotor extendMotor(extendMotorPort);
Servo armServo;

int turn = 0; // The direction the robot is turning. 1 = counterclockwise, -1 = clockwise
int velocity = 0; // The direction the robot is moving. 1 = forward, -1 = backwards
int autotrue = 0;
int intake = 0;
int arm = 0;
int extend = 0;

int currentArmPosition = 0;
//limits for arm:
int armMax = 90;//vertical
//90 units between armMin and armMax, roughly equal to degrees. 
int armMin = 0;//on the ground


void setup() {
  bluetooth.begin(9600); // Start accepting data over bluetooth
  driveRight(0); // Make sure the left motor is stopped
  driveLeft(0); // Make sure the right motor is stopped
  armServo.attach(ArmServoPort);

}

void loop() {
  while (bluetooth.available() > 0) { // Check if any data has been sent
    velocity = bluetooth.parseInt(); // Get the second integer from the data
    turn = bluetooth.parseInt(); // Get the first integer from the data
	  autotrue = bluetooth.parseInt();
	  intake = bluetooth.parseInt();
	  arm = bluetooth.parseInt();
	  extend = bluetooth.parseInt();
	  //currentArmPosition = armServo.read();//read current position of servo

	
    // If you've edited the Driver Station to send extra data, you should
    // to do more calls to bluetooth.parseInt() here to get the data.
    // For example, if you have a number that you want to set the speed of
    // a robot's arm to that you're sending from the Driver Station, you can
    // get it here by doing:
    // arm_speed = bluetooth.parseInt();
    // You should declare the arm_speed variable where the turn and velocity
    // variables are declared above the setup method.
    	
    if (bluetooth.read() == 'a') { // Failsafe to ensure data isn't desynced
      if (velocity == 0 && turn == 0) { // Break
        driveRight(0);
        driveLeft(0);
      }
      else if (velocity == 1 && turn == 0) { // Forward
        driveRight(255);
        driveLeft(255);
      }
      else if (velocity == -1 && turn == 0) { // Backward
        driveRight(-255);
        driveLeft(-255);
      }
      else if (velocity == 0 && turn == 1) { // Rotate left on the spot
        driveRight(255);
        driveLeft(-255);
      }
      else if (velocity == 0 && turn == -1) { // Rotate right on the sport
        driveRight(-255);
        driveLeft(255);
      }
      else if (velocity == -1 && turn == -1) { // Arc back right
        driveRight(-127);
        driveLeft(-255);
      }
      else if (velocity == -1 && turn == 1) { // Arc back left
        driveRight(-255);
        driveLeft(-127);
      }
      else if (velocity == 1 && turn == -1) { // Arc forward right
        driveRight(127);
        driveLeft(255);
      }
      else if (velocity == 1 && turn == 1) { // Arc forward left
        driveRight(254);
        driveLeft(127);
      }
	  if(autotrue == 1){
		  autonom();
	  }
	  if (intake == 1){
		    intakeRun(255);
	  }
	  else if (intake == -1){
		  intakeRun(-255);
	  }
	  else if(intake == 0){
		  intakeRun(0);
	  }
	  else if( arm == 1){
		  armLift(1);
	    }
	  else if(arm == -1){
		    armLift(-1);
      }
	  if(extend == 1){
			extendArm(1);
	  }
	  else if(extend == -1){
			extendArm(-1);
	  }
	  else if(extend == 0){
			extendArm(0);
	  }
    
    }
    // If servos and motors are updated too quickly, they sometimes don't work right.
    delay(50);
  }
}

/** 
  Sets the right motor to a power.
  
  @param motorSpeed what to set the motor power to.
*/
void driveRight(int motorPower) {
  rightMotor.run((motorPower >= 0) ? FORWARD : BACKWARD);
  rightMotor.setSpeed(abs(motorPower)); 
}

/** 
  Sets the left motor to a power.
  
  @param motorSpeed what to set the motor power to.
*/
void driveLeft(int motorPower) {
  leftMotor.run((motorPower >= 0) ? FORWARD : BACKWARD);
  leftMotor.setSpeed(abs(motorPower));
}
void stop(){
	leftMotor.setSpeed(0);
	rightMotor.setSpeed(0);
}

void autonom(){
	driveRight(255);
	driveLeft(255);
	delay(10000);
	stop();
}

void intakeRun(int motorPower){
    intakeMotor.run((motorPower >= 0) ? FORWARD : BACKWARD);
    intakeMotor.setSpeed(abs(motorPower));
}

void extendArm(int motorPower){
    extendMotor.run((motorPower >= 0) ? FORWARD : BACKWARD);
    extendMotor.setSpeed(abs(motorPower));
}

void armLift(int motorPower){
	//how to servos?
	//make sure to add soft limits
	//how to find position of servo
 if(motorPower==1){
  armServo.write(45);
 }else if (motorPower==-1){
  armServo.write(0);
 }

}

