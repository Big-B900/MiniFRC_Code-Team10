/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
Servo leftArm;
Servo rightArm;

//int potpin = 0;  // analo gpin used to connect the potentiometer
int Command, servoPos, firstCommand, secondCommand, thirdCommand;    // variable to read the value from the analog pin
int maxPos = 150, minPos = 10,
deliverPos = 90;//whatever position can deliver gear
//cheval = 0;//0 is retracted, 1 is extended.

//cheval arms positions
int rightRetracted = 180, leftRetracted=0, rightExtended=45, leftExtended=135;


void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(minPos);
  leftArm.attach(10);//not sure if the correct arm, but it doesnt matter.
  leftArm.write(leftRetracted);
  rightArm.attach(11);
  rightArm.write(rightRetracted);
  servoPos = minPos;
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT);
}

void loop() {
  /*command = digitalRead(A5);
  if(command == HIGH){
    armUp();
  }else if(command == LOW){
    armDown();
  }else{
    armStay();
  }
  command = NULL;*/
  firstCommand = digitalRead(A5);
  secondCommand = digitalRead(A4);
  thirdCommand = digitalRead(A3);

  /* first attempt...
   *  
   *if(firstCommand == LOW){//0xx
    if(secondCommand == LOW){//00x
      if(thirdCommand == LOW){//000 - stay
        armStay();
      }else{//001 - down 5 degrees
        if(servoPos-5>=minPos){
          servoPos=servoPos-5;
          myservo.write(servoPos);
          delay(10);//wait 10 ms
        }else{//so it doesn't go below the minimum
          servoPos = minPos;
          myservo.write(minPos);
          delay(10);
        }
      }
    }else{//01x
      if(thirdCommand
    }
  }*/
  //setting command based on the 3 inputs
  if(firstCommand==LOW){//0xx
    if(secondCommand==LOW){//00x
      if(thirdCommand ==LOW){//000 - stay
        Command = 0;
      }else{//001 - up 5 degrees
        Command = 1;
      }
    }else{//01x
      if(thirdCommand ==LOW){//010 - down 5 degrees 
        Command = 2;
      }else{//011 - floor pos
        Command = 3;
      }
    }
  }else{//1xx
        if(secondCommand==LOW){//10x
      if(thirdCommand ==LOW){//100 - deliver pos
        Command = 4;
      }else{//101 - chevalPos
        Command = 5;
      }
    }else{//11x
      if(thirdCommand ==LOW){//110 - low bar pos
        Command = 6;
      }else{//111  - nothing for yet, maybe hp station pos
        Command = 7;
      }
    }
  }
  //reads command
  switch(Command){
    case 0://stay
      armStay();
    break;
    case 1://up 5
      if(servoPos+5<=maxPos){
          servoPos=servoPos+5;
          myservo.write(servoPos);
          delay(10);//wait 10 ms
        }else{//so it doesn't go below the minimum
          servoPos = maxPos;
          myservo.write(maxPos);
          delay(10);
        }
    break;
    case 2://down 5
      if(servoPos-5>=minPos){
          servoPos=servoPos-5;
          myservo.write(servoPos);
          delay(10);//wait 10 ms
        }else{//so it doesn't go below the minimum
          servoPos = minPos;
          myservo.write(minPos);
          delay(10);
        }
    break;
    case 3://floor pos
      servoPos = minPos;
      myservo.write(minPos);
      delay(10);
    break;
    case 4://deliver pos
      servoPos = deliverPos;
      myservo.write(servoPos);
      delay(10);
    break;
    case 5:// cheval pos
      servoPos = maxPos;
      myservo.write(servoPos);
      delay(10);
    break;
    case 6://low bar pos
//if extendeed
      rightArm.write(rightRetracted);
      leftArm.write(leftRetracted);
      //cheval=0;
      delay(10);    
      /*servoPos = lowBarPos;
      myservo.write(servoPos);
      delay(10);*/   
    break;
    case 7://extend
      rightArm.write(rightExtended);
      leftArm.write(leftExtended);
      //cheval=1;
      delay(10);
    break;   
  }
}
/*val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                          // waits for the servo to get there
 *//*
void armUp(){
   for(int u = 0;u<10;u++){  
    if(servoPos < maxPos){
        servoPos++;
        myservo.write(servoPos);
        delay(10);
      }
   }
   /*servoPos=servoPos+10;
   myservo.write(servoPos);
   delay(100);   
}
void armDown(){
   for(int d = 0;d<10;d++){  
     if(servoPos > minPos){
        servoPos--;
        myservo.write(servoPos);
        delay(10);
      }
   }
   /*servoPos = servoPos - 10;
   myservo.write(servoPos);
   delay(100);
}*/
void armStay(){
    myservo.write(servoPos);

}

