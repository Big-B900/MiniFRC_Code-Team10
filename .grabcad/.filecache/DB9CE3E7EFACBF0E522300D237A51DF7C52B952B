#include <Adafruit_MotorShield.h>
//instructions for downloading and installing libraries can be found here: https://www.instructables.com/id/Downloading-All-the-Software-Youll-Need-for-MiniFR/
#include <SoftwareSerial.h>       

#include "FreeIMU.h"//for the IMU

/* <==============================================================>
 *  You will need to change the following variables depending on what
 *  analog pins on your motor shield you are using, which motor goes to
 *  which port, and if your drive logic is flipped. */

//change A0 and A1 to match whatever pins you are useing for your bluetooth chip
SoftwareSerial bluetooth(2, A4); //RX,TX

Adafruit_MotorShield Shield1 = Adafruit_MotorShield();
Adafruit_MotorShield Shield2 = Adafruit_MotorShield(0x61);

//These lines declare which ports your motors will be connected to on the motor shield.
Adafruit_DCMotor* FrontLeft = Shield1.getMotor(4);
Adafruit_DCMotor* FrontRight = Shield1.getMotor(3);
Adafruit_DCMotor* BackLeft = Shield1.getMotor(1);
Adafruit_DCMotor* BackRight = Shield1.getMotor(2);

int xAxisMultiplier = -1;      // Change this variable to -1 if your robot turns the wrong way
int yAxisMultiplier = 1;       // Change ths variable to -1 if your robot drives backward when it should be going forward

/* You shouldn't need to change anything past here unless you're adding
 *  something like an automode, extra motor, or servo. 
 *  <==============================================================> */

// Variables used to recive data from the driverstation and calculate drive logic
float xAxis, yAxis, rAxis, rCheck;
int speedMode, precisionMode, robotCentricMode, fieldCentricMode, zeroHeadingCommand;

bool speedy = true, fieldCentric =  false;

//these are the variables for the field centric drive
float fieldHeadingDegrees;//what the raw heading will read when the robot is aligned with the field
//this is subtracted from the raw heading to get the robot heading 

//for the gyro
float ypr[3]; // yaw pitch roll
FreeIMU my3IMU = FreeIMU();

void setup() {
  bluetooth.begin(9600);
  
  Shield1.begin();
  Shield2.begin();


  omniDrive(0,0,0);
//startig the imu
  Wire.begin();
  delay(5);
  my3IMU.init(); // the parameter enable or disable fast mode
  delay(5);

  fieldHeadingDegrees = 0;//auto zero the robot when code starts up (I think)
  Serial.begin(9600);
  
}

void loop() {
  while(bluetooth.available() > 0){  // This line checks for any new data in the buffer from the driverstation
    if ((bluetooth.read()) == 'z'){     
      // We use 'z' as a delimiter to ensure that the data doesn't desync
      xAxis = bluetooth.parseFloat() * -1;     // For each axis the driver station sends, we have a variable here to recieve it
      yAxis = bluetooth.parseFloat() * -1;
      rCheck = bluetooth.parseFloat();
      
      speedMode = bluetooth.parseInt();
      precisionMode = bluetooth.parseInt();

      robotCentricMode = bluetooth.parseInt();
      fieldCentricMode = bluetooth.parseInt();
      zeroHeadingCommand = bluetooth.parseInt();
     
      /* --- Drive Section --- */

      // Precision v. Speed
      if (precisionMode == 1){
        speedy = false;
      }
      if (speedMode == 1){
        speedy = true;
      }

      // Deadzone for drive rotation
      if(rCheck >= 0.1 || rCheck <= -0.1){
        rAxis = rCheck;
      }
      else{
        rAxis = 0;
      }

      if (yAxis >= 0.2 || yAxis <= -0.2){
        yAxis = yAxis;
      }
      else{
        yAxis = 0;
      }

      if (xAxis >= 0.2 || xAxis <= -0.2){
        xAxis = xAxis;
      }
      else{
        xAxis = 0;
      }


      //make the field aligned with the current robot alignment
      if(zeroHeadingCommand == 1){
        zeroHeading();
      }
       
      //toggle robot and field centric
      if(robotCentricMode == 1){
        fieldCentric = false;
      }
      if(fieldCentricMode == 1){
        fieldCentric = true;
      }
      
      //which drive to use in a one line if statement bc why not
      fieldCentric ? fieldCentricDrive(xAxis,yAxis,rAxis) : omniDrive(xAxis, yAxis, rAxis);
    
     //omniDrive(xAxis, yAxis, rAxis);//robot centric
     //fieldCentricDrive(xAxis,yAxis,rAxis);//field centric
     
    }
  }
}


