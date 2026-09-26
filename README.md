# Automatic Turret (Pan-Tilt System)

An ESP32-based automatic targeting system that uses **dual ultrasonic sensors** to detect an object's position in real time and drives a **servo motor** to align a pan-tilt turret toward it no camera or vision pipeline required.

This repository accompanies the report *"Automatic Turret (Pan-Tilt System)"* (`raturret-project-thesis.docx`) and includes a short demo video (`automatic-turret-demonstration.mp4`).

> Muhammad Omais · Mustafa Ali · Ahmed Razi Ullah
> School of Electrical Engineering and Computer Science (SEECS), National University of Sciences and Technology (NUST), Islamabad, Pakistan

---

## Table of Contents

- [Overview](#overview)
- [Motivation](#motivation)
- [System Overview](#system-overview)
- [Theory of Operation](#theory-of-operation)
- [Mathematical Modeling](#mathematical-modeling)
- [Control Flow](#control-flow)
- [Repository Structure](#repository-structure)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [How to Run](#how-to-run)
- [Experimental Results](#experimental-results)
- [Future Work](#future-work)
- [Project Report and Demo](#project-report-and-demo)
- [References](#references)
- [Authors](#authors)

---

## Overview

The Automatic Turret (Pan-Tilt System) detects the position of an object using two ultrasonic sensors and adjusts the turret's orientation through a servo motor, all driven by an **ESP32** microcontroller running C++ firmware. It's built as a compact, low-cost alternative to camera-based tracking systems, aimed at applications like security, surveillance, and interactive installations.

The project also includes mathematical modeling of the servo motor's angular response (a first-order transfer function), which was used to characterize and tune the system's response time and stability.

---

## Motivation

Camera-based tracking is accurate but computationally expensive and often overkill for simple proximity-based targeting. This project explores a **lightweight, sensor-driven alternative**:

- Two ultrasonic sensors (HC-SR04) measure distance/position without any vision processing
- The ESP32 computes the required angular correction directly from time-of-flight data
- A servo motor executes the correction via PWM, closing the loop in real time
- The design leaves room for future expansion toward active target engagement (e.g., a projectile mechanism)

---

## System Overview

The turret operates as an integrated pan-tilt platform, controlled electronically by the ESP32 and mechanically by the servo motor.

- Two **HC-SR04 ultrasonic sensors** are mounted at fixed angular positions to detect an object's proximity within their respective fields.
- The **ESP32** processes the distance readings from both sensors to determine the object's relative position and computes the required angular correction.
- The **servo motor** receives a PWM control signal and adjusts its orientation proportionally to the difference between its current and desired angle.
- This forms a continuous feedback loop, allowing the turret to track an object as it moves across the detection field.

| Component | Role |
|---|---|
| ESP32 microcontroller | Sensor data processing, control logic, PWM generation |
| 2× HC-SR04 ultrasonic sensors | Object distance/position detection (range ~2–400 cm) |
| Servo motor | Pan-tilt actuation via PWM |
| UART | External configuration and serial debugging |

---

## Theory of Operation

1. **Object Detection** — The ultrasonic sensors emit pulses and measure time-of-flight to determine the target's distance.
2. **Data Processing** — The ESP32 compares the readings from both sensors to compute the angular displacement needed to align the turret with the object.
3. **Servo Control** — The ESP32 sends a PWM signal to the servo motor to adjust the turret's position accordingly.

This loop runs continuously, so the turret follows the object as it moves, while control logic keeps the motion smooth and avoids overshoot or oscillation.

---

## Mathematical Modeling

The servo motor's dynamic behavior was modeled as a **first-order system** relating the control input to the resulting angular displacement.

**Transfer function:**

```
G(s) = K / (τs + 1)
```

**Step response (time domain):**

Given the differential equation `τ·dθ(t)/dt + θ(t) = K·u(t)`, taking the Laplace transform and solving for a step input yields:

```
θ(t) = K(1 − e^(−t/τ))
```

**Angular velocity** is the derivative of displacement:

```
ω(t) = dθ(t)/dt = (K/τ)·e^(−t/τ)
```

This shows the servo's angular speed is highest immediately after a command and decays exponentially as it settles into position — classic first-order, low-pass behavior. The parameters `K` (gain) and `τ` (time constant) were refined using **curve fitting** against experimental step-response data, and verified with magnitude and phase response analysis, confirming stable, non-oscillatory behavior under the chosen control parameters.

---

## Control Flow

```
   ┌─────────────────┐        ┌─────────────────┐
   │ HC-SR04 Sensor 1│        │ HC-SR04 Sensor 2│
   └────────┬────────┘        └────────┬────────┘
            │     distance readings    │
            └───────────┬──────────────┘
                        ▼
                 ┌───────────────┐
                 │     ESP32     │  ← computes angular correction
                 └───────┬───────┘
                         │ PWM signal
                         ▼
                 ┌───────────────┐
                 │  Servo Motor  │  ← pan/tilt actuation
                 └───────────────┘
```

---

## Repository Structure

```
.
├── auitomatic-turret.ino              # ESP32 firmware (Arduino sketch): sensor reads, control logic, servo PWM
├── automatic-turret-demonstration.mp4 # Demo video of the turret tracking an object
├── raturret-project-thesis.docx       # Full project report: design, math modeling, results
└── README.md                          # You are here
```

---

## Hardware Requirements

- ESP32 development board
- 2× HC-SR04 ultrasonic distance sensors
- Servo motor (pan-tilt bracket recommended)
- Jumper wires / breadboard or custom PCB
- 5V power supply appropriate for the servo and ESP32

---

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) (or PlatformIO) with **ESP32 board support** installed
- USB drivers for your ESP32 board (e.g., CP210x/CH340, depending on the board)
- A serial monitor (built into Arduino IDE) for UART-based debugging/configuration

---

## How to Run

1. **Wire the hardware** — connect the two HC-SR04 sensors to the ESP32's GPIO pins (trigger/echo) and the servo's signal line to a PWM-capable pin, per the pin assignments in `auitomatic-turret.ino`.
2. **Open the sketch** — load `auitomatic-turret.ino` in the Arduino IDE (or PlatformIO), select your ESP32 board and correct COM port.
3. **Flash the firmware** — compile and upload the sketch to the ESP32.
4. **Power up and test** — place an object within the sensors' detection range (~2–400 cm) and observe the turret pan/tilt to track it. Use the serial monitor over UART to view sensor readings and debug output.

---

## Experimental Results

- **Object detection accuracy** — the ultrasonic sensors reliably detected objects within a range of roughly **2 to 400 cm**, with consistent readings across different surfaces and orientations.
- **Servo response** :the servo motor tracked the computed angular displacement accurately, exhibiting smooth rotation and precise positioning without overshoot.
- **System latency** : the delay between object detection and turret adjustment was minimal, giving near real-time tracking.
- **Model validation** : the servo's measured step response matched the first-order transfer function model closely, with curve-fitted parameters confirming expected exponential settling behavior.

See `raturret-project-thesis.docx` for the full magnitude/phase response plots and detailed discussion.

---

## Future Work

- **Object tracking and shooting** : integrating a mechanism to engage the detected object once it's within range
- **Advanced targeting algorithms** : more sophisticated prediction and tracking for moving objects (e.g., adaptive or PID-based control)
- **Camera integration** : adding visual feedback to improve targeting accuracy beyond ultrasonic-only sensing

---

## Project Report and Demo

- 📘 **Full write-up:** [`raturret-project-thesis.docx`](./raturret-project-thesis.docx) — literature review, system design, mathematical modeling, and experimental results.
- 🎥 **Demo video:** [`automatic-turret-demonstration.mp4`](./automatic-turret-demonstration.mp4)

---

