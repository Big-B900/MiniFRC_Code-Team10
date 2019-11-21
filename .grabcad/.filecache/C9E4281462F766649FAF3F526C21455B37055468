//test auto priming
//test vaulting: turning with cube without lifting

#include <SimpleSoftwareServo.h>
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //bottom shield
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x61);//top shield

//drive motors
Adafruit_DCMotor *motorL1 = AFMS.getMotor(2);//2
Adafruit_DCMotor *motorL2 = AFMS2.getMotor(3);//3
Adafruit_DCMotor *motorR1 = AFMS.getMotor(1);//1
Adafruit_DCMotor *motorR2 = AFMS2.getMotor(4);//4
//elevator motors
Adafruit_DCMotor *elvMotor1 = AFMS.getMotor(3);//3
Adafruit_DCMotor *elvMotor2 = AFMS.getMotor(4);//4
//intake and shooter motors tbd
Adafruit_DCMotor *intakeMotor = AFMS2.getMotor(2);//placeholder pins 1,2
Adafruit_DCMotor *shooterMotor = AFMS2.getMotor(1);//placeholder pins


SoftwareSerial bluetooth(2,3); //RX,TX --> goes to tx,rx on the arduino, I think

SimpleSoftwareServo servo1;
SimpleSoftwareServo servo2;

float Xaxis,Yaxis,Uservo,Dservo;
float V,W;
int velocityL,velocityR;

//take commands from bluetooth
float intakeRoller, elevatorPos;
float elvMotor1Multiplier = 1,
elvMotor2Multiplier = 1;

//vars for shooting cubes
int spinupCommand,spindownCommand;
bool spinup = false;

//vars for servos
int wingRelease, 
srv1ReleasePos = -10, 
srv2ReleasePos = 90, 
srv1KeepPos = 45, 
srv2KeepPos = 180;

//vars for auto
int autoCommand = 0, switchAutoCommand = 0;


//testing stuff
int timeout = 0;

//allows for turbo mode
int fastMode, backMode;
int speedMultiplier = 150, backMultiplier = -1;


//testing autonomous priming method that should only allow auto to run once
int autoPrime;
bool autoPrimed = true;

//time for more auto modes
//axis, left is up
int centerSwitchAuto,//start center, R=left switch, U=right switch
doubleCenterSwitchAuto,//start center,E=left switch, I=right switch
scaleAuto, //start right, F=left scale, J=right scale
advScaleAuto;//start right, K=double right scale (now go to the right side of the switch), L=right scale right switch

void setup(){   
  AFMS.begin();
  AFMS2.begin();
  bluetooth.begin(9600);
  servo1.attach(10);//10 is the pin the servo is connected to
  servo2.attach(9);//signal on pin 9
  drive(0,0);
  //zero servos to keeping the wings in
  servo1.write(srv1KeepPos);
  servo2.write(srv2KeepPos);

//releases test
  motorL1->run(RELEASE);
  motorL2->run(RELEASE);
  motorR1->run(RELEASE);
  motorR2->run(RELEASE);
  elvMotor1->run(RELEASE);
  elvMotor2->run(RELEASE);
  intakeMotor->run(RELEASE);
  shooterMotor->run(RELEASE);

}

