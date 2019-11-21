#include <BluetoothSerial.h>
#include <Alfredo_NoU.h>

BluetoothSerial ESP_BT;

NoU_Motor frontLeftMotor(1);
NoU_Motor frontRightMotor(2);
NoU_Motor rearLeftMotor(3);
NoU_Motor rearRightMotor(4);
NoU_Servo servo(16);

NoU_Drivetrain drivetrain(&frontLeftMotor, &frontRightMotor, &rearLeftMotor, &rearRightMotor);

float throttle, rotation, servoAxis;
long lastTimePacketReceived = 0;

void setup() {
    Serial.begin(9600);
    RSL::initialize();
    ESP_BT.begin("DefaultBot");
    frontLeftMotor.setInverted(false);
    frontRightMotor.setInverted(false);
    rearLeftMotor.setInverted(false);
    rearRightMotor.setInverted(false);
}

void loop() {
    while (ESP_BT.available() > 0) {
        lastTimePacketReceived = millis();
        if ((ESP_BT.read()) == 'z') {
            throttle = -ESP_BT.parseFloat();
            rotation = ESP_BT.parseFloat();
            servoAxis = ESP_BT.parseFloat();
            //add more driverstation inputs here
        }
    }

    
    servo.write((servoAxis + 1) * 90);//range 0-180


    
    //drivetrain logic
    if (fabs(throttle) < 0.2) {
        drivetrain.curvatureDrive(throttle, rotation, true);
    }
    else {
        drivetrain.curvatureDrive(throttle, rotation, false);
    }

    // RSL logic
    if (millis() - lastTimePacketReceived > 1000) {
        RSL::setState(RSL_DISABLED);
    }
    else {
        RSL::setState(RSL_ENABLED);
    }
    RSL::update();
}
