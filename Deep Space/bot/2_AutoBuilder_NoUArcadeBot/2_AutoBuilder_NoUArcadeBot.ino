#include <BluetoothSerial.h>
#include <Alfredo_NoU.h>
#include <ESP32Encoder.h>


BluetoothSerial ESP_BT; 
int armCommand = 12, activeSide=1,
currentArmPos=0, currentWristPos=0, currentTelescopePos=0, targetArmPos, 
targetWristPos, targetTelescopePos, liveTelescopePos, telescopeError, i, limitSwitch;

int x = 0, manualTelescope; //TEST VALUE ONLY
int in1 = 18, in2 = 19, climbCommand, auton, cargoAuto, ultraAuto1, cargoAutoSimple, cargoAutoSide,trigPin = 19, echoPin = 36, ultrasonicFollow, followDistance = 50, ultrasonicCm, ultrasonicError;

NoU_Motor LeftMotor(1);
NoU_Motor RightMotor(2);
NoU_Motor telescopeMotor(4);
NoU_Motor intakeMotor(3);
NoU_Servo armServo(16);
NoU_Servo wristServo(17);
ESP32Encoder encoder;


NoU_Drivetrain drivetrain(&LeftMotor, &RightMotor);

float throttle, rotation, telescopePower,activeSideInput, intakeCommand;
long lastTimePacketReceived = 0;
//preset positions for the arm, 
//θ1 is arm angle
//θ2 is wrist angle
//L is telescope length
//all measured from vertical - wrist can be + or -, arm must be positive
int presets[13][3]={//75,90,0 are zeroes -
  //update 85 is now 0 - why? idk
   {0,0,0},//no command - stay at current position
   {75,50,120},//position 1: hatch lvl 1:           θ1,θ2,L
   {25,-75,1500},//position 2: hatch lvl 2
   {10,-50,700},//position 3: hatch lvl 3
   {70,-45,400},//position 4: hatch ground in
   {75,0,100},//position 5: cargo in
   {20,80,0},//position 6: home: - not needed           DELETE
   {75,60,x},//position 7: cargo rocket 1 
   {30,-30,x},//position 8: cargo rocket 2
   {5,-55,800},//position 9: cargo rocket 3
   {30,-75,100},//position 10: cargo cargo
   {5,-30,1800},//position 11: cargo rocket top
   {0,0,0}//position 12 is 0s bc not used             DELETE
  };




//autoBuilder Stuff
int autoBuilderCommand = 0;
float autoBuilderTasks[50] = {
  999,1,1,0,1500,//drive forward
  999,1,0,1,350,//turn 90 ish degrees
  999,1,.75,0,700,//drive forward a bit
  999,3,55,165,//arm to 55,165
  999,5,1,200,///intake out for .2 seconds
  999,1,-1,0,400,//drive back
  999,1,0,1,400,//turn
  999,1,1,0,800,//drive forward
  9999,//end command39 slots taken
  0,0,0,0,0,0,0,0,0 //excess
  };
  //the array will be a series of commands with break signals after each command and an ending signal
//999 = start of a task
//9999 = end