void loop() {
  while(bluetooth.available()>0){
    if ((bluetooth.read()) == 'z'){
      Xaxis = -1*(bluetooth.parseFloat()*100);//negative is axis multiplier
      Yaxis = -1*(-bluetooth.parseFloat()*100);//old code has it multiplied by 100 
      wingRelease = bluetooth.parseInt();
      intakeRoller = bluetooth.parseFloat();
      elevatorPos = bluetooth.parseFloat();
      backMode = bluetooth.parseInt();
      spindownCommand = bluetooth.parseInt();//dont need
      autoCommand = bluetooth.parseInt();
      switchAutoCommand = bluetooth.parseInt();
      fastMode = bluetooth.parseInt();
      autoPrime = bluetooth.parseInt();
      //testing advanced autos
      centerSwitchAuto = bluetooth.parseInt();
      doubleCenterSwitchAuto = bluetooth.parseInt();
      scaleAuto = bluetooth.parseInt();
      advScaleAuto = bluetooth.parseInt();

      
      if(autoPrime == 1){
        autoPrimed = true;
      }

      
      if(fastMode == 1){
        speedMultiplier = 255;
      }else{
        speedMultiplier = 150;
      }
      if(backMode == 1){
        backMultiplier = 1;
      }

      //todo: auto stuff (expect just drive forward auto), use keyboard

      //firing commands = just spinup / down, use intake otherwise
      /*if(spindownCommand == 1){//if spin down button is pressed, spin down
        spinup = false;
        shooterMotor->setSpeed(0);
      }else if(spinupCommand == 1){//if spinup buttom is pressed
        spinup = true;
      }
      if (spinup){
        shooterMotor->run(BACKWARD);//change this if spinning the wrong way
        shooterMotor->setSpeed(200);//var is shooter speed, stay constant for everything?
        //maybe multiple shooter speeds with multiple spinup buttons with diff speeds
      }*/   

      //intake movement --> stolen from climb last year
      if(intakeRoller > -.5 && intakeRoller <.5){//"dead zone"
        intakeMotor->setSpeed(0);
      }else if (intakeRoller > 0){
        intakeMotor->run(BACKWARD);
        intakeMotor->setSpeed((int)(255*intakeRoller));
      }else{
        intakeMotor->run(FORWARD);
        intakeMotor->setSpeed((int)(-255*intakeRoller));
      }
      
      //elevator movement --> stolen from intake, doubled
      if(elevatorPos > -.5 && elevatorPos <.5){//"dead zone"
        elvMotor1->setSpeed(0);
        elvMotor2->setSpeed(0);
      }else if (elevatorPos > 0){
        elvMotor1->run(FORWARD);//multipliers below to hopefully even it
        elvMotor1->setSpeed((int)(255*elevatorPos*elvMotor1Multiplier));
        elvMotor2->run(FORWARD);
        elvMotor2->setSpeed((int)(255*elevatorPos*elvMotor2Multiplier));
      }else{
        elvMotor1->run(BACKWARD);
        elvMotor1->setSpeed((int)(-255*elevatorPos*elvMotor1Multiplier));
        elvMotor2->run(BACKWARD);
        elvMotor2->setSpeed((int)(-255*elevatorPos*elvMotor2Multiplier));
      }

      
      //wing release, if the button is pressed, release
      if (wingRelease == 1){
        servo1.write(srv1ReleasePos);
        servo2.write(srv2ReleasePos);
      }else{//default position is kept
        servo1.write(srv1KeepPos);
        servo2.write(srv2KeepPos);
     }
    //simple autos
    if(autoCommand==1 && autoPrimed){
      simpleAuto();
      autoPrimed = false;
    }else if (switchAutoCommand==1 && autoPrimed){
      switchAuto();
      autoPrimed = false;
   //start center autos   
    }else if (centerSwitchAuto==1&&autoPrimed){
      leftSwitchAuto();
      autoPrimed = false;
    }else if (centerSwitchAuto==-1&&autoPrimed){
      rightSwitchAuto();
      autoPrimed = false;
    }else if (doubleCenterSwitchAuto==1&&autoPrimed){
      doubleLeftSwitchAuto();
      autoPrimed = false;
    }else if (doubleCenterSwitchAuto==-1&&autoPrimed){
      doubleRightSwitchAuto();
      autoPrimed = false;
      //start right
    }else if (scaleAuto==1&&autoPrimed){
      leftScaleAuto();
      autoPrimed = false;
    }else if (scaleAuto==-1&&autoPrimed){
      rightScaleAuto();
      autoPrimed = false;
    }else if (advScaleAuto==1&&autoPrimed){
      sideRightSwitchAuto();
      autoPrimed = false;
    }else if(advScaleAuto==-1&&autoPrimed){
      rightScaleRightSwitchAuto();
      autoPrimed = false;
    }

/*centerSwitchAuto,//start center, R=left switch, U=right switch
  doubleCenterSwitchAuto,//start center,E=left switch, I=right switch
  scaleAuto, //start right, F=left scale, J=right scale
  advScaleAuto;//start right, K=double right scale, L=right scale right switch*/

     
      SimpleSoftwareServo::refresh();

      drive(Xaxis,Yaxis);
    timeout = 0;
    } else{
      timeout++;
      if(timeout>500){
        motorL1->run(RELEASE);
        motorL2->run(RELEASE);
        motorR1->run(RELEASE);
        motorR2->run(RELEASE);
        elvMotor1->run(RELEASE);
        elvMotor2->run(RELEASE);
        intakeMotor->run(RELEASE);
        shooterMotor->run(RELEASE);
        timeout = 0;
      }
     /* 
  motorL1->run(RELEASE);
  motorL2->run(RELEASE);
  motorR1->run(RELEASE);
  motorR2->run(RELEASE);
  elvMotor1->run(RELEASE);
  elvMotor2->run(RELEASE);
  intakeMotor->run(RELEASE);
  shooterMotor->run(RELEASE);*/
    }
  }
}

