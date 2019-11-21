#include <AFMotor.h>//motor shield

AF_DCMotor motor0(1);
AF_DCMotor motor1(2);

int pwmRead0, pwmRead0, motorCommand0=0, motorCommand1=0;
int pwmPin0 = 9, pwmPin1 = 10;//servo pins on the motor shield

void setup() {
  // put your setup code here, to run once:
  pinMode(pwmPin0, INPUT);
  pinMode(pwmPin1, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  pwmRead0 = pulseIn(pwmPin0, HIGH);
  pwmRead1 = pulseIn(pwmPin1, HIGH);
  motorCommand0 = pwmRead0/2;
  if(pwmRead0<128){
    motor0.run(BACKWARD);
  }else{
    motor0.run(FORWARD);
  }
  motorCommand1 = pwmRead1/2;
   if(pwmRead1<128){
    motor1.run(BACKWARD);
  }else{
    motor1.run(FORWARD);
  }
  motor0.setSpeed(motorCommand0);
  motor1.setSpeed(motorCommand1);
}
