#include <Adafruit_MotorShield.h>
//instructions for downloading and installing libraries can be found here: https://www.instructables.com/id/Downloading-All-the-Software-Youll-Need-for-MiniFR/
#include <SoftwareSerial.h>       

/* <==============================================================>
 *  You will need to change the following variables depending on what
 *  analog pins on your motor shield you are using, which motor goes to
 *  which port, and if your drive logic is flipped. */

//change A0 and A1 to match whatever pins you are useing for your bluetooth chip
SoftwareSerial bluetooth(A3, A4); //RX,TX

Adafruit_MotorShield Shield1 = Adafruit_MotorShield();
Adafruit_MotorShield Shield2 = Adafruit_MotorShield(0x61);

//These lines declare which ports your motors will be connected to on the motor shield.
Adafruit_DCMotor* FrontLeft = Shield1.getMotor(1);
Adafruit_DCMotor* FrontRight = Shield1.getMotor(2);
Adafruit_DCMotor* BackLeft = Shield1.getMotor(4);
Adafruit_DCMotor* BackRight = Shield1.getMotor(3);

Adafruit_DCMotor* HatchIntake = Shield2.getMotor(3);
Adafruit_DCMotor* Arm = Shield2.getMotor(4);
Adafruit_DCMotor* CargoIntake = Shield2.getMotor(2);

int xAxisMultiplier = 1;      // Change this variable to -1 if your robot turns the wrong way
int yAxisMultiplier = 1;       // Change ths variable to -1 if your robot drives backward when it should be going forward

/* You shouldn't need to change anything past here unless you're adding
 *  something like an automode, extra motor, or servo. 
 *  <==============================================================> */

// Variables used to recive data from the driverstation and calculate drive logic
float xAxis, yAxis, rAxis, rCheck;
float moveArm, intakeHatch, outtakeHatch, intakeCargo, outtakeCargo, speedMode, precisionMode, fulcrumSucc, fulcrumRelease;
float FLMod, FRMod, BLMod, BRMod;
float drive1A, drive2A, drive3A, hatchA, cargoA;
int velocityL, velocityR;
bool speedy = true;
bool autoUsed;
// In setup, we tell bluetooth communication to start and set all of our motors to not move
void setup() {
  bluetooth.begin(9600);
  
  Shield1.begin();
  Shield2.begin();

  FLMod = 1;
  FRMod = 1;
  BRMod = 1;
  BLMod = 1;
  omniDrive(0,0,0);

  autoUsed = false;
}

void loop() {
  while(bluetooth.available() > 0){  // This line checks for any new data in the buffer from the driverstation
    if ((bluetooth.read()) == 'z'){     
      // We use 'z' as a delimiter to ensure that the data doesn't desync
      xAxis = bluetooth.parseFloat() * -1;     // For each axis the driver station sends, we have a variable here to recieve it
      yAxis = bluetooth.parseFloat() * -1;
      rCheck = bluetooth.parseFloat();
      
      moveArm = bluetooth.parseFloat();
      intakeHatch = bluetooth.parseFloat();
      outtakeHatch = bluetooth.parseFloat();
      intakeCargo = bluetooth.parseFloat();
      outtakeCargo = bluetooth.parseFloat();
      speedMode = bluetooth.parseFloat();
      precisionMode = bluetooth.parseFloat();
      drive3A = bluetooth.parseFloat();
      drive2A = bluetooth.parseFloat();
      drive1A = bluetooth.parseFloat();
      hatchA = bluetooth.parseFloat();
      cargoA = bluetooth.parseFloat();

      /* --- Autos --- */
        //if(!autoUsed){
          if(drive1A == 1){
            delay(5000);
            autoUsed = true;
            omniDrive(1,0,0);
            delay(1500);
            omniDrive(0,0,0);
            speedy = true;
          }
          else if(drive2A == 1){
            delay(5000);
            autoUsed = true;
            speedy = false;
            Arm->run(BACKWARD);
            Arm->setSpeed(255);
            omniDrive(1,0,0);
            delay(250);
            Arm->setSpeed(0);
            delay(2000);
            omniDrive(0,0,0);
            speedy = true;
          }
          else if(drive3A == 1){
            CargoIntake->setSpeed(255);
            CargoIntake->run(FORWARD);
            delay(500);
            CargoIntake->setSpeed(0);
            delay(5000);
            autoUsed = true;
            speedy = true;
            Arm->run(BACKWARD);
            Arm->setSpeed(255);
            delay(500);
            Arm->setSpeed(0);
            omniDrive(1,0,0);
            delay(1000);
            omniDrive(0,0,0);
            speedy = false;
            Arm->run(FORWARD);
            Arm->setSpeed(255);
            delay(550);
            Arm->setSpeed(0);
            delay(250);
            omniDrive(-0.5,0,0);
            delay(1000);
            omniDrive(1,0,0);
            delay(1250);
            omniDrive(0,0,1);
            delay(250);
            omniDrive(0,0,0);
            delay(250);
            omniDrive(0,0.5,0);
            delay(1500);
            omniDrive(0,0,0);
            speedy = true;
          }
          else if(hatchA == 1){
            delay(5000);
            autoUsed = true;
            speedy = true;
            for(float i = 0; i <= 0.55; i += 0.01){
              omniDrive(0.55-i,i,0);
              delay(4);
            }
            omniDrive(0,0.55,0.2);
            delay(700);
            omniDrive(0,0,0);
            HatchIntake->run(BACKWARD);
            HatchIntake->setSpeed(255);
            delay(100);
            HatchIntake->setSpeed(0);
            omniDrive(0,-1,0);
            delay(300);
            omniDrive(0,0,0);
          }
          
        //}
      /* --- Mechanism Section --- */

        // Hatch grabber code
        if (intakeHatch == 1){
          HatchIntake->run(BACKWARD);
          HatchIntake->setSpeed(255);
        }
        else if (outtakeHatch == 1){
          HatchIntake->run(FORWARD);
          HatchIntake->setSpeed(255);
        }
        else{
          HatchIntake->setSpeed(0);
        }

        // Cargo intake code
        if (intakeCargo > 0){
          CargoIntake->run(BACKWARD);
          CargoIntake->setSpeed(255);
        }
        else if (outtakeCargo > 0){
          CargoIntake->run(FORWARD);
          CargoIntake->setSpeed(255);
        }
        else{
          CargoIntake->setSpeed(0);
        }

        // Arm code
        if (moveArm > 0.1){
          Arm->run(BACKWARD);
          Arm->setSpeed(moveArm*255);
        }
        else if (moveArm < -0.1){
          Arm->run(FORWARD);
          Arm->setSpeed(moveArm*255*-1);
        }
        else{
          Arm->setSpeed(0);
        }
      
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
     omniDrive(xAxis, yAxis, rAxis);
    }
  }
}

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
  BackRight->run((v4 >= 0) ? FORWARD : BACKWARD);     
  BackRight->setSpeed(abs(v4));
  FrontLeft->run((v2 >= 0) ? FORWARD : BACKWARD);
  FrontLeft->setSpeed(abs(v2));
  BackLeft->run((v3 >= 0) ? FORWARD : BACKWARD);
  BackLeft->setSpeed(abs(v3));
}
