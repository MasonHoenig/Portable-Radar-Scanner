# Portable Radar Scanner

A portable radar detection system built with an Arduino Uno and an HC-SR04 ultrasonic sensor mounted on a 180° servo motor. As the sensor sweeps, distance readings are collected at each angle and sent to a Java application where they are serialized and visualized in real time as a radar-style map. An RGB LED provides distance-based visual feedback.

## Features

- 180° sweep using a servo motor mounted HC-SR04 ultrasonic sensor
- Real-time radar-style visualization in a Java application
- Distance-based RGB LED feedback:
  - 🔴 **Red** — object within 25 cm
  - 🟢 **Green** — object 26–50 cm away
  - 🔵 **Blue** — object over 50 cm away
  - ⚪ **White** — no object detected

## Tech Stack

- **Hardware:** Arduino Uno, HC-SR04 Ultrasonic Sensor, Servo Motor, RGB LED
- **Firmware:** Arduino (C++) — `RadarScanner.ino`
- **Visualization:** Java (Processing) — `Final_Project_Sketch.pde`

## Parts List

| Part | Quantity |
|---|---|
| Arduino Uno | 1 |
| Breadboard | 1 |
| HC-SR04 Ultrasonic Distance Sensor | 1 |
| HC-SR04 Mount | 1 |
| Servo Motor | 1 |
| RGB LED | 1 |
| Resistors | 4 |
| Jumper Cables (M-M) | 9 |
| Jumper Cables (M-F) | 4 |
| Adhesive Strips | — |

## Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Processing](https://processing.org/download)

### Setup

1. Clone the repository
```
git clone https://github.com/MasonHoenig/Portable-Radar-Scanner.git
```

2. Wire the components according to the circuit diagram in the repository

3. Open `RadarScanner.ino` in the Arduino IDE and upload it to your Arduino Uno

4. Note the COM port your Arduino is connected to

5. Open `Final_Project_Sketch.pde` in Processing, update the serial port to match your Arduino's COM port, and run the sketch

6. The radar visualization will launch and begin displaying distance readings in real time