void setup() {
    Serial.begin(9600);
    //RSL::initialize();
    ESP_BT.begin("DreamBig_0");
    LeftMotor.setInverted(false);
    RightMotor.setInverted(false);
    //telescopeMotor.setInverted(false);
    //intakeMotor.setInverted(false);

    //zero the telescope
    //make it dumb
    liveTelescopePos = 0;
    pinMode (34,INPUT);
    pinMode (39,INPUT);
    encoder.setCount(0);
    //Serial.println("Encoder Start = "+String((int32_t)encoder.getCount()));
    encoder.attachHalfQuad(34, 39);
    pinMode(35, INPUT);
    //ledcAttachPin(in1,6);
    //ledcAttachPin(in2,7);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
    while (ESP_BT.available() > 0) {
        lastTimePacketReceived = millis();
        if ((ESP_BT.read()) == 'z') {
            throttle = -ESP_BT.parseFloat()*activeSide;
            rotation = -ESP_BT.parseFloat();
            
            //more inputs here
            activeSideInput = ESP_BT.parseFloat();
            //Serial.print("sidein: "+activeSideInput+" ");
            armCommand = ESP_BT.parseInt();
           // manualArmCommand = ESP_BT.parseFloat();
            intakeCommand = ESP_BT.parseFloat();
            manualTelescope = ESP_BT.parseInt();
            climbCommand = ESP_BT.parseInt();
            auton = ESP_BT.parseInt();
            cargoAuto = ESP_BT.parseInt();
            autoBuilderCommand = ESP_BT.parseInt();
            ultrasonicFollow = ESP_BT.parseInt();
            ultraAuto1 = ESP_BT.parseInt();
            cargoAutoSimple = ESP_BT.parseInt();
            cargoAutoSide = ESP_BT.parseInt();
        }
    }
    //actions here

    
//autobuilder stuff

//there should be an if statement here to just do it if some character is sent over serial (the start of the string)

autoBuilder();//fill the auto builder array

if(autoBuilderCommand == 1){//run the auto from autobuilder
  runAutoBuilder();
}
if(ultrasonicFollow==1){//testing with cm
  ultrasonicCm = getDistanceCm();//how far is the thing
  ultrasonicError = followDistance - ultrasonicCm;//how off is the thing from where we want it to be
  if (abs(ultrasonicError)<2){//if deadzone
    drivetrain.arcadeDrive(0,0);
  }else{
    drivetrain.arcadeDrive(-1*ultrasonicError/10.0,0);
  }
}
  //Serial.println("cm: "+(String)ultrasonicCm+" error: "+(String)ultrasonicError);




if(auton==1){
  drivetrain.arcadeDrive(1,0);
  delay(1000);
  drivetrain.arcadeDrive(0,0);
}if(cargoAuto==1){
  drivetrain.arcadeDrive(1,0);
  delay(1500);
  drivetrain.arcadeDrive(0,1);
  delay(350);
  drivetrain.arcadeDrive(.75,0);
  delay(700);
  drivetrain.arcadeDrive(0,0);
  armServo.write(55);
  wristServo.write(165);
  delay(200);
  intakeMotor.set(1);
  delay(200);
  intakeMotor.set(0);
  //end of cycle
  drivetrain.arcadeDrive(-1,0);
  delay(400);
  drivetrain.arcadeDrive(0,1);
  delay(400);
  drivetrain.arcadeDrive(1,0);
  delay(800);
  drivetrain.arcadeDrive(0,0);
  }
if(ultraAuto1 == 1){
  int targetDistance = 50;
  ultrasonicCm = getDistanceCm();//how far is the thing
  ultrasonicError = targetDistance - ultrasonicCm;//how off is the thing from where we want it to be
  while(abs(ultrasonicError)>5){
    drivetrain.arcadeDrive(-1*ultrasonicError/10.0,0);
    ultrasonicCm = getDistanceCm();//how far is the thing
    ultrasonicError = targetDistance - ultrasonicCm;//how off is the thing from where we want it to be
  
  }
  
}
if(cargoAutoSimple==1){
  drivetrain.arcadeDrive(1,0);
  delay(1500);
  drivetrain.arcadeDrive(0,1);
  delay(350);
  drivetrain.arcadeDrive(.75,0);
  delay(700);
  drivetrain.arcadeDrive(0,0);
  armServo.write(55);
  wristServo.write(165);
  delay(200);
  intakeMotor.set(1);
  delay(200);
  intakeMotor.set(0);
  //end of cycle
  }
if(cargoAutoSide==1){
  drivetrain.arcadeDrive(1,0);
  delay(1500);
  drivetrain.arcadeDrive(0,-1);
  delay(350);
  drivetrain.arcadeDrive(.75,0);
  delay(700);
  drivetrain.arcadeDrive(0,0);
  armServo.write(55);
  wristServo.write(165);
  delay(200);
  intakeMotor.set(1);
  delay(200);
  intakeMotor.set(0);
  //end of cycle
  }
    
//climbing now
if(climbCommand>0){
  ledcWrite(6,abs(climbCommand)*1023);
  ledcWrite(7,0);
}else if(climbCommand<0){
  ledcWrite(7,abs(climbCommand)*1023);
  ledcWrite(6,0);
}else{
  ledcWrite(7,0);
  ledcWrite(6,0);
}

    
limitSwitch = digitalRead(35);
//add intake here
intakeMotor.set(intakeCommand);
//telescopeMotor.set(manualTelescope*.75);

/*if(activeSideInput!=0){//if active side has a new command, update the active side
  activeSide=activeSideInput;
}*/

if(activeSideInput>0.5){
  activeSide = 1;
}if(activeSideInput<-.5){
  activeSide = -1;
}
//Serial.print("activeside: "+activeSide);

//setting the correct values for the arm
if(armCommand == 12){//starting confing (bc it's not mirrored)
  targetArmPos = 75; //CHANGE THESE TEST VALUES
  currentArmPos = 75;
  targetWristPos = 120;
  currentWristPos = 120;
  targetTelescopePos = 0;
  currentTelescopePos = 0;
}else{
  targetArmPos = 85-(activeSide*presets[armCommand][0]);//top position plus/minus (active side) preset angle;
  currentArmPos = presets[armCommand][0];
  targetWristPos = 90-(activeSide*presets[armCommand][1]);
  currentWristPos = presets[armCommand][1];
  targetTelescopePos = presets[armCommand][2];
  currentTelescopePos = presets[armCommand][2];
}
//writing the arm positions
armServo.write(targetArmPos);

wristServo.write(targetWristPos);

if(targetTelescopePos==0){//if the telescope should be retracted
  if(limitSwitch==0){//if limit switch isn't pressed
    telescopeMotor.set(-.7);
  }else{//if switch is pressed
    telescopeMotor.set(-.2);//to keep it there it needs a little power
    encoder.setCount(0);//zero the encoder

  }
}else if(targetTelescopePos<0){//manual control of elevator
  telescopeMotor.set(manualTelescope*.75);
}else{  //put the encoder stuff here
  liveTelescopePos = encoder.getCount();
  telescopeError = targetTelescopePos - liveTelescopePos;
  telescopePower = telescopeError/300.0;
  //Serial.print(telescopePower);
  telescopeMotor.set(telescopePower);
}

//*****should I add something to stall it a little bit and zero it when in the home position or make it a button?

//control loop for the telescope
//assume that there don't need to be sequences because there's no points below the halfline where the telescope is too extended
//ENCODER BASED TELESCOPE
/*
if(manualTelescope == 1){
      encoder.setCount(0);
}
liveTelescopePos = encoder.getCount();
telescopeError = targetTelescopePos + liveTelescopePos;
  telescopePower = telescopeError/300.0;
  Serial.print(telescopePower);
  telescopeMotor.set(telescopePower);
*/


//zero the arm automagially after it's been at home for a while
/*if(armCommand == 0){
  i++;s
}if(i>500){
  encoder.setCount(0);
  i = 0;
  Serial.println("zeroed arm");
}*/
//liveTelescopePos = encoder.getCount();
Serial.println(" raw encoder: "+String((int32_t)encoder.getCount())+" telescope pos: "+liveTelescopePos+" telescope error: "+ telescopeError);

//for P control, need to figure out the range of encoder and the range of power
//can it be just a multiple or does it need more math?


    //drivebase
    drivetrain.arcadeDrive(throttle, rotation);

    // RSL logichw
    /*if (millis() - lastTimePacketReceived > 1000) {
        RSL::setState(RSL_DISABLED);
    }
    else {
        RSL::setState(RSL_ENABLED);
    }
    RSL::update();*/
}

