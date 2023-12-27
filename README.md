# Abstractions of ELEGO V4

By: Youwei Zhen 12/27/2023

**IMPORTANT:**
```
1. Install I2CDev.h library zip file by adding the zip file as a library.
2. All code should be written inside the Robot-Tour-Starter-Kit.ino file.
```

## Includes:
- Abstraction for cardinal movements:
    - moveForward(int speed) <- speed is an analog value between 0 - 255
    - moveBackward(int speed)
    - moveLeft(int speed)
    - moveRight(int speed)
    - turnLeft(int speed)
    - turnRight(int speed)

- Gyroscope handling
    - Self-corrects moveForward/Backwards angle
    - Close to perfect 90 degree turn for turnLeft/Right