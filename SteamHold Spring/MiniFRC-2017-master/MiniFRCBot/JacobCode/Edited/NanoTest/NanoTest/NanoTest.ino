/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analo gpin used to connect the potentiometer
int armPos, servoPos, servoCommand;    // variable to read the value from the analog pin
int maxPos = 180, minPos = 0;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  servoPos = 0;
}

void loop() {
  armPos = analogRead(5);
  if(armPos == 1){
    //armUp();
    servoCommand = 20;
  }
  else if(armPos == 2){
    //armDown();
    servoCommand = 100;
  }else{
    //armStay();
  }
  myservo.write(servoCommand);
}
/*val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                          // waits for the servo to get there
 */
void armUp(){
   while(servoPos < maxPos){
      servoPos++;
      myservo.write(servoPos);
   }
}
void armDown(){
     while(servoPos > minPos){
      servoPos--;
      myservo.write(servoPos);
   }
}
void armStay(){
    myservo.write(servoPos);

}

