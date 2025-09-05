# Arduino RC Line Follower


- Designed and assembled a remote-controlled car using the Elegoo Smart Robot Car Kit V4 and Arduino Uno.
- Programmed motor control via IR remote, enabling directional movement and speed control.
- Integrated ultrasonic and IR sensors to develop an autopilot feature that allows the car to navigate a road playmat autonomously.
- Implemented line-following logic for path tracking.
- Gained experience in embedded systems, sensor interfacing, PWM motor control, and real-time debugging.


---

## Features
- Line Following: The car autonomously follows a black line on a white background using three infrared (IR) sensors placed in the front.
- Wiggle Search: If the car loses the line, it will wiggle left and right to find the line again.
- Simple Motor Control: The car uses PWM to control its speed and direction.
- Serial Debugging: Provides real-time data on sensor readings for debugging and analysis.

---

## Hardware Requirements
- Elegoo Smart Robot Car Kit V4.0:
- Arduino Uno
- TB6612FNG Motor Driver
- 3x IR Sensors (Left, Middle, Right)
- 2x DC Motors for movement

---

## Code Structure
Motor Control
- Functions like setRightMotor(), setLeftMotor(), and stopMotors() are used to control the movement of the car. The motor driver (TB6612FNG) receives PWM signals for motor speed and direction.

Line Following Algorithm
- The car detects the line using infrared (IR) sensors. Based on the sensor readings, it adjusts the motor speeds to keep the car on the line.
- If the car detects a curve, it adjusts the speed of the left or right motor to navigate the curve.

The logic uses the following states:
- Straight: Both motors are moving forward at the same speed.
- Hard Left Pivot: Left motor is stopped, and right motor is moving.
- Hard Right Pivot: Right motor is stopped, and left motor is moving.
- Curve Left: Left motor is slowed down, and right motor continues at base speed.
- Curve Right: Right motor is slowed down, and left motor continues at base speed.

Wiggle Search (Recovery)
- When the line is lost, the car alternates between moving left and right for a period (wiggle), searching for the line.

Serial Debugging
- Outputs real-time sensor values to the serial monitor to help with debugging and tuning of the line-following logic.
