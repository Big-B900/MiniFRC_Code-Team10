#include <Alfredo_NoU.h>
//#include <ESP32Encoder.h>

NoU_Servo servo1(16);
NoU_Servo servo2(17);
NoU_Motor telescopeMotor(4);
//ESP32Encoder encoder;


int encoderAPort = 18, encoderBPort = 19,//change to make the right ports
encoderState, encoderLastState, telescopeError, liveTelescopePos = 0;
int zeroCommand, telescopeCommand=1000, i;

void setup() {
    Serial.begin(9600);
    RSL::initialize();
    RSL::setState(RSL_ENABLED);
    servo1.write(85);
    servo2.write(90);
    /*pinMode (encoderAPort,INPUT);
    pinMode (encoderBPort,INPUT);
    encoder.setCount(0);
    Serial.println("Encoder Start = "+String((int32_t)encoder.getCount()));
    encoder.attachHalfQuad(18, 19);*/
    //pinMode (35, INPUT);

}

void loop() {
 //just repeatedly moves it and out
  /*i++;
  if(i%200<100){
  telescopeMotor.set(.7);
  delay(1);}else{
  telescopeMotor.set (-.7);
  delay(1);}   
liveTelescopePos = -encoder.getCount();
telescopeError = telescopeCommand-liveTelescopePos;
if(abs(telescopeError)<20){//deadband - close enough
  telescopeMotor.set(0);
}else if(telescopeError>0){
  telescopeMotor.set(.4);
}else{
  telescopeMotor.set(-.5);
}*/

//Serial.println(String(digitalRead(35)));
  
  //Serial.println("Encoder count = "+String((int32_t)encoder.getCount()));


}
