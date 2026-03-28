# Satellite_Sim - AI-Assisted Embedded Systems Learning Project

This repository documents Satellite_Sim, a simplified satellite subsystem simulator I built on an Arduino Mega.

## Context

I started this project as a self-directed semester-break learning project after I was unable to take an assistant position in a university lab, as I had originally planned.
The goal was to gain hands-on experience in embedded systems, hardware integration, and system-oriented thinking. I hoped to achieve this by developing a simplified satellite subsystem simulator on an Arduino Mega.
The software implementation was heavily AI-assisted, as I had only very limited knowledge of C++ and object-oriented programming when I started the project.

## Project Overview

Satellite_Sim simulates a simplified satellite system where subsystem behavior and availability are determined by the simulated energy input from the photovoltaic (PV) solar cell.

The Arduino Mega is powered from a 5 V USB power bank, as well as the micro servo, which is supplied separately from an additional 5 V output. The solar cell is not used as the primary power source for the controller, but its measured voltage is used as a proxy for available solar energy. I implemented a 10 kΩ / 10 kΩ voltage divider to halve the applied PV voltage. By that, I reduced the risk of damaging the Arduino´s analog pins. The solar cell produced approximately 5.12 V during testing under direct sun exposure.

Based on the measured PV voltage, as well as the simulated leak status from the water-level sensor, the system operates in one of three modes:

- **NOMINAL** - normal operation with full system availability
- **WARN** - limited operation and decreased duty-cycle telemetry
- **SAFE** - safety-oriented system behavior with strict duty-cycle telemetry

These modes determine which subsystems will remain responsive and how they react.

### Main Subsystems

- **Mode Control**
  Sets the current system mode (**NOMINAL**, **WARN**, or **SAFE**) based on PV voltage thresholds and the simulated leak input from the water-level sensor.
  
- **Sensors**
  The solar cell is used to monitor input voltage, while the water-level sensor is used to simulate a leak condition that can influence mode policy.
   
- **User Interface (UI)**
  A 16x2 I²C LCD displays relevant system information such as PV voltage, leak status, security state, and entered PIN. A buzzer generates a periodic safe-beacon signal in **SAFE** mode, and a traffic light module displays the current system mode (**green = NOMINAL**, **yellow = WARN**, and **red = SAFE**).
  
- **Security**
  The security subsystem uses four states: **WAIT_RFID**, **WAIT_CODE**, **LOCKOUT**, and **AUTH_OK**. Security access is granted in two stages: first through RFID authentication, then through PIN entry. If the entered PIN is correct, the system transitions to **AUTH_OK**. After three failed PIN attempts, the system enters **LOCKOUT**.
  
- **Actuator Control**
  If the security state is set to **AUTH_OK** and the system is currently operating in **NOMINAL** mode, then the micro servo performs a short movement to simulate an actuator response. 

## My Contribution

I focused on:

- defining the project concept and simplifying the satellite system idea
- selecting components and assembling the physical hardware prototype
- using AI as a tool to generate, refine, and iterate on the software implementation
- testing hardware behavior and debugging the software with AI support
- documenting the system structure, hardware setup, and project context
- analyzing, annotating, and learning key parts of the software implementation

## Technologies and Components

### Development

- C++
- PlatformIO

### Hardware

- Arduino Mega 2560
- PV solar cell
- 10 kΩ / 10 kΩ voltage divider
- Analog water-level sensor
- 4x4 matrix membrane keypad (16-key)
- RC522 RFID reader/writer module
- 16x2 I²C LCD
- HYT-1205 buzzer
- Traffic light module
- SG90 9g micro servo
- ULN2003 driver module
- 100 nF and 47 µF capacitors
- Power bank as external supply
- Breadboard and jumper wires

## What I Learned

This project gave me first-hand exposure on topics such as setting up and integrating Arduino hardware, reading AI-assisted C++ code beyond my skill level and debugging complex embedded systems.

### Embedded Hardware

- Built and tested functional circuits that combined multiple electronic components into a working prototype rather than isolated single-part experiments.
- Learned how to examine unfamiliar components experimentally. For example, I tested the water-level sensor by connecting it to the Arduino, applying water to the sensing surface, and reading its output values in a short test program to determine whether it behaved as a digital or analog sensor.
- Developed a better understanding of how different hardware elements interface with a microcontroller, including the distinction between sensor inputs, status/display outputs, and actuator-driven components.
- Learned to work within the electrical limits of the hardware. Since the Arduino Mega’s analog input pins should only receive voltages within a safe measurement range, I used a 10 kΩ / 10 kΩ voltage divider to scale the PV input voltage before measuring it.
- Gained practical awareness of power supply considerations when integrating actuators. The micro servo was supplied through a separate 5 V output path from the power bank so that its current draw would be less likely to disturb the Arduino supply during movement.
- Used the ULN2003 driver as an intermediate driver stage for the buzzer instead of driving it directly from an Arduino pin. This helped reduce the electrical load on the microcontroller output and was a useful introduction to the idea that some components should be driven indirectly rather than connected straight to a digital pin.
