# STM32 Embedded Systems Project

<p align="center">
  <img alt="C" src="https://img.shields.io/badge/C-00599C?logo=c&logoColor=white">
  <img alt="STM32" src="https://img.shields.io/badge/STM32-03234B?logo=stmicroelectronics&logoColor=white">
  <img alt="STM32CubeIDE" src="https://img.shields.io/badge/STM32CubeIDE-03234B?logo=stmicroelectronics&logoColor=white">
  <img alt="STM32CubeMX" src="https://img.shields.io/badge/STM32CubeMX-03234B?logo=stmicroelectronics&logoColor=white">
  <img alt="STM32 HAL" src="https://img.shields.io/badge/STM32%20HAL-Firmware-1E4D8C?logoColor=white">
  <img alt="Nucleo L476RG" src="https://img.shields.io/badge/Nucleo%20L476RG-Development%20Board-2C8EBB?logoColor=white">
  <img alt="Makefile" src="https://img.shields.io/badge/Makefile-Build%20System-427819?logo=gnu&logoColor=white">
  <img alt="UART" src="https://img.shields.io/badge/UART-Serial%20Communication-00599C?logoColor=white">
  <img alt="USART2" src="https://img.shields.io/badge/USART2-Peripheral%20Interface-00599C?logoColor=white">
  <img alt="GPIO" src="https://img.shields.io/badge/GPIO-Digital%20I%2FO-2C8EBB?logoColor=white">
  <img alt="PCB Design" src="https://img.shields.io/badge/PCB%20Design-Hardware%20Layout-008080?logoColor=white">
  <img alt="Enclosure Design" src="https://img.shields.io/badge/Enclosure%20Design-Prototype%20Packaging-6f42c1?logoColor=white">
  <img alt="Power Analysis" src="https://img.shields.io/badge/Power%20Analysis-Battery%20Life-B30B00?logoColor=white">
  <img alt="GitHub" src="https://img.shields.io/badge/GitHub-181717?logo=github&logoColor=white">
  <img alt="Markdown" src="https://img.shields.io/badge/Markdown-000000?logo=markdown&logoColor=white">
</p>

An embedded systems design project focused on STM32 microcontroller programming, peripheral configuration, UART communication, sensor integration, PCB development, enclosure design, power analysis, and full system-level engineering.

This repository documents a multi-lab embedded systems workflow that progresses from basic STM32 firmware development to a complete embedded product prototype with firmware, hardware, mechanical packaging, power management, and cost analysis.

---

## Overview

This project explores the design and implementation of embedded systems using STM32 microcontrollers and STM32CubeIDE. The work includes low-level firmware development, GPIO control, UART communication, sensor interfacing, PCB layout, enclosure design, and system feasibility analysis.

The project emphasizes the full embedded systems development process:

```text
Firmware Development
        │
        ▼
Peripheral Configuration
        │
        ▼
Sensor and Hardware Integration
        │
        ▼
PCB and Enclosure Design
        │
        ▼
Power and Cost Analysis
        │
        ▼
Complete Embedded System Prototype
```

Rather than focusing only on code, this repository shows how embedded systems require coordination between software, electronics, mechanical design, power constraints, and manufacturability.

---

## Project Goals

- Develop embedded firmware using STM32 microcontrollers
- Configure GPIO, UART, and peripheral interfaces using STM32CubeIDE
- Build command-based serial communication using UART
- Interface firmware with sensors and external hardware
- Design PCB and hardware layouts for embedded system integration
- Create a protective enclosure for real-world deployment
- Evaluate power consumption and battery life
- Estimate prototype costs across production scales
- Practice complete system-level embedded design

---

## Technical Areas

