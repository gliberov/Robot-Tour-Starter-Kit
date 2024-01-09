#include "SmartCar.cpp"

DeviceDriverSet_Motor AppMotor;
Application_xxx Application_ConquerorCarxxx0;
MPU6050_getdata AppMPU6050getdata;

SmartCar car;

void setup(){
  Serial.begin(9600);
  car.init();
  car.moveForward(200);
  delay(1000);
  car.turnLeft(200);
  car.moveForward(200);
  delay(1000);
  car.stop();
}

void loop(){

}