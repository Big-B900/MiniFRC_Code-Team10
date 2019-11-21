#include <SoftwareSerial.h>//for the bluetooth
#include <AFMotor.h>//for the motor shield
//ADDed stuff here and below for sensors
#include <QTRSensors.h>

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
AF_DCMotor intakeRollerMotor(2);
AF_DCMotor climber(3);

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

//ADDED advAuto button
int advAuto;

//add this line of code if you want another button
//float button;
int pin1 = 2, pin2=16, pin3 = 17;//pind for data to nano

//ADDED stuff for the IR sensors...
#define NUM_SENSORS   3     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2
QTRSensorsRC qtrrc((unsigned char[]) {2,9,10},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
int lineThreshold = 700,//threshold at which robot decides a line has been found, 0-1000, higher is darker...
lineTarget = 1000;//target for where the line should be, 1000 is under the middle sensor

void setup(){                    // in setup we tell bluetooth communication to start and set all of out motors to "move" forward at 0 speed
  bluetooth.begin(9600);
  drive(0,0);
  pinMode(pin1,OUTPUT); // first digit of commands to nano
  // added for fall 17
  pinMode(pin2,OUTPUT); //second digit
  pinMode(pin3,OUTPUT); //third digit
  //ADD sensor setup...
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
      //ADD a second auto button in the config
      //ADDED a second auto parse button
      advAuto = bluetooth.parseInt();
      
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
      //ADDED advancedAuto caller
      if(advAuto == 1){
        advancedAuto();
      }else{
        advAuto = 0;
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


      
      /*// handles commands to the intake roller
      if(intakeRoller<.1&&intakeRoller>-.1){
        intakeRollerMotor.setSpeed(0);
      }else if (intakeRoller>0){
        intakeRollerMotor.run(FORWARD);
        intakeRollerMotor.setSpeed(255*intakeRoller);
      }else{
        intakeRollerMotor.run(BACKWARD);
        intakeRollerMotor.setSpeed(-255*intakeRoller);
      }*/

//avrdude: ser_recv(): read error: The I/O operation has been aborted because of either a thread exit or an application request.

       //old intake commands
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
      }
      
      
      
      
      
      //arm commands
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
    digitalWrite(19,HIGH);//update...
    drive(0,100); 
    intakeRollerMotor.run(FORWARD);
    intakeRollerMotor.setSpeed(100); 
    delay(5000);
    drive(0,0);
    intakeRollerMotor.setSpeed(0);
}
//ADDED advancedAuto function
//need to add a calibrate function (and button and stuff, sigh...)
void advancedAuto(){
    //send command to go to deliver position, leave it there
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);
    delay(100);
    //send stay command...
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);    
    drive(0,100);//drive forward full speed
    delay(3000);//wait 3 seconds, need to calibrate from testing to see what makes sense. 
    //would work better if there was gyro to detect going over defence
    drive(0,0);//stop driving

    //put arm (and more importantly light sensors down
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,HIGH);
    digitalWrite(pin3,HIGH);
    //drive forward until line detected by middle sensor
    //HERE IS WHERE THE STUFF WITH THE LINE SENSOR STARTS THAT I HAVE NOT ADDED UP ABOVE, IT WONT WORK, ALSO IT MAY NOT WORK even with stuff above
    drive(0,50);//drive forward half speed
    bool line = false;
    while(!line){//while line is not found...
      //use this if a calibration command has been done earlier...
      //higher number is darker
      qtrrc.readCalibrated(sensorValues);//reads sensor values 0 - 1000, 1000 is darkest.
      if(sensorValues[1]>lineThreshold){//check if the center read value is greater than the threshold
        //if higher than threshold, there should be a line
        line = true;//exit loop, line found, continue...
      }
    }
    //now comes the line following part...
    //loop to repeat this until end of line found, then try for end of line, otherwise follow the line...
   bool endFound = false;
   while(!endFound){
      qtrrc.readCalibrated(sensorValues);//read the raw sensor values
      //test if all 3 have found the line, signifying the end of the line.
      if(sensorValues[0]>lineThreshold&&sensorValues[1]>lineThreshold&&sensorValues[2]>lineThreshold){
        endFound = true;//exit linefollowing loop
        drive(0,0);//stop the robot.
      }else{//if end is not found follow line
        unsigned int linePos = qtrrc.readLine(sensorValues);
        //using int lineTarget defined above, if the number is above or below it, use some command scheme to bring it back to the right place.
        //use bang bang or pid...
        //need to know if 0 is right or left (how the sensor is oriented on robot)

  //for example, here is bang bang (I think)
        //if it is turning the wrong direction, flip > and < in if and elif statements
        if(linePos<lineTarget-125){//if the line is to the [lower value] of target, with a threshold (in this case 125) so that it is not going crazy...
          motorR.run(FORWARD);//drive 1 side forward
          motorR.setSpeed(100);// drive at about half speed
        }else if(linePos>lineTarget+125){//if the line is to the [higher value] of the target, with a threshold
          motorL.run(FORWARD);//drive the other side forward
          motorL.setSpeed(100);//drive at about half speed
        }else{//if the robot is on the line
          drive(0,50);//drive forward half speed. 
        }
          
      }
   }
      //the robot should be at the end of the line now
    //put the arm in the deliver position
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);
    delay(100);
    //send stay command...
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW); 
    drive(0,50);//drive forward 1/2 speed
    delay(2000);//wait 2 seconds driving forward, hopefully on peg
    drive(0,0);
    //gear should be on peg and in robot, pilot should hopefully be able to lift it... 
    //next part may be neccessary if unable to lift gear from robot, it delivers it on the peg.
    /*
    //go to the down position
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,HIGH);
    digitalWrite(pin3,HIGH);
    delay(100);
    //send stay command...
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW); 
    drive(0,-50);//drive backwards half speed
    delay(1000);//wait 1 second
    drive(0,0);//stop the bot
    */
   
   
    
}

