#include <ADXL345.h>
#include <bma180.h>
#include <HMC58X3.h>
#include <ITG3200.h>
#include <MS561101BA.h>
#include <I2Cdev.h>
#include <MPU60X0.h>
#include <EEPROM.h>

//#define DEBUG
#include "DebugUtils.h"
#include "CommunicationUtils.h"
//I think this is the only one that might be needed...
#include "FreeIMU.h"

#include <Wire.h>
#include <SPI.h>#include <Servo.h>


//this program is to test the freeimu without the use of serial

FreeIMU myIMU = FreeIMU();
Servo myservo;
int pin = 9;//pin for the servo
int command;
float ypr[3];//creates yaw pitch roll array of floats
int mode = 0;//0 = Yaw, 1 = roll(technically pitch), 2 = pitch (technically yaw)

void setup() {
  // put your setup code here, to run once:
  delay(5);
  myIMU.init();
  delay(5);
  myservo.attach(pin);
  myservo.write(0);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  myIMU.getYawPitchRoll(ypr);
  command = (int)(abs(ypr[mode]));
  
  
  myservo.write(command);//write the right number...
  delay(10);
}
