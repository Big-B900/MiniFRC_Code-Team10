#include <SoftwareSerial.h>
#include <AFMotor.h>


//Basic MiniFRC Bot Code 2017 V1


float zero;                             //these are the axes/buttons the robot expects to recieve from the driver station
float one;
//int button_one; //this is what it would look like to define a button up here

int noSignal = 0;                       //this variable is used to shut off the 4 drive motors if the robot loses signal

char z;                                 //this is for making sure our bluetooth signal does not get out of order

float powerLeft = 0;
float powerRight = 0;                       //these variables are for telling their respective motor how much to move
//float powerBL = 0;
//float powerBR = 0;
                                          //RX,TX  note that RX will go to the TX on the bluetooth, and TX will go to the RX on the bluetooth
SoftwareSerial bluetooth(14,15);          //Also note that pin 14 is actually analog 0 and 15 is analog 1
                                       
AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);      
//AF_DCMotor motorBL(3);                      //defining our motors and what port they will be on the motor shield
//AF_DCMotor motorBR(4);

void setup(){                                // in setup we simply allow bluetooth to start and set all of out motors to "move" forward at 0 speed
  bluetooth.begin(9600);
  
  motorLeft.setSpeed(0);
  motorLeft.run(FORWARD);
  
  motorRight.setSpeed(0);
  motorRight.run(FORWARD);
}

void loop() {
  while(bluetooth.available()>0){
    z = bluetooth.read();
    if (z == 'z'){                          //this ensures that the arriving data remains in order
      zero = bluetooth.parseFloat(); 
      one = bluetooth.parseFloat();          //for each axis the driver station sends, we have a variable here to recieve it
      
      //button_one = bluetooth.parseInt();   //this is an example of adding a button, remember though that it must also be defined back at the top
      
      powerLeft = (zero*255)+(one*255);         //In this block we turn our axis values(-1 to 1) into motor power levels (-255 to 255)
      powerRight = (zero*255)+(-one*255);   
      //powerBL = (zero*255)+(one*255); 
      //powerBR = (zero*255)+(-one*255);        //If a motor is turning in the wrong direction, you'd change these to be positive/negative as required
    
        powerLeft = 255;
      }
      else if (powerLeft < -255){               //we make sure that our power levels have not exceeded +- 255
        powerLeft = -255;
      }
      if(powerRight >255){
        powerRight = 255;
      }
      else if (powerRight < -255){
        powerRight = -255;
      }/*
      if(powerBL >255){     
        powerBL = 255;
      }
      else if (powerBL < -255){
        powerBL = -255;
      }
      if(powerBR >255){
        powerBR = 255;
      }
      else if (powerBR <-255){
        powerBR = -255;
      }*/
      
      if(powerLeft < 0){                    //Now we finally apply our power levels to their motors, note that setSpeed() only accepts positive numbers
        motorLeft.run(BACKWARD);
        motorLeft.setSpeed(-powerLeft);       //note that to handle negative power levels, we reverse the motor direction and make the power level positive
      }
      if (powerLeft>=0){
        motorLeft.run(FORWARD);
        motorLeft.setSpeed(powerLeft);
      } 
      
      if(powerRight < 0){
        motorRight.run(BACKWARD);
        motorRight.setSpeed(-powerRight);
      }
      if (powerRight>=0){
        motorRight.run(FORWARD);
        motorRight.setSpeed(powerRight);
      }
      
     /* if(powerBL < 0){
        motorBL.run(BACKWARD);
        motorBL.setSpeed(-powerBL);
      }
      if (powerBL>=0){
        motorBL.run(FORWARD);
        motorBL.setSpeed(powerBL);
      }  
      
      if(powerBR < 0){
        motorBR.run(BACKWARD);
        motorBR.setSpeed(-powerBR);
      }
      if (powerBR>=0){
        motorBR.run(FORWARD);
        motorBR.setSpeed(powerBR);
      } */      
    }
 

  noSignal += 1;
  if (noSignal > 40){
    noSignal = 0;                         //if you have over 3ish seconds of packet loss, the robot will stop moving.
    motorLeft.setSpeed(0);
    motorRight.setSpeed(0);
    //motorBL.setSpeed(0);
    //motorBR.setSpeed(0);    
  }
  delay(40);
}
