/**
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
const int intakeMotorPort = 3; // change if different port
const int extendMotorPort = 4; // change if other port. 
const int ArmServoPort = 9; //no idea what the correct number is...

/*how to do servo*/


SoftwareSerial bluetooth(bluetoothTxPin, bluetoothRxPin); // Construct the bluetooth module
AF_DCMotor leftMotor(leftMotorPort); // Construct the left motor
AF_DCMotor rightMotor(rightMotorPort); // Construct the right motor
AF_DCMotor intakeMotor(intakeMotorPort);
AF_DCMotor extendMotor(extendMotorPort);
Servo armServo;
armServo.attach(ArmServoPort);//attach the servo on ArmServoPort to the armServo object


int turn = 0; // The direction the robot is turning. 1 = counterclockwise, -1 = clockwise
int velocity = 0; // The direction the robot is moving. 1 = forward, -1 = backwards
int intake = 0;//The direction of the intake 1:in, 0:off, -1:out
int extend = 0;//the direction of the extend motors. 1:extend, 0:off, -1:retract
int armCommand = 0; //the direction the arm servo is moving
int autonomous = 0;//autonomous?
//the next three are for auto, need to figure out how to read these 
int defence = 0;//he defence the robot is in front of 0: Low bar, 1: rough terrain, 2: moat, 3: ramparts, 4:cheval, 5:Portcullis, 6:any other one I can't do 
int position = 0;//position which position the robot is in 0-4, from low bar towards secret passage
int goal = 0;//goal 1 yes, 0 no.

int currentArmPosition=0;
int autoDone = 0;

//limits for arm:
int armMax = 90;//vertical
//90 units between armMin and armMax, roughly equal to degrees. 
int armMin = 0;//on the ground



/* start robot with the arm on the ground, then turn on. Have something to change it when it moves up, */


void setup() {
  bluetooth.begin(9600); // Start accepting data over bluetooth
  driveRight(0); // Make sure the left motor is stopped
  driveLeft(0); // Make sure the right motor is stopped
}

