#include <BluetoothSerial.h>
#include <Alfredo_NoU.h>
#include <ESP32Encoder.h>


BluetoothSerial ESP_BT; 
int armCommand = 12, activeSide=1, activeSideInput, intakeCommand,
currentArmPos=0, currentWristPos=0, currentTelescopePos=0, targetArmPos, 
targetWristPos, targetTelescopePos, liveTelescopePos, telescopeError, i, limitSwitch;

int x = 0, manualTelescope; //TEST VALUE ONLY


NoU_Motor LeftMotor(1);
NoU_Motor RightMotor(2);
NoU_Motor telescopeMotor(4);
NoU_Motor intakeMotor(3);
NoU_Servo armServo(16);
NoU_Servo wristServo(17);
ESP32Encoder encoder;


NoU_Drivetrain drivetrain(&LeftMotor, &RightMotor);

float throttle, rotation, telescopePower;
long lastTimePacketReceived = 0;
//preset positions for the arm, 
//θ1 is arm angle
//θ2 is wrist angle
//L is telescope length
//all measured from vertical - wrist can be + or -, arm must be positive
int presets[13][3]={//75,120,0 are zeroes -
  //update 85 is now 0 - why? idk
   {0,0,0},//no command - stay at current position
   {75,60,x},//position 1: hatch lvl 1:           θ1,θ2,L
   {40,-5,0},//position 2: hatch lvl 2
   {10,-50,700},//position 3: hatch lvl 3
   {70,-45,400},//position 4: hatch ground in
   {75,0,100},//position 5: cargo in
   {0,0,0},//position 6: home: - not needed           DELETE
   {75,60,x},//position 7: cargo rocket 1 
   {30,-30,x},//position 8: cargo rocket 2
   {5,-55,800},//position 9: cargo rocket 3
   {30,-75,100},//position 10: cargo cargo
   {5,-30,1800},//position 11: cargo rocket top
   {0,0,0}//position 12 is 0s bc not used             DELETE
  };


void setup() {
    Serial.begin(9600);
    RSL::initialize();
    ESP_BT.begin("DreamBig_0");
    LeftMotor.setInverted(false);
    RightMotor.setInverted(false);
    //telescopeMotor.setInverted(false);
    //intakeMotor.setInverted(false);

    //zero the telescope
    //make it dumb
    liveTelescopePos = 0;
    pinMode (18,INPUT);
    pinMode (19,INPUT);
    encoder.setCount(0);
    Serial.println("Encoder Start = "+String((int32_t)encoder.getCount()));
    encoder.attachHalfQuad(18, 19);
    pinMode(35, INPUT);

}

void loop() {
    while (ESP_BT.available() > 0) {
        lastTimePacketReceived = millis();
        if ((ESP_BT.read()) == 'z') {
            throttle = -ESP_BT.parseFloat()*activeSide;
            rotation = -ESP_BT.parseFloat();
            
            //more inputs here
            activeSideInput = ESP_BT.parseInt();
            armCommand = ESP_BT.parseInt();
           // manualArmCommand = ESP_BT.parseFloat();
            intakeCommand = ESP_BT.parseInt();
            manualTelescope = ESP_BT.parseInt();
        }
    }
    //actions here
limitSwitch = digitalRead(35);
//add intake here
intakeMotor.set(intakeCommand);
//telescopeMotor.set(manualTelescope*.75);

if(activeSideInput!=0){//if active side has a new command, update the active side
  activeSide=activeSideInput;
}
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
  targetWristPos = 120-(activeSide*presets[armCommand][1]);
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
}else{//put the encoder stuff here
  liveTelescopePos = encoder.getCount();
  telescopeError = targetTelescopePos + liveTelescopePos;
  telescopePower = telescopeError/300.0;
  Serial.print(telescopePower);
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

Serial.println(" raw encoder: "+String((int32_t)encoder.getCount())+" telescope pos: "+liveTelescopePos+" telescope error: "+ telescopeError);

//for P control, need to figure out the range of encoder and the range of power
//can it be just a multiple or does it need more math?


    //drivebase
    drivetrain.arcadeDrive(throttle, rotation);

    // RSL logichw
    if (millis() - lastTimePacketReceived > 1000) {
        RSL::setState(RSL_DISABLED);
    }
    else {
        RSL::setState(RSL_ENABLED);
    }
    RSL::update();
}