void drive(int Xaxis, int Yaxis) {               
  V = (100 - abs(Xaxis)) * (Yaxis/100) + Yaxis;
  W = (100 - abs(Yaxis)) * (Xaxis/100) + Xaxis;
  velocityL = ((((V-W)/2)/100)*speedMultiplier);//255
  velocityR = ((((V+W)/2)/100)*speedMultiplier);//255
  //changed all of the motors from pointers (->) to .
  motorR1->run((velocityR >= 0) ? BACKWARD : FORWARD);//switched because wires
  motorR1->setSpeed(abs(velocityR));
  motorL1->run((velocityL >= 0) ? FORWARD : BACKWARD);
  motorL1->setSpeed(abs(velocityL));
  //added these to split the motors
  motorR2->run((velocityR >= 0) ? FORWARD : BACKWARD);
  motorR2->setSpeed(abs(velocityR));
  motorL2->run((velocityL >= 0) ? FORWARD : BACKWARD);
  motorL2->setSpeed(abs(velocityL));
}
void simpleAuto(){
  speedMultiplier = 210;
  drive(0,-50);
  delay(2000);
  drive(0,0);
}
void switchAuto(){
  speedMultiplier = 210;
  drive(0,-100);
  delay(1000);
  drive(0,0);
  /*elvMotor1->run(FORWARD);
  elvMotor2->run(FORWARD);
  elvMotor1->setSpeed(200);
  elvMotor2->setSpeed(200);
  delay(1000);
  elvMotor1->setSpeed(0);
  elvMotor2->setSpeed(0);
  delay(200);
  intakeMotor->run(BACKWARD);
  intakeMotor->setSpeed(175);
  delay(2000);
  intakeMotor->setSpeed(0);*/
}

//switch autos
  void leftSwitchAuto(){//start center
  rightSideSpeed(100);
  leftSideSpeed(40);
  delay(1200);
  rightSideSpeed(100);
  leftSideSpeed(100);
  delay(250);
  rightSideSpeed(40);
  leftSideSpeed(100);
  delay(1200);
  drive(0,0);
  //lift elevator to switch height
  //outtake cube
  }
  void rightSwitchAuto(){//start center
   rightSideSpeed(40);
  leftSideSpeed(100);
  delay(1200);
  rightSideSpeed(100);
  leftSideSpeed(100);
  delay(250);
  rightSideSpeed(100);
  leftSideSpeed(40);
  delay(1200);
  drive(0,0);
  //lift elevator to switch height
  //outtake cube   
  }
  //ignore these for now
  void doubleLeftSwitchAuto(){//start center 
  }
  void doubleRightSwitchAuto(){//start center
  
  }
  //scale autos
  void leftScaleAuto(){//start right
  //drive forward
  //turn left
  //drive forward
  //turn right
  //drive forward
  //lift elevator
  //outtake cube
  }
  void rightScaleAuto(){//start right
  rightSideSpeed(100);
  leftSideSpeed(90);
  delay(2700);//3750
  rightSideSpeed(100);
  leftSideSpeed(40);
  delay(750);
  //lift elevator to scale height
  rightSideSpeed(40);
  leftSideSpeed(100);
  delay(750);
  drive(0,0);   
  //outtake cube
  //backup?

  }
  //ignore these for now
  void sideRightSwitchAuto(){//start right
  rightSideSpeed(100);
  leftSideSpeed(100);
  delay(2100);
  rightSideSpeed(150);
  leftSideSpeed(-150);
  delay(850);
  //lift to switch heightx
  rightSideSpeed(100);
  leftSideSpeed(100);
  delay(500);
  drive(0,0);
  //outtake  
  }
  void rightScaleRightSwitchAuto(){//start right
  rightSideSpeed(100);
  leftSideSpeed(90);
  delay(2700);//3750
  rightSideSpeed(100);
  leftSideSpeed(40);
  delay(750);
  //lift elevator to scale height
  rightSideSpeed(40);
  leftSideSpeed(100);
  delay(750);
  drive(0,0); 
  //outtake here
  delay(500);
  rightSideSpeed(-60);
  leftSideSpeed(-60); 
  delay(250);
  //lower scale to intake
  rightSideSpeed(220);
  leftSideSpeed(-220); 
  delay(700);
  //start intaking
  rightSideSpeed(100);
  leftSideSpeed(50);
  delay(1000 );  
  drive(0,0);
  //lift elevator to switch height
  //outtake cube      
  }
  void rightSideSpeed(int velocity){
  motorR1->run((velocity>= 0) ? BACKWARD : FORWARD);//switched because wires
  motorR1->setSpeed(abs(velocity));
 
  //added these to split the motors
  motorR2->run((velocity >= 0) ? FORWARD : BACKWARD);
  motorR2->setSpeed(abs(velocity));
  
  }
  void leftSideSpeed(int velocity){
  motorL1->run((velocity >= 0) ? FORWARD : BACKWARD);
  motorL1->setSpeed(abs(velocity));
  motorL2->run((velocity >= 0) ? FORWARD : BACKWARD);
  motorL2->setSpeed(abs(velocity));
  }




