#include <SoftwareSerial.h>//for the bluetooth
#include <AFMotor.h>//for the motor shield
//thes are from the ypr example. I believe a majority of them are for serial, but I don't know and 
//havent taken the time to figure which
#include <ADXL345.h>
#include <bma180.h>
#include <HMC58X3.h>
#include <ITG3200.h>
#include <MS561101BA.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <EEPROM.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
#include "FreeIMU.h"

#include <Wire.h>
#include <SPI.h>
//end ypr inclusions
/*
You will need to change the following variables based off of what
analog pins on your motor shild you are using, what motor goes to
what port, and if your drive logic is flipped 
*/

SoftwareSerial bluetooth(A0,A1); //RX,TX  note that RX will go to the TX on the bluetooth, and TX will go to the RX on the bluetooth

//these commands define the motors and what port they will be on the motor shield
//with this code you put both motors on each side of the drivetrain in parallel in one port per side
//this leaves the other two ports open for motors with other functions
AF_DCMotor motorL(1);
AF_DCMotor motorR(4);
AF_DCMotor intakeRollerMotor(3);
AF_DCMotor climber(2);

//for example, you might want to add a climber motor. To do that,deselct the following line of code and hook up your motor to port 4
//AF_DCMotor climber(4);
//you will also need to follow some instructions further down

int Xaxis_multiplyer = -1;      //change this variable to 1 if your robot turns the wrong way
int Yaxis_multiplyer = -1;       // change ths variable to -1 if your robot drives backward when it should be going forward or vise versa

/*
You shouldn't need to change anything past here unless your adding
something like an automode or an extra motor 
*/

//variables used to recive data from the drive-station and calculate drive logic
float Xaxis,Yaxis,V,W, intakeRoller,
armPos, 
groundPos, lowBarPos, chevalPos, deliverPos;
int velocityL,velocityR;
int auton;
int climb, endClimb, backClimb;
bool armCommand;

//add this line of code if you want another button
//float button;
int pin1 = 2, pin2=16, pin3 = 17;//pind for data to nano


void setup(){                    // in setup we tell bluetooth communication to start and set all of out motors to "move" forward at 0 speed
  bluetooth.begin(9600);
  drive(0,0);
  pinMode(pin1,OUTPUT); // first digit of commands to nano
  // added for fall 17
  pinMode(pin2,OUTPUT); //second digit
  pinMode(pin3,OUTPUT); //third digit

  
}

void loop() {
  while(bluetooth.available()>0){                                      //this line tests if we are connected to blutooth
    if ((bluetooth.read()) == 'z'){                                    //this ensures that the arriving data remains in order
      Xaxis = (bluetooth.parseFloat()) * (100) * Xaxis_multiplyer;     //for each axis the driver station sends, we have a variable here to recieve it
      Yaxis = (bluetooth.parseFloat()) * (100) * Yaxis_multiplyer;     //
      intakeRoller = bluetooth.parseFloat();
      armPos = bluetooth.parseFloat();
      climb = bluetooth.parseFloat(); //new climb axis
      auton = bluetooth.parseInt();
      //buttons for preset positions of arm
      groundPos = bluetooth.parseInt();//a
      lowBarPos = bluetooth.parseInt();//b
      chevalPos = bluetooth.parseInt();//x
      deliverPos = bluetooth.parseInt();//y

      
      /*climb = bluetooth.parseInt();
      endClimb = bluetooth.parseInt();
      backClimb = bluetooth.parseInt();*/
      armCommand = false;
      
      
      //auto
      if(auton == 1){
        autonomous();
      }else{
        auton = 0;
      }
     
      //new climb, with axis
      if(climb > -.5 && climb <.5){
        climber.setSpeed(0);
      }else if (climb > 0){
        climber.run(FORWARD);
        climber.setSpeed((int)(255*climb));
      }else{
        climber.run(BACKWARD);
        climber.setSpeed((int)(-255*climb));
      }
      // handles commands to the intake roller
      switch((int)(intakeRoller)){ 
      case 1:
        intakeRollerMotor.run(FORWARD);
        intakeRollerMotor.setSpeed(255);
      break;
      case -1:
        intakeRollerMotor.run(BACKWARD);
        intakeRollerMotor.setSpeed(255);    
      break;
      case 0:
        intakeRollerMotor.setSpeed(0);
      break;
      }//arm commands
      if(groundPos == 1){//put the arm on the floor
           digitalWrite(pin1,LOW);
           digitalWrite(pin2,HIGH);
           digitalWrite(pin3,HIGH);
           armCommand = true;//allows no other commands to arm this cycle
        }else if(lowBarPos == 1){//put the arm so it and gear is off the floor, but can go under the low bar
          digitalWrite(pin1,HIGH);//allows low bar auto
          digitalWrite(pin2,HIGH);
          digitalWrite(pin3,LOW);
          armCommand = true;//allows no other commands to arm this cycle
        }else if(chevalPos == 1){//put the arm in the extreme up position so it can manipulate the cheval
          digitalWrite(pin1,HIGH);
          digitalWrite(pin2,LOW);
          digitalWrite(pin3,HIGH);
          armCommand = true;//allows no other commands to arm this cycle
        }else if(deliverPos == 1){//put the arm straight or slightly down so that the lift will go in the gear easily. 
          digitalWrite(pin1,HIGH);//allows possible gear placement in auto
          digitalWrite(pin2,LOW);
          digitalWrite(pin3,LOW);
          armCommand = true;//allows no other commands to arm this cycle
        }
      //sends digital signals to Nano to control servo. Low if down, High if up, none if unmoved. 
        
       if(!armCommand){//if I have not already commanded arm to go up or down
        //groundPos, lowBarPos, chevalPos, deliverPos
        switch((int)(armPos)){
          case -1: //001
            digitalWrite(pin1,LOW);
            digitalWrite(pin2,LOW);
            digitalWrite(pin3,HIGH);
          break;
          case 0: //010
            digitalWrite(pin1,LOW);
            digitalWrite(pin2,LOW);
            digitalWrite(pin3,LOW);
          break;
          case 1://100
            digitalWrite(pin1,LOW);
            digitalWrite(pin2,HIGH);
            digitalWrite(pin3,LOW);
          break;
        }
       }




      
      drive(Xaxis,Yaxis);              //This line tells the drive function what speed and direction to move the motors in
    } 

  }
}

//this function handles drive logic and actuation. There arn't too many reasons to mess with this function
void drive(int Xaxis, int Yaxis) {               
  V = (100 - abs(Xaxis)) * (Yaxis/100) + Yaxis;        //this is where the X and Y axes inputs are converted into tank drive logic
  W = (100 - abs(Yaxis)) * (Xaxis/100) + Xaxis;        //
  velocityL = ((((V-W)/2)/100)*255);                   //         
  velocityR = ((((V+W)/2)/100)*255);                   //        

  motorR.run((velocityR >= 0) ? FORWARD : BACKWARD);  //These comands tell the motors what speed and direction to move it
  motorR.setSpeed(abs(velocityR));                    //
  motorL.run((velocityL >= 0) ? FORWARD : BACKWARD);  //
  motorL.setSpeed(abs(velocityL));                    //
}
void autonomous(){
    digitalWrite(19,HIGH);
    drive(0,100); 
    intakeRollerMotor.run(FORWARD);
    intakeRollerMotor.setSpeed(100); 
    delay(5000);
    drive(0,0);
    intakeRollerMotor.setSpeed(0);
}


