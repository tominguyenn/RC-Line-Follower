# Arduino RC Line Follower


- Designed and assembled a remote-controlled car using the Elegoo Smart Robot Car Kit V4 and Arduino Uno.
- Programmed motor control via IR remote, enabling directional movement and speed control.
- Integrated ultrasonic and IR sensors to develop an autopilot feature that allows the car to navigate a road playmat autonomously.
- Implemented line-following logic for path tracking.
- Gained experience in embedded systems, sensor interfacing, PWM motor control, and real-time debugging.


---

## Features
- Real-time brightness control with **PWM (LEDC)**.
- LCD feedback showing:
  - Current LED color
  - Next LED color
  - Brightness percentage
  - Button instructions
- Debounced button handling.
- Modular and extensible (future Wi-Fi/mobile app control planned).

---

## Hardware Requirements
- Arduino Nano ESP32
- 3 × LEDs (Blue, Red, Green)
- 3 × 220Ω resistors
- 1 × Potentiometer (10kΩ recommended)
- 20x4 I2C LCD module (e.g. GeeekPi IIC LCD2004)
- Push button
- Jumper wires and breadboard

---

## Wiring
- **LCD**: SDA → D8, SCL → D9, VCC → 5V, GND → GND
- **LEDs**: D2 (Blue), D3 (Red), D4 (Green) → through resistors → GND
- **Button**: D5 to GND (with internal pull-up)
- **Potentiometer**: middle pin → A0, sides → 3.3V and GND
