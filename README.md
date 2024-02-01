# Abstractions of ELEGO V4

By: Youwei Zhen 12/27/2023

**IMPORTANT:**
```
1. Install I2CDev.h library zip file by adding the zip file as a library.
2. All code should be written inside the Robot-Tour-Starter-Kit.ino file.
```

**SETUP:**
```
1. All code should be written inside the Robot-Tour-Starter-Kit.ino file.
1. Make sure the folder that Robot-Tour-Starter-Kit.ino is named "Robot-Tour-Starter-Kit"
2. Install the I2CDev library. Sketch -> Include Library -> Add .ZIP Library -> Select the I2CDev.zip.
3. All code should be written inside the Robot-Tour-Starter-Kit.ino file.
```

## Includes:
- Abstraction for cardinal movements:
    - moveForwardForSeconds(int speed, int ms) <- drives **PERFECTLY** straight forward for ms milliseconds.
    - moveBackwardForSeconds(int speed, int ms) <- drives **PERFECTLY** straight backward for ms milliseconds.
    - moveForward(int speed) <- speed is an analog value between 0 - 255
    - moveBackward(int speed)
    - moveLeft(int speed)`
    - moveRight(int speed)
    - turnLeft(int speed) <- Turns approx. 90 deg
    - turnRight(int speed) <- Turns approx. 90 deg

- Gyroscope handling
    - Self-corrects moveForward/Backwards angle
    - Close to perfect 90 degree turn for turnLeft/Right