| Area | Description |
|---|---|
| Embedded Firmware | C-based firmware development using STM32CubeIDE and STM32 HAL |
| GPIO Control | Digital input/output control for LEDs, buttons, and external devices |
| UART Communication | Serial command interface using USART and terminal input/output |
| Peripheral Configuration | STM32CubeMX/STM32CubeIDE configuration for microcontroller peripherals |
| Sensor Integration | Hardware/software integration for sensor-based embedded systems |
| PCB Design | Light sensor PCB layout and embedded hardware design |
| Mechanical Integration | Protective enclosure design for embedded electronics |
| Power Analysis | Battery life and operating-mode evaluation using STM32CubeMX |
| Cost Analysis | Prototype cost estimation across multiple production quantities |
| System Integration | Combining firmware, hardware, enclosure, power, and cost decisions |

---

## Repository Structure

```text
stm32-embedded-systems-project/
│
├── Lab 1 ENGR-E 514, 1.26.26/
│   └── Light sensor report and early embedded system analysis
│
├── Lab 2 ENGR-E 514, 2.4.26/
│   └── UART command-based LED control using STM32 firmware
│
├── Lab 3 ENGR-E 514, 2.11.26/
│   └── STM32CubeIDE project files and embedded firmware development
│
├── Lab 4 ENGR-E 514. 2.18.26/
│   └── STM32 firmware and hardware interfacing lab
│
├── Lab 5 ENGR-E 514, 3.4.26/
│   └── STM32 embedded systems development lab
│
├── Lab 6 ENGR-E 514, 3.27.26/
│   └── STM32 hardware/software integration lab
│
├── Lab 7 ENGR-E 514, 4.3.26/
│   └── STM32 embedded systems integration lab
│
├── Lab 8 ENGR-E 514, 4.10.26/
│   └── Light sensor PCB layout and board design
│
├── Lab 9 ENGR-E 514, 4.17.26/
│   └── Enclosure design and mechanical system integration
│
├── Lab 10 ENGR-E 514, 4.24.26/
│   └── Power management and prototype cost analysis
│
├── Final Report, 5.06.26/
│   └── Final embedded system design report
│
└── README.md
```

---

## Labs and Documentation

| Lab | Focus |
|---|---|
| [Lab 1 — Light Sensor Report](./Lab%201%20ENGR-E%20514%2C%201.26.26) | Early embedded system analysis focused on a light sensor application |
| [Lab 2 — UART Command-Based LED Control](./Lab%202%20ENGR-E%20514%2C%202.4.26) | STM32 UART communication and serial command-based LED control |
| [Lab 3 — STM32CubeIDE Firmware Development](./Lab%203%20ENGR-E%20514%2C%202.11.26) | STM32CubeIDE project setup and embedded firmware development |
| [Lab 4 — STM32 Hardware Interfacing](./Lab%204%20ENGR-E%20514.%202.18.26) | STM32 firmware and hardware interfacing |
| [Lab 5 — Embedded Systems Development](./Lab%205%20ENGR-E%20514%2C%203.4.26) | Continued STM32 embedded systems development |
| [Lab 6 — Hardware/Software Integration](./Lab%206%20ENGR-E%20514%2C%203.27.26) | STM32 hardware and software integration |
| [Lab 7 — System Integration](./Lab%207%20ENGR-E%20514%2C%204.3.26) | Embedded systems integration and testing |
| [Lab 8 — Light Sensor PCB Layout](./Lab%208%20ENGR-E%20514%2C%204.10.26) | PCB layout and board design for the light sensor system |
| [Lab 9 — Enclosure Design](./Lab%209%20ENGR-E%20514%2C%204.17.26) | Protective enclosure design and mechanical integration |
| [Lab 10 — Power and Cost Analysis](./Lab%2010%20ENGR-E%20514%2C%204.24.26) | Power management, battery life, and prototype cost analysis |
| [Final Report](./Final%20Report%2C%205.06.26) | Complete final embedded system design report |

---

## Featured Work

### [STM32 Firmware Development](./Lab%203%20ENGR-E%20514%2C%202.11.26)

The firmware portion of this project focuses on configuring and programming STM32 microcontrollers using C, STM32 HAL drivers, and STM32CubeIDE.

Key work includes:

