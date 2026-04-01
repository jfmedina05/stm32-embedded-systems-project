# Lab 6: Flash Commands (STM32)

## Overview
This project implements a flash memory command system on an STM32 microcontroller. The purpose of this lab is to design and build functionality that allows system data to be written to non-volatile flash memory, retrieved later, and erased when necessary. The system communicates through a UART-based command-line interface, enabling real-time interaction through a terminal. This implementation demonstrates how embedded systems can persist data across resets and power cycles using onboard flash memory.

## Objectives
- Implement flash memory read, write, and erase operations using STM32 HAL
- Design a UART-based command interface for user interaction
- Store data persistently in flash memory using sequential addressing
- Retrieve and display stored data over UART
- Understand flash memory organization, constraints, and usage on STM32 devices

## System Architecture
The system is divided into three main components: the command interface, the flash memory module, and the main application.

### Command Interface
The command interface handles all user input received over UART. Characters are received one at a time and stored in a buffer. When a newline (`\n`) or carriage return (`\r`) is detected, the buffered input is treated as a complete command. The command string is then compared against predefined commands using string comparison, and the corresponding function is executed.

Supported commands:
- `sample` → stores a timestamp into flash memory
- `data` → reads and displays stored flash data
- `log` → reads stored records (currently behaves the same as `data`)
- `erase` → erases flash memory and resets storage

If an invalid command is entered, the system responds with an error message.

### Flash Memory Module
The flash module is responsible for all low-level interactions with flash memory using STM32 HAL drivers.

#### Flash Write Operation
- Flash memory is unlocked before writing
- Data is written using 64-bit (double word) programming
- Each write stores a timestamp value
- After writing, flash is locked again
- The write address is incremented by 8 bytes after each record to allow sequential storage

#### Flash Read Operation
- Reading begins at a predefined base flash address
- The system reads sequentially until it reaches the most recent write location
- Each stored value is retrieved and printed via UART

#### Flash Erase Operation
- Flash is unlocked before erase
- A flash page is selected and erased
- The write pointer is reset to the base address
- Flash is locked after erase to prevent unintended writes

### Main Application
The main application initializes all required peripherals and runs the main execution loop.

#### Initialized Peripherals
- UART (USART2) → used for command input/output
- Timer (TIM2) → used to generate timestamps
- ADC → initialized but not used in this implementation
- RTC → initialized but not used in this implementation

#### Main Loop Behavior
- Continuously listens for UART input
- Buffers incoming characters
- Processes commands when a full input line is received
- Executes the corresponding command function

## Data Storage Design
Each record written to flash consists of a timestamp value (32-bit) stored within a 64-bit flash write operation.

### Storage Characteristics
- Flash writes occur in 8-byte increments
- Data is stored sequentially in memory
- No overwriting occurs unless flash is erased
- A pointer tracks the current write address
- Memory grows linearly as new data is added

## Command Execution Flow
1. User types a command in the UART terminal
2. Characters are received and stored in a buffer
3. When newline is detected, the command is parsed
4. The system compares the input string to known commands
5. The corresponding function is executed
6. Output is transmitted back to the user via UART

## Key Embedded Systems Concepts
This project demonstrates several important embedded systems concepts:
- Non-volatile memory usage and persistence
- Flash memory programming constraints and alignment
- UART-based command-line interfaces
- Memory addressing and pointer management
- Hardware abstraction using STM32 HAL drivers
- Sequential data logging in embedded systems

## Limitations
This implementation is a simplified version of a full flash data logging system.

Current limitations include:
- Only timestamps are stored (no sensor data such as voltage or temperature)
- No structured data format (no use of structs for records)
- No distinction between log and data records
- RTC is initialized but not used for real date/time tracking
- Only a single flash page is used
- No bounds checking for flash overflow
- No formatted output (e.g., CSV-style or structured logs)

## Future Improvements
To extend this project into a complete data logging system, the following improvements can be made:
- Implement structured records for sensor data and logs
- Integrate RTC for real-time date and time tracking
- Add record types (data vs log) with identifiers
- Format output for readability (CSV or structured format)
- Support multiple flash pages and dynamic memory allocation
- Add bounds checking and error handling for flash limits
- Integrate real sensor data (battery voltage, temperature, light sensor)
- Implement circular buffering or wear leveling techniques

## How to Run
1. Build and flash the program to the STM32 using STM32CubeIDE
2. Open a UART terminal with baud rate set to 115200
3. Enter commands: `sample`, `data`, `log`, `erase`

## Dependencies
- STM32 HAL Library
- STM32CubeIDE
- UART peripheral (USART2)
- Timer peripheral (TIM2)

## Conclusion
This project provides a foundational implementation of flash memory interaction on an STM32 microcontroller. It demonstrates how data can be written, stored, retrieved, and erased using non-volatile memory. The system establishes a complete pipeline from user input to persistent storage and retrieval, forming a strong base for more advanced embedded data logging applications.
