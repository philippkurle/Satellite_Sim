# Satellite_Sim - AI-Assisted Embedded Systems Learning Project

This repository documents Satellite_Sim, a simplified satellite subsystem simulator built on an Arduino Mega.

## Context

I started this project as a self-directed semester-break learning project after I was unable to take an assistant position in a university lab, as I had originally planned.  
The goal was to gain hands-on experience in embedded systems, hardware integration, and system-oriented thinking by developing a simplified satellite subsystem simulator on an Arduino Mega.  
The software implementation was heavily AI-assisted, as I had only very limited knowledge of C++ and object-oriented programming when I started the project.

## Project Overview

Satellite_Sim simulates a simplified satellite system in which subsystem behavior and availability are determined by the simulated energy input from a photovoltaic (PV) solar cell.

The Arduino Mega is powered by a 5 V USB power bank. The micro servo is supplied separately through an additional 5 V output from the same power bank. The solar cell is not used as the primary power source for the controller; instead, its measured voltage is used as a proxy for available solar energy. To keep the PV input within a safe measurement range for the Arduino’s analog pins, I used a 10 kΩ / 10 kΩ voltage divider to halve the applied voltage. During testing under direct sunlight, the solar cell produced approximately 5.12 V.

Based on the measured PV voltage and the simulated leak status from the water-level sensor, the system operates in one of three modes:

- **NOMINAL** - normal operation with full system availability
- **WARN** - limited operation with reduced telemetry duty cycle
- **SAFE** - safety-oriented system behavior with strict telemetry duty cycle

These modes determine which subsystems remain responsive and how they react.

## Development Snapshot

media/prototype_picture(2).jpeg

*Early integrated prototype during subsystem testing. Visible components include the Arduino Mega, RFID module, keypad, LCD, traffic-light mode indicator, buzzer, 4-digit display, and servo.*

## Main Subsystems

- **Mode Control**  
  Sets the current system mode (**NOMINAL**, **WARN**, or **SAFE**) based on PV voltage thresholds and the simulated leak input from the water-level sensor.
- **Sensors**  
  The solar cell is used to monitor input voltage, while the water-level sensor is used to simulate a leak condition that can influence mode behavior. 
- **User Interface (UI)**  
  A 16x2 I²C LCD displays relevant system information such as PV voltage, leak status, security state, and the entered PIN. A buzzer generates a periodic safe-beacon signal in **SAFE** mode, and a traffic light module indicates the current system mode (**green = NOMINAL**, **yellow = WARN**, **red = SAFE**).
- **Security**  
  The security subsystem uses four states: **WAIT_RFID**, **WAIT_CODE**, **LOCKOUT**, and **AUTH_OK**. Access is granted in two stages: first through RFID authentication, then through PIN entry. If the entered PIN is correct, the system transitions to **AUTH_OK**. After three failed PIN attempts, the system enters **LOCKOUT**.
- **Actuator Control**  
  If the security state is **AUTH_OK** and the system is operating in **NOMINAL** mode, the micro servo performs a short movement to simulate an actuator response.

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
- SG90 9 g micro servo
- ULN2003 driver module
- 100 nF and 47 µF capacitors
- USB power bank
- Breadboard and jumper wires

## What I Learned

This project gave me firsthand exposure to integrating Arduino-based hardware, reading AI-assisted C++ code beyond my original skill level, and debugging a multi-subsystem embedded prototype.

### Embedded Hardware

- Built and tested functional circuits that combined several electronic components into a working prototype.
- Learned how to evaluate unfamiliar hardware experimentally. For example, I tested the water-level sensor with a short Arduino script to determine whether its output behaved as an analog or digital signal.
- Improved my understanding of how sensors, displays, and actuators are connected to and controlled by a microcontroller.
- Learned to account for electrical limits in the hardware design. To keep the PV input voltage within a safe measurement range for the Arduino Mega, I used a 10 kΩ / 10 kΩ voltage divider before feeding the signal into an analog input. This taught me that sensor integration is not only about code, but also about ensuring that electrical signals are safe and measurable.
- Gained practical experience with power distribution. The micro servo was supplied through a separate 5 V output path from the power bank to reduce the risk of voltage instability during movement. This gave me a first practical understanding of how actuator loads can influence overall system stability.
- Used a ULN2003 driver stage for the buzzer instead of driving it directly from an Arduino pin, which reduced the electrical load on the microcontroller and introduced me to indirect load driving.
- Learned how to identify the row and column structure of the keypad matrix experimentally by pressing keys one by one and tracing the corresponding connections with a continuity tester. This helped me understand how the keypad wiring relates to the software input logic.

### Software and C++

