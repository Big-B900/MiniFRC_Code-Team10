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
	Edited  11/03/16
	By Bram Lovelace
*/
//pinMode(LED_BUILTIN, OUTPUT);
#include <SoftwareSerial.h>
#include <AFMotor.h>
//change all of the port numbers to make sure they are the right ones
const int leftMotorPort = 1; // Port number for the left motor on the Adafruit motor shield
const int rightMotorPort = 2; // Port number for the right motor on the Adafruit motor shield
const int bluetoothTxPin = 14; // Analog input for Bluetooth Tx pin
const int bluetoothRxPin = 15; // Analon input for Bluetooth Rx pin
//adding my ports,
//const int intakeMotorPort = //??
//const int extendMotorPort = //??
//const int armMotorPort = //?? - servo might need something different


SoftwareSerial bluetooth(bluetoothTxPin, bluetoothRxPin); // Construct the bluetooth module
AF_DCMotor leftMotor(leftMotorPort); // Construct the left motor
AF_DCMotor rightMotor(rightMotorPort); // Construct the right motor
/*constructing my motors
AF_DCMotor extendMotor(extendMotorPort);
AF_DCMotor intakeMotor(intakeMotorPort);
AF_DCMotor armMotor(armMotorPort); //look up how to create a servo motor
*/

int turn = 0; // The direction the robot is turning. 1 = counterclockwise, -1 = clockwise
int velocity = 0; // The direction the robot is moving. 1 = forward, -1 = backwards
int intake = 0; //intake 1 = in, -1 =out
int extend = 0; //1=extend, -1 = retract
int arm = 0; //arm rotation, 1 = up, -1 = down

void setup() {
  bluetooth.begin(9600); // Start accepting data over bluetooth
  driveRight(0); // Make sure the left motor is stopped
  driveLeft(0); // Make sure the right motor is stopped
}

// is loop like main method?
void loop() {
  while (bluetooth.available() > 0) { // Check if any data has been sent
    
    velocity = bluetooth.parseInt(); // Get the first integer from the data
    turn = bluetooth.parseInt(); // Get the second integer from the data
	  intake = bluetooth.parseInt(); 
    extend = bluetooth.parseInt();
    arm = bluetooth.parseInt();
    // If you've edited the Driver Station to send extra data, you should
    // to do more calls to bluetooth.parseInt() here to get the data.
    // For example, if you have a number that you want to set the speed of
    // a robot's arm to that you're sending from the Driver Station, you can
    // get it here by doing:
    // arm_speed = bluetooth.parseInt();
    // You should declare the arm_speed variable where the turn and velocity
    // variables are declared above the setup method.
	
	//testing an auto sequence - probably needs another button to toggle false when start, when pressed set to true, run certain time, then go to teleop, or press again for teleop
	/*bool autonomus = true;
	int i = 0;
	if (autonomus){
		while (i<500){
			driveLeft(255);
			driveRight(255);
			i++;
		}
		autonomus = false;
	}*/
		
		
		
		
		
    if (bluetooth.read() == 'a') { // Failsafe to ensure data isn't desynced
	  //why not use a case structure?
      for (int i=0;i<3;i++){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
      }
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
        driveRight(255);
        driveLeft(127);
      }
	  //change the parameters to send to the functions if they are too strong or too weak
    /*
	  if (intake ==1){//intake in
		  intake(128);
	  }
	  else if (intake == -1){//intake out
		  intake(-128);
	  }
	  if (extend == 1){//extend arm
		  armExtend(255);
	  }
	  else if (extend == -1){//retract arm
		  armExtend(-255);
	  }
	  if (arm == 1){//arm up
		  armLift(128);
	  }
	  else if (arm == -1){//arm down
		  armLift(-128);
	  }
	 */
    }
  }
  // If servos and motors are updated too quickly, they sometimes don't work right.
  delay(50);
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

/**
   Moves arm up and down
* /
void armLift(int speed) {
  armMotor.run((speed >= 0) ? FORWARD : BACKWARD);//initiate motor
                             //make sure it is the right direction
  armMotor.setSpeed(abs(speed)); //I assume same commands for servos and stuff?
}

/**
  extends the arm 
* /
void armExtend(int speed){
	extendMotor.run((speed>=0) ? FORWARD : BACKWARD);
	extendMotor.setSpeed(abs(speed));
}

/**
   runs intake
* /
void intake (int speed){
  intakeMotor.run((speed >= 0) ? FORWARD : BACKWARD);
  intakeMotor.setSpeed(abs(speed));
}*/

