#include "SmartCar.cpp"

DeviceDriverSet_Motor AppMotor;
Application_xxx Application_ConquerorCarxxx0;
MPU6050_getdata AppMPU6050getdata;

SmartCar car;

void setup(){
  car.init();
  car.moveFoward(150);
  delay(1000);
  car.stop();
}

void loop(){

}