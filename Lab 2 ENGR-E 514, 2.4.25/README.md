# Lab 2 – UART Communication and GPIO Control

## Overview
In this lab, we developed a microcontroller application using the STM32 HAL to implement serial communication and basic digital output control. The goal was to interface with the board over UART and control on-board hardware through user commands.

---

## Objectives
- Configure system clock and peripheral initialization using STM32Cube-generated code
- Initialize and use **USART2** for serial communication
- Retarget `printf`/`scanf` to UART for terminal interaction
- Configure GPIO for output control
- Implement a command interpreter using serial input

---

## Hardware/Software Used
- STM32 Nucleo development board  
- STM32CubeIDE  
- HAL (Hardware Abstraction Layer) libraries  
- Serial terminal (PuTTY, TeraTerm, etc.)

---

## System Configuration
The following peripherals were configured:

| Peripheral | Purpose |
|-----------|---------|
| **System Clock** | Set MCU operating frequency |
| **USART2** | Serial communication with PC terminal |
| **GPIO (LD3 Pin)** | Output control for onboard LED |

---

## Program Functionality

### 1. UART Retargeting
The UART interface was configured so that standard C I/O functions (`printf`, `scanf`) communicate over the serial terminal.

### 2. Startup Message
When powered on, the system prints a status message to confirm successful initialization.

### 3. Command Interpreter
The program continuously waits for user input from the terminal.

Supported commands:

| Command | Action |
|---------|--------|
| `LON` | Turns the LED **ON** |
| `LOF` | Turns the LED **OFF** |

If an invalid command is entered, an error message is displayed.

### 4. GPIO Control
Based on received commands, the program sets or resets the GPIO pin connected to the LED.

---

## Program Flow
1. HAL initialization  
2. System clock setup  
3. GPIO initialization  
4. UART initialization  
5. UART retargeting  
6. Print startup message  
7. Enter infinite loop  
   - Wait for user command  
   - Compare input string  
   - Control LED  
   - Prompt for next command  

---

## Key Concepts Learned
- UART communication using HAL drivers  
- Retargeting standard I/O to serial interfaces  
- String comparison in embedded systems  
- Polling-based command processing  
- GPIO output control  

---

## Outcome
By the end of this lab, the system could receive text commands from a PC terminal and control onboard hardware in real time, demonstrating basic embedded input/output interaction.

---
