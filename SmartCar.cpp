#include "avr/wdt.h"
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"
extern DeviceDriverSet_Motor AppMotor;
extern Application_xxx Application_ConquerorCarxxx0;
extern MPU6050_getdata AppMPU6050getdata;

static bool debug = true;
const float offset = 0.0;

class SmartCar
{

private:
    // Call this after a turn is completed
    void updateYawReference()
    {
        AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
        yaw_So = Yaw; // Set the new reference yaw
    }

    int calculateGradualSpeed(float currentYaw, float targetYaw, int initialSpeed){
        // Calculate the remaining angle to turn
        float remainingAngle = abs(currentYaw - targetYaw);

        // Gradually reduce speed based on the remaining angle
        int speed = initialSpeed * (remainingAngle / 90); // Assuming 90 is the max turn angle
        if (speed < 50) speed = 50; // Enforce a minimum speed of 50

        // Turn logic
        return speed;
    }

    float closestMultipleOf90(float number) {
        return round(number / 90.0f) * 90.0f;
    }

public:
    void init()
    {
        // Serial.begin(9600);
        AppMotor.DeviceDriverSet_Motor_Init();
        AppMPU6050getdata.MPU6050_dveInit();
        delay(2000);
        AppMPU6050getdata.MPU6050_calibration();
    }

    void moveFoward(int speed)
    {
        updateYawReference();
        ApplicationFunctionSet_ConquerorCarMotionControl(
            ConquerorCarMotionControl::Forward,
            speed // 0-255
        );
    }

    void moveBackward(int speed)
    {
        updateYawReference();
        ApplicationFunctionSet_ConquerorCarMotionControl(
            ConquerorCarMotionControl::Backward,
            speed // 0-255
        );
    }

    void moveLeft(int speed)
    {
        ApplicationFunctionSet_ConquerorCarMotionControl(
            ConquerorCarMotionControl::Left,
            speed // 0-255
        );
    }

    void turnLeft(int speed)
    {
        AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
        float target = closestMultipleOf90(Yaw - 90 + offset);

        while (Yaw > target)
        {
            AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
            this->moveLeft(calculateGradualSpeed(Yaw, target, speed));
            if (debug)
                Serial.println("Current Yaw: " + String(Yaw) + " | target: " + String(target));
        }
        updateYawReference();
        this->stop();
        delay(125);
    }

    void moveRight(int speed)
    {
        ApplicationFunctionSet_ConquerorCarMotionControl(
            ConquerorCarMotionControl::Right,
            speed // 0-255
        );
    }

    void turnRight(int speed)
    {
        AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
        float target = Yaw + 90 + offset;

        while (Yaw < target)
        {
            AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
            this->moveRight(calculateGradualSpeed(Yaw, target, speed));
            if (debug)
                Serial.println("Current Yaw: " + String(Yaw) + " | target: " + String(target));
        }

        this->stop();
        updateYawReference();
    }

    void adjust(int speed){

        AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
        float target = closestMultipleOf90(Yaw);

        Serial.println("Adjust Target | Current Yaw: " + String(Yaw) + " | target: " + String(target));

        if (Yaw > target){ // too far right, turn left
            while (Yaw > target)
            {
                AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
                this->moveLeft(calculateGradualSpeed(Yaw, target, speed));
                if (debug)
                    Serial.println("Adjusting | Current Yaw: " + String(Yaw) + " | target: " + String(target));
            }
        }else{ // too far left, turn right
            while (Yaw < target)
            {
                AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
                this->moveRight(calculateGradualSpeed(Yaw, target, speed));
                if (debug)
                    Serial.println("Adjusting | Current Yaw: " + String(Yaw) + " | target: " + String(target));
            }
        }

        this->stop();
    }

    void stop()
    {
        ApplicationFunctionSet_ConquerorCarMotionControl(
            ConquerorCarMotionControl::stop_it,
            0);
    }
};