void loop() {
  while (bluetooth.available() > 0) { // Check if any data has been sent
    velocity = bluetooth.parseInt(); // Get the second integer from the data
    turn = bluetooth.parseInt(); // Get the first integer from the data
	/*send these from driver station*/
	intake = bluetooth.parseInt();//The direction of the intake 1:in, 0:off, -1:out
	extend = bluetooth.parseInt();
	armCommand = bluetooth.parseInt(); //the direction the arm servo is moving
	autonomous = bluetooth.parseInt();//autonomous?
	//the next three are for auto, need t figure out how to send these 
	defence = bluetooth.parseInt();
	position = bluetooth.parseInt();
	goal = bluetooth.parseInt();
	currentArmPosition = armServo.read();//read current position of servo
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
        driveRight(255);
        driveLeft(127);
      }
	  intake(intake*255);// the intake at whichever speed it needs to be, always updating so no if - also simplifys stuff
	  extend(extend*255);
	  armlift(armCommand);
	  if(autonomous==1&&autoDone==0){//perform autonomous if it is activated, and hasnt done it before.
		  autonomus(defence, position, goal);
	  }
	  if(currentArmPosition>90){//soft limit to make sure it doesnt go past vertical.
		  arm(90);
	  }
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

void intake(int motorPower) {
  intakeMotor.run((motorPower >= 0) ? FORWARD : BACKWARD);
  intakeMotor.setSpeed(abs(motorPower));
}



/**used for manual arm control*/
void armlift(int motorPower){
	//how to servos?
	//make sure to add soft limits
	//how to find position of servo
	currentArmPosition = armServo.read();
	if (currentArmPosition<(armMin+2)&&motorPower<0){//if trying to go down, set to lowest possible, if too close
		armServo.write(armMin);
	}else if (currentArmPosition>(armMax-2)&& motorPower>0){//if trying to go too high, set to vertical 
		armServo.write(armMax);
	}else{
	armServo.write(armServo.read()+motorPower);
	}
}
/**used for automated tasks that include moving the arm , also maybe commands?
@param target the goal rotation of the servo. 0 is down on the floor, 90 is vertical. 
*/
void arm(int target) {
	armServo.write(target);//mpve servo to target
}

void extend(int motorPower){
	extendMotor.run((motorPower >= 0) ? FORWARD : BACKWARD);
	extendMotor.setSpeed(abs(motorPower));
}


void stop(){
	leftmotor.setSpeed(0);
	rightMotor.setSpeed(0);
}





/**
 autonomous
 how do I send all of the data?
 @param defense the defence the robot is in front of 0: Low bar, 1: rough terrain, 2: moat, 3: ramparts, 4:cheval, 5:Portcullis, 6:any other one I can't do 
 @param position which position the robot is in 0-4, from low bar towards secret passage
 @param goal 1 yes, 0 no.
*/
void autonomous (int defence, int position, int goal){
	
	switch(defence){
		case 0: //low bar, left goal
		{
			arm(armMin);//lower arm to minimum... change to a little higher if gets cautght on defences
			driveLeft(255);
			driveRight(255);
			delay(5000); //change this as the distance it goes, in milliseconds.
			//this is for the possible low goal
			if(goal==1){//check if low goal 
			//if differnt position, switch based on it
				driveRight(-255);//turn toward tower
				delay(1500); //change this to make sure right angle
				driveRight(255);//go towards tower
				delay(2000);//change for distance
				stop();
				intake(-1);// output of intake
				delay(500)//how long does it take for a ball to get out?
				intake(0);//stop intake
			}else{
				stop();
			}
			
			
		}
		break;//did I do this right?
		case 1: //rough terrain 
		{
			//not sure if this will work
			driveLeft(255);
			driveRight(255);
			//this is for the possible low goal
			if(goal==1){//check if low goal 
			switch(position){//only if I do low goal
				case 1: //closest to low bar, left goal
				{
				//if differnt position, switch based on it
				delay(5500); //change this as the distance it goes, in milliseconds
				driveRight(-255);//turn toward tower
				delay(1500); //change this to make sure right angle
				driveRight(255);//go towards tower
				delay(2000);//change for distance
				stop();
				intake(-1);// output of intake*/
				delay(500)//how long does it take for a ball to get out?
				intake(0);//stop intake*/
				//if there is time, turn around and do stuff

				}
				
				break;
				case 2://middle goal
				{
					delay(4000);//change for the distance it goes
					driveRight(-255);//turn right
					delay(750);//wait to turn correct angle
					driveRight(255);//drive straight
					delay(500);//not long, just long enough to be lined up w/ low goal 
					driveLeft(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveLeft(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 3://middle defence(little to the right ), middle goal
				{
					delay(4000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(333);//not long, just long enough to be lined up w/ low goal 
					driveRight(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveRight(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 4://closest to secret passage, right goal 
				{
					delay(5000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
			}
			}else{stop();}
			
		}
		break;
		case 2: //moat
		{
			/*not sure if this will work, just copy from rough terrain */
			//not sure if this will work
			driveLeft(255);
			driveRight(255);
			//this is for the possible low goal
			if(goal==1){//check if low goal 
			switch(position){//only if I do low goal
				case 1: //closest to low bar, left goal
				{
				//if differnt position, switch based on it
				delay(5500); //change this as the distance it goes, in milliseconds
				driveRight(-255);//turn toward tower
				delay(1500); //change this to make sure right angle
				driveRight(255);//go towards tower
				delay(2000);//change for distance
				stop();
				intake(-1);// output of intake
				delay(500)//how long does it take for a ball to get out?
				intake(0);//stop intake
				//if there is time, turn around and do stuff

				}
				
				break;
				case 2://middle goal
				{
					delay(4000);//change for the distance it goes
					driveRight(-255);//turn right
					delay(750);//wait to turn correct angle
					driveRight(255);//drive straight
					delay(500);//not long, just long enough to be lined up w/ low goal 
					driveLeft(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveLeft(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 3://middle defence(little to the right ), middle goal
				{
					delay(4000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(333);//not long, just long enough to be lined up w/ low goal 
					driveRight(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveRight(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 4://closest to secret passage, right goal 
				{
					delay(5000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
			}
			}else{stop();}

		}
		break;

		case 3: //ramparts 
		{
			/*not sure if this will work, just copy from rough terrain */
			//not sure if this will work
			driveLeft(255);
			driveRight(255);
			//this is for the possible low goal
			if(goal==1){//check if low goal 
			switch(position){//only if I do low goal
				case 1: //closest to low bar, left goal
				{
				//if differnt position, switch based on it
				delay(5500); //change this as the distance it goes, in milliseconds
				driveRight(-255);//turn toward tower
				delay(1500); //change this to make sure right angle
				driveRight(255);//go towards tower
				delay(2000);//change for distance
				stop();
				intake(-1);// output of intake
				delay(500)//how long does it take for a ball to get out?
				intake(0);//stop intake
				//if there is time, turn around and do stuff

				}
				
				break;
				case 2://middle goal
				{
					delay(4000);//change for the distance it goes
					driveRight(-255);//turn right
					delay(750);//wait to turn correct angle
					driveRight(255);//drive straight
					delay(500);//not long, just long enough to be lined up w/ low goal 
					driveLeft(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveLeft(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 3://middle defence(little to the right ), middle goal
				{
					delay(4000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(333);//not long, just long enough to be lined up w/ low goal 
					driveRight(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveRight(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 4://closest to secret passage, right goal 
				{
					delay(5000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
			}
			}else{stop();}
		}
		break;
		case 4: //cheval - hard
		{
			//how to PID? do I even need PID?
			//not sure if this will work
			driveLeft(255);
			driveRight(255);
			delay(2000);//drive until hit cheval, wait a [time], to make sure it gets there.
			driveLeft(-255);
			driveRight(-255);
			delay(200);//back up a hair. 
			stop();
			arm(armMin + 30);//lift up the arm to position above cheval.
			delay(50);//not sure if this needed, but I think it will make sure the arm actually goes to the right position
			arm(armMin);//bring arm down to lower cheval
			driveLeft(255);
			driveRight(255);
			//this is for the possible low goal
			if(goal==1){//check if low goal 
			switch(position){//only if I do low goal
				case 1: //closest to low bar, left goal
				{
				//if differnt position, switch based on it
				delay(5500); //change this as the distance it goes, in milliseconds
				driveRight(-255);//turn toward tower
				delay(1500); //change this to make sure right angle
				driveRight(255);//go towards tower
				delay(2000);//change for distance
				stop();
				intake(-1);// output of intake
				delay(500)//how long does it take for a ball to get out?
				intake(0);//stop intake
				//if there is time, turn around and do stuff

				}
				
				break;
				case 2://middle goal
				{
					delay(4000);//change for the distance it goes
					driveRight(-255);//turn right
					delay(750);//wait to turn correct angle
					driveRight(255);//drive straight
					delay(500);//not long, just long enough to be lined up w/ low goal 
					driveLeft(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveLeft(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 3://middle defence(little to the right ), middle goal
				{
					delay(4000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(333);//not long, just long enough to be lined up w/ low goal 
					driveRight(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveRight(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 4://closest to secret passage, right goal 
				{
					delay(5000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
			}
			}else{stop();}
		}
		break;
		case 5: //Portcullis - hard
		{
			//not sure if this will work
			arm(armMin);//lower arm
			driveLeft(255);
			driveRight(255);
			delay(2000);//drive until hit portcullis, wait a [time], to make sure it gets there.
			driveLeft(-255);
			driveRight(-255);
			delay(200);//back up a hair. 
			stop();
			arm(armMin + 60);//lift up the arm to open the portcullis
			delay(50);//not sure if this needed, but I think it will make sure the arm actually goes to the right position
			arm(armMin);//bring arm down to allow to go through the 
			driveLeft(255);
			driveRight(255);
			//this is for the possible low goal
			if(goal==1){//check if low goal 
			switch(position){//only if I do low goal
				case 1: //closest to low bar, left goal
				{
				//if differnt position, switch based on it
				delay(5500); //change this as the distance it goes, in milliseconds
				driveRight(-255);//turn toward tower
				delay(1500); //change this to make sure right angle
				driveRight(255);//go towards tower
				delay(2000);//change for distance
				stop();
				intake(-1);// output of intake*/
				delay(500)//how long does it take for a ball to get out?
				intake(0);//stop intake*/
				//if there is time, turn around and do stuff

				}
				
				break;
				case 2://middle goal
				{
					delay(4000);//change for the distance it goes
					driveRight(-255);//turn right
					delay(750);//wait to turn correct angle
					driveRight(255);//drive straight
					delay(500);//not long, just long enough to be lined up w/ low goal 
					driveLeft(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveLeft(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 3://middle defence(little to the right ), middle goal
				{
					delay(4000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(333);//not long, just long enough to be lined up w/ low goal 
					driveRight(-255);//turn to be facing low goal
					delay(750);//wait to turn to the correct angle
					driveRight(255);//drive towards goal
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
				case 4://closest to secret passage, right goal 
				{
					delay(5000);//change for the distance it goes
					driveLeft(-255);//turn left
					delay(750);//wait to turn correct angle
					driveLeft(255);//drive straight
					delay(3000);//change so that stop after running into low goal for a small period of time to make sure it has gotten there. 
					stop();//stop at low goal 
					intake(-1);// output of intake
					delay(500)//how long does it take for a ball to get out?
					intake(0);//stop intake
					//if there is time, turn around and do stuff
				}
				break;
			}
			}else{stop();}
		}
		break;
		case 6: //other 
		{
			driveLeft(255);
			driveRight(255);
			delay(2000); //change this as the distance it goes, in milliseconds, just go faar enough to get 2 points. 
			stop();
		}
		break;
		
	}
	autoDone = 1;//mark that the autonomous command has been completed so it cant be accidentaly twice.
}