- Configuring STM32 projects using STM32CubeIDE
- Writing embedded C firmware
- Using STM32 HAL drivers
- Controlling GPIO outputs
- Reading user input through serial communication
- Structuring firmware for reliable embedded behavior
- Building and debugging STM32 projects

---

### [UART Command-Based LED Control](./Lab%202%20ENGR-E%20514%2C%202.4.26)

One major firmware lab implemented UART-based command input to control an onboard LED through a serial terminal.

The system uses USART communication to receive user commands and send terminal feedback.

Supported commands:

```text
LON  → Turns LED ON
LOF  → Turns LED OFF
```

Example terminal interaction:

```text
System Up and Running
Enter command (LON / LOF):

LON
LED ON

LOF
LED OFF
```

Key features:

- USART2 serial communication
- UART transmit and receive
- `printf()` retargeting over UART
- `scanf()` command input
- String comparison using command buffers
- LED control through STM32 HAL GPIO functions
- Serial feedback for valid and invalid commands

---

### [Sensor and Hardware Integration](./Lab%206%20ENGR-E%20514%2C%203.27.26)

The project includes sensor-focused embedded system work, including light sensor analysis, board-level hardware development, and system integration.

Key work includes:

- Light sensor system analysis
- Sensor-aware enclosure design
- PCB layout considerations
- Physical access for communication connectors
- Hardware constraints influencing firmware and packaging decisions

---

### [PCB Design and Layout](./Lab%208%20ENGR-E%20514%2C%204.10.26)

The project includes PCB development work for a light sensor system.

Key work includes:

- Light sensor board layout
- Hardware design organization
- PCB manufacturing considerations
- Integration between circuit board dimensions and enclosure design
- Design decisions connected to physical system deployment

---

### [Enclosure Design](./Lab%209%20ENGR-E%20514%2C%204.17.26)

A major system integration lab focused on designing a custom enclosure for the STM32-based embedded system.

The enclosure was designed to:

- Protect electronics from environmental exposure
- Reduce moisture risk using a sealing mechanism
- Secure the PCB using integrated standoffs
- Provide access to the communication connector
- Use a clear lid so the light sensor can still function
- Align the mechanical design with PCB and sensor placement

Design components included:

```text
3D-printed enclosure base
Clear acrylic lid
O-ring sealing interface
PCB standoffs
Connector opening
Sensor exposure area
```

This part of the project shows how embedded systems extend beyond firmware. A working device also needs packaging, protection, accessibility, and mechanical reliability.

---

### [Power Management and Battery Analysis](./Lab%2010%20ENGR-E%20514%2C%204.24.26)

The final system analysis evaluated power consumption and battery life for the STM32-based embedded system.

The power analysis considered:

- Run modes
- Low-power run mode
- Sleep modes
- Stop modes
- Standby modes
- Sensor current draw
- Battery capacity
- Sampling frequency
- System operating schedule

The goal was to understand how firmware behavior, sampling rate, and low-power modes affect total system lifetime.

Key takeaways:

- Low-power modes can significantly extend battery life
- Wake-up latency and power efficiency must be balanced
- Sensor current matters alongside microcontroller consumption
- Embedded systems must be designed around real energy constraints

---

### [Prototype Cost Analysis](./Lab%2010%20ENGR-E%20514%2C%204.24.26)

The project also includes prototype cost analysis across different production quantities.

Manufacturing considerations included:

- PCB fabrication
- 3D-printed enclosure
- Laser-cut lid
- Small-batch prototype cost
- Per-unit cost scaling
- Cost tradeoffs between electronic and mechanical components

Production quantities analyzed included:

```text
5 units
20 units
50 units
100 units
```

This analysis connects technical design decisions with real-world feasibility and manufacturability.

---

## System-Level Design Flow

```text
STM32 Microcontroller
        │
        ▼
Firmware Configuration
STM32CubeIDE / STM32 HAL
        │
        ▼
Peripheral Interfaces
GPIO / UART / Sensors
        │
        ▼
PCB and Circuit Integration
Light Sensor Board
        │
        ▼
Mechanical Packaging
Enclosure / Lid / Standoffs / Seal
        │
        ▼
Power and Cost Analysis
Battery Life / Manufacturing Cost
        │
        ▼
Embedded Product Prototype
```