//example serial: y,999,1,1,0,400,999,1,0,1,350,9999;
//y says there is a command, 999 says there is a following action, 1 is the type of action, 1,0,400 are its arguments, 999 says theres another one... 9999 says end
int getDistanceCm(){
  long duration, cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
  return (int)cm;
}
int getDistanceInches(){
  long duration, inches;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = (duration/2) / 74; 
  return (int)inches;
}







void autoBuilder(){//fill auto builder array
  //get something from serial, put it into the array correctly
  //autoBuilderTasks = 
}
void runAutoBuilder(){//auto builder run
float value, modA, modB, modC, modD, modE;
int j, commandType;
  while(j<=50){//repeat until the end of the array (limited at 50 chars for now
    value = autoBuilderTasks[j];
    if(value==9999){//end of command
      break;//should exit the for loop hopefully
    }
    if(value = 999){///if there is a command 
      commandType = (int)autoBuilderTasks[j+1];//the type of command is the next character
      switch(commandType){
        case 1://timeDrive (float throttle, float rotation, int time)
        modA = autoBuilderTasks[j+2];//throttle
        modB = autoBuilderTasks[j+3];//rotation
        modC = autoBuilderTasks[j+4];//time
        drivetrain.arcadeDrive(modA,modB);
        delay(modC);
        drivetrain.arcadeDrive(0,0);
        j+=5;
        break;
        case 2://sensorDrive(int sensor, int target)
        //modA = autoBuilderTasks[j+2];//throttle
        //modB = autoBuilderTasks[j+3];//rotation
        modA = autoBuilderTasks[j+2];//sensor
        modB = autoBuilderTasks[j+3];//target
        switch((int)modA){
          case 0://ultrasonic
            while(abs(ultrasonicError)>2){
              ultrasonicCm = getDistanceCm();//how far is the thing
              ultrasonicError = modB - ultrasonicCm;//how off is the thing from where we want it to be
              drivetrain.arcadeDrive(-1*ultrasonicError/10.0,0);
              //add a timeout function in case the error is too small
            }
            drivetrain.arcadeDrive(0,0);
          break;
          case 1://imu
        


          
          break;
        }
        j+=4;        
        break;
        case 3://armCommandSimple(int armservo, int wristServo)
        modA = autoBuilderTasks[j+2];//armServo
        modB = autoBuilderTasks[j+3];//wristServo
        armServo.write(55);
        wristServo.write(165);
        delay(200);
        j+=4;
        break;
        case 4://armCommandAdvanced(int position, int side)
        j+=2;
        break;
        case 5://intakeCommand(float power, int time)
        modA = autoBuilderTasks[j+2];//power
        modB = autoBuilderTasks[j+3];//time
        intakeMotor.set(modA);
        delay(modB);
        intakeMotor.set(0);
        j+=4;
        break;
        case 6://delay(int time)
        modA = autoBuilderTasks[j+2];//
        delay(modA);
        j+=3;
        break;
        //add more cases for more functionality here
      }
      

      
    }
  }
  
}
