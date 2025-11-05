# automatic-turret (Pan-Tilt System)

This project presents the design and implementation of an Automatic Turret (Pan-Tilt System) capable of detecting and tracking objects in real time using ultrasonic sensors and an ESP32 microcontroller. The system interprets distance data from the sensors, processes it through embedded control algorithms, and drives a servo motor to orient the turret toward the detected target.

The main goal of the project was to demonstrate how simple hardware components, when combined with intelligent control logic, can create an automated system capable of spatial awareness and responsive actuation. The turret system acts as a foundation for applications such as automated surveillance, security monitoring, and robotics research, where precise angular positioning and quick response are essential.

## Introduction

The motivation for developing this automatic turret stems from the need for compact, low-cost systems that can perform accurate real-time tracking without relying on cameras or computationally expensive vision algorithms.
By utilizing two ultrasonic sensors to detect the object’s distance and position, the system computes angular displacement data and commands the servo motor to align the turret accordingly.

The entire operation is handled by the ESP32 controller, which executes the signal processing and control logic in real time. The firmware was developed in C++, optimized for fast computation, and capable of handling sensor feedback and actuator control simultaneously.

Beyond static detection, the system was designed to support future expansion toward active target engagement, where the turret could be equipped with a projectile mechanism for demonstration purposes.

## System Overview

The turret operates as an integrated pan-tilt platform, controlled electronically through the ESP32 and mechanically by the servo motor.
Two HC-SR04 ultrasonic sensors are placed at fixed angular positions to detect the proximity of an object within their respective fields. The measured distance values are processed by the ESP32, which determines the relative position of the object and calculates the required angular correction.

The servo motor receives control signals through pulse-width modulation (PWM), adjusting its orientation proportionally to the difference between the current and desired angular position. This continuous feedback loop allows the turret to follow an object as it moves across the detection field.

The design emphasizes both accuracy and stability of movement. Mathematical modeling of the servo’s angular response was conducted to establish a transfer function relating input control voltage to angular displacement. The obtained model allowed prediction and tuning of response parameters such as rise time, settling time, and steady-state error.

## Theory of Operation

The ESP32 receives continuous distance data from the two ultrasonic sensors, which operate by emitting ultrasonic pulses and measuring the time required for their reflection to return. From these time-of-flight readings, the controller computes the target’s approximate position and sends a corresponding control signal to the servo.

The servo motor is modeled using a first-order transfer function, where the output angular displacement depends on the input voltage and the motor’s time constant. The relationship can be described by:

G(s) = K / (τs + 1)

This function expresses how the servo’s angular position changes over time when driven by a step input. In the time domain, the displacement θ(t) follows an exponential curve that approaches a steady value proportional to the control input. The derivative of θ(t) represents the angular velocity, which starts at a maximum and decreases exponentially, reflecting the damping behavior of the servo mechanism.

By fitting the experimental data to this model, the transfer function parameters were refined to accurately describe the servo’s performance, ensuring precise control under varying conditions.

## System Operation

In practical operation, the turret continuously scans its environment through the ultrasonic sensors. When an object is detected, the ESP32 interprets the differential distance data to determine the direction of motion and computes the angle required to align the servo toward the target.

The servo is then commanded to rotate, adjusting the turret’s orientation until the object is centered within the detection range. This process occurs in real time, allowing smooth and accurate tracking without abrupt movements.

The control loop ensures that the turret’s angular motion remains stable, preventing overshoot or oscillation. The communication between the controller and servo motor is maintained through PWM control signals, while UART connectivity allows external configuration and debugging through a serial interface.

## Mathematical Modeling and Simulation

The servo motor’s response was studied through mathematical analysis and experimental validation. Using the identified transfer function, the system’s step response was derived as:

θ(t) = K(1 − e^(−t/τ))

This equation represents the motor’s angular displacement over time, while its derivative gives the angular speed profile ω(t) = (K/τ)e^(−t/τ).

Using curve-fitting methods, the actual motor response was compared with the theoretical model. The results confirmed that the servo behaves as a first-order system, where speed decreases exponentially until the desired position is reached.
The simulation also included magnitude and phase response analysis, which verified the expected low-pass characteristics and stable behavior under the designed control parameters.

## Experimental Results

The complete system was built and tested to validate detection accuracy, response time, and control precision.
The ultrasonic sensors demonstrated reliable detection performance for targets within a range of approximately 2 to 400 cm, maintaining consistent readings across different surfaces and orientations.
The servo motor exhibited smooth rotation and precise positioning, successfully aligning with the target detected by the sensors.

The latency between object detection and servo response was minimal, ensuring near real-time tracking capability. The system was able to maintain continuous operation without instability or oscillation.
Overall, the experimental outcomes confirmed that the control algorithms executed on the ESP32 provided accurate angular displacement and stable servo performance, consistent with the predictions of the transfer function model.