---

## Tools and Technologies

### Languages

- C

### Microcontroller and Firmware Tools

- STM32CubeIDE
- STM32CubeMX
- STM32 HAL drivers
- Makefile-based build components

### Hardware

- STM32 microcontroller boards
- STM32L476RG Nucleo board
- GPIO-connected LED hardware
- Light sensor circuit
- PCB layout files
- Battery-powered embedded system components

### Communication and Interfaces

- UART
- USART2
- GPIO
- Serial terminal communication

### Design and Analysis

- PCB layout
- Enclosure design
- Power consumption analysis
- Battery life estimation
- Prototype cost analysis
- System integration planning

---

## Skills Demonstrated

- Embedded C programming
- STM32 firmware development
- STM32CubeIDE project setup
- STM32CubeMX peripheral configuration
- GPIO control
- UART communication
- Serial command parsing
- HAL driver usage
- Sensor-aware embedded design
- PCB layout understanding
- Hardware/software integration
- Mechanical enclosure design
- Moisture protection and packaging considerations
- Power consumption modeling
- Battery life analysis
- Prototype cost estimation
- Engineering tradeoff analysis
- Complete embedded system design

---

## Representative Labs

### [Lab 1 — Light Sensor Report](./Lab%201%20ENGR-E%20514%2C%201.26.26)

Early embedded system analysis focused on a light sensor application and system-level design considerations.

### [Lab 2 — UART Command-Based LED Control](./Lab%202%20ENGR-E%20514%2C%202.4.26)

Implemented a UART command interface to control an onboard LED using serial terminal commands.

### [Lab 8 — Light Sensor PCB Layout](./Lab%208%20ENGR-E%20514%2C%204.10.26)

Developed PCB layout work for a light sensor board, connecting circuit design with physical board constraints.

### [Lab 9 — Enclosure Design](./Lab%209%20ENGR-E%20514%2C%204.17.26)

Designed a protective enclosure with standoffs, connector access, moisture protection, and a clear lid for sensor functionality.

### [Lab 10 — Power and Prototype Cost Analysis](./Lab%2010%20ENGR-E%20514%2C%204.24.26)

Modeled power consumption, battery life, and manufacturing cost across multiple production quantities.

### [Final Report](./Final%20Report%2C%205.06.26)

Compiled the complete embedded system design into a final report covering firmware, hardware, enclosure design, power strategy, and system feasibility.

---

## Why This Project Matters

This project demonstrates the full embedded systems engineering workflow. Real embedded systems are not only firmware projects. They require microcontroller programming, peripheral configuration, sensor integration, PCB design, physical packaging, power budgeting, and cost-aware engineering decisions.

The strongest part of this repository is that it shows the progression from low-level firmware to complete product thinking. It reflects the type of system-level engineering used in embedded devices, IoT systems, medical devices, industrial sensors, and hardware products.

---

## Future Improvements

- Reorganize lab folders into cleaner portfolio-style project folders
- Add screenshots of STM32CubeIDE configurations
- Add wiring diagrams and hardware photos
- Include enclosure CAD files or renderings in a dedicated `assets/` folder
- Add final report images directly into the README
- Add a `docs/` folder for reports and lab documentation
- Add build/run instructions for each STM32CubeIDE project
- Add serial terminal screenshots for UART demos
- Add power analysis tables directly into the README
- Add a short demo video or GIF for the UART LED control lab

---

## Author

**Jaiden Medina**  
Computer Engineering @ Indiana University  
Accelerated M.S. in Intelligent Systems Engineering  

- GitHub: [jfmedina05](https://github.com/jfmedina05)
- Portfolio: [www.jaidenmedina.com]( http://www.jaidenmedina.com/)
- LinkedIn: [jaiden-medina](https://www.linkedin.com/in/jaiden-medina)