- Some parts of C++ felt familiar because of my previous exposure to Python. Concepts such as arrays, variables, constants, loops, conditional logic, and functions gave me a starting point, even though the overall structure and syntax were new to me. I also encountered `enum class` definitions for the first time and began to understand their role in representing clearly defined system states.
- This project introduced me to concepts I had not worked with before, such as classes, structs, and namespaces. It also helped me understand that, unlike in Python, C++ requires variables, function parameters, and return values to be declared with explicit types such as `uint8_t`, `bool`, or `void`.
- I also learned about fixed-width integer types such as `uint8_t`, `uint16_t`, and `uint32_t`, which were completely new to me at the start of the project. Through that, I gained a first understanding of the difference between signed and unsigned integer types and why a developer might choose one over another.
- While working through the codebase, I began to understand how the project was organized across header files and `.cpp` source files. I learned that header files define interfaces such as declarations, types, and class structure, while larger function implementations are placed in the corresponding `.cpp` files.
- I was also introduced to access control through `public` and `private` sections in classes. At a basic level, this helped me understand that some parts of a class are intended to be accessible from outside, while others are meant to remain internal to the class itself.
- Another thing I learned was how subsystem-specific files, such as sensor-related headers and source files, connect to the main program structure and are included where needed.
- This project also gave me my first practical exposure to the Arduino program structure built around `setup()` and `loop()`. In addition, the subsystems in this project were organized with `begin()` functions for initialization and `update()` functions for recurring execution, which helped me understand more clearly how initialization and repeated runtime behavior can be separated.
- Through the `update(uint32_t now_ms)` pattern, I gained a surface-level understanding of non-blocking program flow. I began to see why embedded software often needs regular updates across multiple subsystems without allowing one part of the program to stop the others from running.
- I also encountered C++ keywords such as `inline`, `constexpr`, and `static` for the first time. While my understanding of them is still basic, I now at least have a first conceptual idea of the role they can play in code structure and behavior.
- Working with structs taught me that they can be useful for grouping related variables into a single, more organized unit. Creating instances of those structs then helped me see how concrete values can be assigned and managed more clearly.
- More generally, I gained an entry-level understanding of how objects and instances of user-defined types such as structs and classes are created and used within a larger codebase.
- I also developed a first practical understanding of how actuator control is handled in software. In the servo logic, this included initialization, regular update checks, verifying whether movement was allowed, clamping requested angles to safe limits, counting movements, attaching and detaching the servo, and finally issuing the actual movement command with `.write(angle)`.
- One of the most important things I learned from this project was the difference between understanding what a piece of code does and understanding why it was designed in a certain way. In many places, I can now follow the functional behavior of the code and explain its purpose at a basic level. At the same time, I also became aware that understanding the underlying design decisions, such as why a specific abstraction, class structure, or language feature was chosen, requires a much deeper level of software knowledge. That distinction was one of the most useful lessons from the project for me.

### System Debugging

- Some parts of the system did not work as intended at first, and I had to track down smaller logic mistakes in the code. In a few cases, this meant correcting conditional checks by inverting boolean logic where the original condition did not match the intended behavior.
- I also learned that some subsystem behavior needed additional policy logic beyond the original implementation. For example, mode control was initially driven only by PV voltage, and I extended it so that simulated leak conditions could also trigger mode changes. In that logic, I made sure that leak handling could escalate the current mode when necessary, but would not incorrectly de-escalate a mode already set by the power conditions.
- I learned how useful targeted serial prints can be for debugging. By printing selected variable values and function calls to the terminal, I was able to check whether certain code paths were reached and what state important variables had at specific moments during runtime.
- I also learned to narrow down faults by commenting out individual functions or subsystem `update()` calls step by step. This helped me reduce the search area when something was not working correctly and made it easier to isolate the source of a problem.
- One of the more important debugging issues was the RFID handling. The MFRC522 library calls I was using introduced too much blocking time in the main loop, which made keypad input less responsive. To investigate that, I traced the relevant timeout behavior inside the `miguelbalboa/MFRC522` library, copied the library into the project tree, and reduced the blocking timeout in the local version. This improved how reliably keypad inputs were registered during normal operation.
- More generally, this part of the project showed me that debugging embedded systems often means looking at both software logic and timing behavior. A problem was not always caused by “wrong code” alone, but sometimes by the way one subsystem affected the responsiveness of the others during runtime.

### Reflection and Known Limitations

- Looking back, I know that this project was more ambitious than my original skill level realistically supported. I started it without prior experience in C++ or object-oriented programming, so when I began working on the first subsystem in more detail, I quickly realized how much larger the scope was than I had expected.
- At the same time, I am very glad that I still chose to go through with it. Working on something that was clearly beyond my comfort zone forced me to adapt quickly and pushed me to keep learning throughout the project. One of the most rewarding parts for me was seeing self-assembled hardware come to life and interact as a functioning system.
- At this stage, I can understand most parts of the code at least on a functional level, although the depth of that understanding still varies between subsystems. I feel more confident with simpler parts such as the Buzzer Beacon and the Mode Light, and I have a more solid overall understanding of subsystems such as Sensors, Mode Control, and parts of the actuator logic. Other parts, such as keypad handling, RFID integration, and some parts of the LCD logic, remain less intuitive to me and would require further study to understand fully from a software-design perspective.
- One subsystem I eventually chose to remove was a 4-digit display that was originally intended to show PV voltage continuously. I spent a significant amount of time mapping its pins and segments experimentally, testing it safely with resistors, and distinguishing between common-anode and common-cathode behavior. Although I was eventually able to understand the hardware mapping, I was not satisfied with the display quality and stability in the final setup, especially with regard to flicker. Because this part of the project was consuming a disproportionate amount of time, I removed it from the active build and archived that part of the code instead.
- If I were to approach the project again with my current knowledge, I would probably focus on only two or three subsystems and try to develop them myself with minimal or no AI support, with the goal of understanding them more deeply from both the implementation and design side. At the same time, I would still choose a project with a broader system context, because working on something larger gave me a much better feel for the complexity, integration effort, and trade-offs involved in projects that are closer to real technical systems.
- Overall, this project confirmed that I enjoy learning by doing, especially when hardware, software, and system behavior come together in one prototype. It was a strong complement to the more theoretical learning during the semester, and it also showed me how interested I am in continuing to build my programming skills, especially in Python and C++.

## Potential Next Steps

- Reimplement one or two selected subsystems with minimal AI support to deepen my understanding of both the implementation and the design decisions behind them.
- Simplify parts of the current software structure where the architecture became more complex than necessary for the current prototype.
- Improve the documentation of subsystem interactions, operating modes, and signal flow to make the system easier to understand and explain.
