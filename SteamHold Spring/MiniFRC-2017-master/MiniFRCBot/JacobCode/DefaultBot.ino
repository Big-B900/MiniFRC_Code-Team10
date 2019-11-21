#include <SoftwareSerial.h>
#include <AFMotor.h>

/*
You will need to change the following variables based off of what
analog pins on your motor shild you are useing, what motor goes to
what port, and if your drive logic is flipped 
*/

SoftwareSerial bluetooth(A2,A4); //RX,TX  note that RX will go to the TX on the bluetooth, and TX will go to the RX on the bluetooth

//these commands define the motors and what port they will be on the motor shield
//with this code you put both motors on each side of the drivetrain in parallel in one port per side
//this leaves the other two ports open for motors with other functions
AF_DCMotor motorL(2);
AF_DCMotor motorR(3);

//for example, you might want to add a climber motor. To do that,deselct the following line of code and hook up your motor to port 4
//AF_DCMotor climber(4);
//you will also need to follow some instructions further down

int Xaxis_multiplyer = -1;      //change this variable to 1 if your robot turns the wrong way
int Yaxis_multiplyer = 1;       // change ths variable to -1 if your robot drives backward when it should be going forward or vise versa

/*
You shouldn't need to change anything past here unless your adding
something like an automode or an extra motor 
*/

//variables used to recive data from the drive-station and calculate drive logic
float Xaxis,Yaxis,V,W;
int velocityL,velocityR;

//add this line of code if you want another button
//float button;

void setup(){                    // in setup we tell bluetooth communication to start and set all of out motors to "move" forward at 0 speed
  bluetooth.begin(9600);
  drive(0,0);
}

void loop() {
  while(bluetooth.available()>0){                                      //this line tests if we are connected to blutooth
    if ((bluetooth.read()) == 'z'){                                    //this ensures that the arriving data remains in order
      Xaxis = (bluetooth.parseFloat()) * (100) * Xaxis_multiplyer;     //for each axis the driver station sends, we have a variable here to recieve it
      Yaxis = (bluetooth.parseFloat()) * (100) * Yaxis_multiplyer;     //

      /*if you wanted to add a climbing motor, add this. Also remember to define the variable with the other axes and add the button in your config file.
      
      button = bluetooth.parseFloat();      
              
      if (button == 1){
      climber.run(FORWARD);
      climber.setSpeed(255);
      }else{
      climber.setSpeed(0);
      }
      
      */

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

