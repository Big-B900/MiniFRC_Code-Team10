#include <SimpleSoftwareServo.h>
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x61);

Adafruit_DCMotor *motorL1 = AFMS.getMotor(3);
Adafruit_DCMotor *motorR1 = AFMS.getMotor(4);
Adafruit_DCMotor *motorL2 = AFMS.getMotor(1);
Adafruit_DCMotor *motorR2 = AFMS.getMotor(2);

SoftwareSerial bluetooth(2,3); //RX,TX

SimpleSoftwareServo servo1;
SimpleSoftwareServo servo2;

float Xaxis,Yaxis,Uservo,Dservo;
float V,W;
int velocityL,velocityR,srv;

void setup(){   
  AFMS.begin();
  bluetooth.begin(9600);
  servo1.attach(10);//10 is the pin the servo is connected to
  servo2.attach(9);//signal on pin 9
  drive(0,0);
}

void loop() {
  while(bluetooth.available()>0){
    if ((bluetooth.read()) == 'z'){
      Xaxis = (-bluetooth.parseFloat());
      Yaxis = (-bluetooth.parseFloat()); 
      Uservo = bluetooth.parseFloat();
      Dservo = bluetooth.parseFloat();

      if (Dservo == 1)
        srv = 45;
      if (Uservo == 1)
        srv = 180;
      if ((Uservo == 0)&&(Dservo == 0))
        srv = 70;

      servo1.write(srv);
      SimpleSoftwareServo::refresh();
      
      drive(Xaxis,Yaxis);

    } 
  }
}

void drive(int Xaxis, int Yaxis) {               
  V = (100 - abs(Xaxis)) * (Yaxis/100) + Yaxis;
  W = (100 - abs(Yaxis)) * (Xaxis/100) + Xaxis;
  velocityL = ((((V-W)/2)/100)*255);
  velocityR = ((((V+W)/2)/100)*255);

  motorR->run((velocityR >= 0) ? FORWARD : BACKWARD);
  motorR->setSpeed(abs(velocityR));
  motorL->run((velocityL >= 0) ? FORWARD : BACKWARD);
  motorL->setSpeed(abs(velocityL));
}








