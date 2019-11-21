/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

//int potpin = 0;  // analo gpin used to connect the potentiometer
int input1, input2, input3, //make sure int is the right variable type
servoPos;    // variable to read the value from the analog pin
int maxPos = 130, minPos = 0;
int lowBarPos = 30, //position at which arm clears bottom and low bar with gear
deliverPos = 90, //position for delivering gears, probably straight in front
chevalPos = maxPos, //position at which clears cheval
floorPos = minPos;//position at which on floor to intake gear

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(minPos);
  servoPos = minPos;
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);//change to actual pin
  pinMode(A7, INPUT);//change to actual pin
}

void loop() {
  /**this whole thing is new**/
  //add digital reads!
  input1 = digitalRead(A5);
  input2 = digitalRead(A6);
  input3 = digitalRead(A7);
  if(input1 == LOW){//0__
    if(input2 == LOW){//00_
      if(input3 == LOW){//000 - stay
        myservo.write(servoPos);
      }else{//001 - arm up
        armUp();
      }
    }else{//01_
      if(input3 == LOW){//010 -arm down
        armDown();
      }else{//011 - floorPos
        myservo.write(floorPos);
        servoPos = floorPos;
      }
    }
  }else{//1__
    if(input2 == LOW){//10_
      if(input3 == LOW){//100 -deliverPos
        myservo.write(deliverPos);
        servoPos = deliverPos;
      }else{//101 -chevalPos
        myservo.write(chevalPos);
        servoPos = chevalPos;
      }
    }else{//11_
      if(input3 == LOW){//110 - lowBarPos
        myservo.write(lowBarPos);
        servoPos = lowBarPos;
      }
      else{//111 - nothing here yet
        
      }
    }
  }



//might need to add a delay(10); here

  
  /**Removed for new system
   * command = digitalRead(A5);
  if(command == HIGH){
    armUp();
  }else if(command == LOW){
    armDown();
  }else{
    armStay();
  }
  command = NULL;**/
}
/*val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                          // waits for the servo to get there
 */
void armUp(){
   for(int u = 0;u<5;u++){  
    if(servoPos < maxPos){
        servoPos++;
        myservo.write(servoPos);
        delay(10);
      }
   }
   /*servoPos=servoPos+10;
   myservo.write(servoPos);
   delay(100);  */ 
}
void armDown(){
   for(int d = 0;d<5;d++){  
     if(servoPos > minPos){
        servoPos--;
        myservo.write(servoPos);
        delay(10);
      }
   }
   /*servoPos = servoPos - 10;
   myservo.write(servoPos);
   delay(100);*/
}
/**removed
void armStay(){
    myservo.write(servoPos);

}**/

