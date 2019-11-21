#include <AFMotor.h>//for the motor shield


AF_DCMotor intakeRollerMotor(2);


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
    intakeRollerMotor.run(FORWARD);
    intakeRollerMotor.setSpeed(200);
    delay(2000);
        intakeRollerMotor.setSpeed(0);
    delay(2000);
    intakeRollerMotor.run(BACKWARD);
    intakeRollerMotor.setSpeed(200);
    delay(2000);
    intakeRollerMotor.setSpeed(0);
    delay(2000);
}