/*establishments:
 * Inputs:
 *  x axis is horizontal on the stick
 *  y axis is vertical on the stick
 *  
 * Outputs:
 *  robot x is strafe
 *  robot y is forward backwards
 * 
*/
//get heading from the gyro
float getHeading(){
  float rawHeadingDegrees, robotHeadingDegrees, robotHeadingRadians;
  rawHeadingDegrees = getYaw();
  robotHeadingDegrees = rawHeadingDegrees - fieldHeadingDegrees;//account for an offset
  robotHeadingRadians = robotHeadingDegrees*PI/180;
  return robotHeadingRadians;
}

//this function interprets commands in field centric mode and sends them to the drive code as robot centric
void fieldCentricDrive(float fieldY, float fieldX, float rotation){
  float robotX, robotY, robotHeading;
  
  robotHeading = getHeading();
  //trig taken from Kauailabs, needs to be tested
  //it feels wrong, at 0 it switches x and y
  robotX = fieldY*cos(robotHeading)+fieldX*sin(robotHeading);
  robotY = -fieldY*sin(robotHeading)+fieldX*cos(robotHeading);
  
  omniDrive(robotX, robotY, rotation);
}
//zero the field to whatever the current angle of the robot is
void zeroHeading(){
  fieldHeadingDegrees = getYaw();
}
//use the gyro to get the current yaw of the robot
float getYaw(){
  my3IMU.getYawPitchRoll(ypr);
  Serial.println(ypr[0]);
  return ypr[0];  
  //return 0;//TEMPORARY, should make field centric act like robot centric
}


//todo:
  //zero the gyro
  //snap to angle
  //toggle between field centric and robot centric
  //account for gyro noise and gyro drift




// This function handles drive logic and actuation. Don't change this unless you know what you're doing.
void omniDrive(float x, float y, float r) {

  float driveMod;
  bool overFlow = false;

  x *= 255;
  y *= 255;
  r *= 255;
  
  float v1 = (-1*sqrt(2)/2) * x - (sqrt(2)/2) * y + r;
  float v2 = (-1*sqrt(2)/2) * x + (sqrt(2)/2) * y - r;
  float v3 = (sqrt(2)/2) * x + (sqrt(2)/2) * y + r;
  float v4 = (sqrt(2)/2) * x - (sqrt(2)/2) * y - r;

  if (v1 > 255 || v2 > 255 || v3 > 255 || v4 > 255){
    overFlow = true;
  }

  if (speedy || overFlow){
    if (v1 >= v2 && v1 >= v3 && v1 >= v4){
     driveMod = 255/v1;
      v1 *= driveMod;
      v2 *= driveMod;
      v3 *= driveMod;
      v4 *= driveMod; 
    }
    else if (v2 >= v3 && v2 > v4){
      driveMod = 255/v2;
      v1 *= driveMod;
      v2 *= driveMod;
      v3 *= driveMod;
      v4 *= driveMod;
    }
    else if (v3 > v4){
      driveMod = 255/v3;
      v1 *= driveMod;
      v2 *= driveMod;
      v3 *= driveMod;
      v4 *= driveMod;
    }
    else{
      driveMod = 255/v4;
      v1 *= driveMod;
      v2 *= driveMod;
      v3 *= driveMod;
      v4 *= driveMod;
    }
  }

  FrontRight->run((v1 >= 0) ? FORWARD : BACKWARD);     // These comands tell the motors what speed and direction to move
  FrontRight->setSpeed(abs(v1));
  BackRight->run((v4 >= 0) ? BACKWARD : FORWARD);     
  BackRight->setSpeed(abs(v4));
  FrontLeft->run((v2 >= 0) ? FORWARD : BACKWARD);
  FrontLeft->setSpeed(abs(v2));
  BackLeft->run((v3 >= 0) ? BACKWARD : FORWARD);
  BackLeft->setSpeed(abs(v3));
}
