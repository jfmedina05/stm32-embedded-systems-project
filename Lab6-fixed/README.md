#Lab 6 – Embedded CLI with Flash Logging, ADC, and Timer Capture

##Summary
In this lab, I developed an advanced embedded Command Line Interface (CLI) system on the STM32L432KC that integrates multiple peripherals including Flash memory, ADC, RTC, and Timer Input Capture.

The system is built around an interrupt-driven UART interface with a circular buffer, allowing reliable, non-blocking command input from a serial terminal. The CLI supports command parsing, execution, and logging functionality, while also displaying a dynamic prompt that includes the current system time and date from the RTC.

Additional hardware features include internal ADC measurements (such as temperature and voltage reference) and timer-based input capture for signal timing analysis.

--

##Implemented Features
-Interrupt-driven UART communication using USART2
-Circular buffer for safe and efficient RX data handling
-Line-based command parsing system
-Modular command execution framework (commands.c)
-Flash memory logging on system reset
-printf retargeted to UART for terminal output
-RTC integration with dynamic timestamped CLI prompt
-Internal ADC configuration using LL drivers
-ADC channel reading (VBAT, temperature sensor, VREFINT)
-Timer 2 Input Capture for measuring signal timing
-Non-blocking CLI architecture

--

##System Architecture
-UART Command Flow
USART2 RX Interrupt, Circular Buffer (rxbuf), Line Buffer (linebuf), Command Parser, execute_command(), Response (OK / NOK), Prompt Refresh

-Timer Input Capture Flow
External Signal → PA5 (TIM2 CH1), Timer Capture Event, Interrupt Triggered, Capture Value Stored, Processed in Main Application

-ADC Measurement Flow
Internal Sensor Selection, ADC Conversion (LL Driver), Raw Digital Value, Used for CLI Commands / Debugging

--

##CLI Behavior
-Command Execution
-User inputs commands through a serial terminal (115200 baud)
-Characters are buffered via interrupt
-Commands are executed upon pressing Enter (\r)

System returns:
-OK → command succeeded
-NOK → command failed
-Example Interaction
-02/07/2026 14:32:10 IULS> help

Available commands:
help - show commands
OK

02/07/2026 14:32:15 IULS> ping
pong
OK

02/07/2026 14:32:18 IULS> unknown
Unknown command: unknown
NOK

--

##Dynamic Command Prompt
The CLI prompt includes real-time RTC data and updates after each command.

Format:
MM/DD/YYYY HH:MM:SS IULS>
Example:
03/31/2026 08:30:12 IULS>

--

##Flash Logging
-On system startup, a log entry is written to Flash memory:
-flash_write_log("reset");
-This enables persistent tracking of system resets for debugging and analysis.

--

##Key Modules
uart_rx_buffer.c / .h:
-Implements circular buffer for UART RX
-Prevents data loss during high-speed input

retarget.c / .h:
-Redirects printf() to UART
-Enables easy debugging and CLI output

commands.c / .h:
-Parses and executes CLI commands
-Easily extendable for new functionality

-main.c:
-Integrates all peripherals and system logic
-Handles CLI processing loop
-Manages ADC, RTC, and Timer configuration

--

##Hardware Used
-STM32L432KC Nucleo Board
-USART2 (UART CLI communication)
-RTC Peripheral
-ADC1 (internal channels)
-TIM2 (Input Capture on PA5)
-LD3 Onboard LED
-Serial Terminal (PuTTY / TeraTerm)

--

##Peripherals Overview
-Peripheral	Purpose
-USART2	CLI communication
-RTC	Timekeeping and prompt display
-ADC1	Internal voltage/temp measurement
-TIM2	Input signal timing capture
-Flash	Persistent logging

--

##Notable Design Decisions
-Interrupt-driven UART ensures no data loss
-Circular buffer decouples ISR from main loop processing
-Non-blocking CLI improves responsiveness
-LL drivers used for ADC and Timer for lower-level control
-HAL used for RTC and UART for simplicity and portability
-Prompt updated dynamically to reflect real-time system state

--

##Demonstrated Functionality
-Real-time CLI interaction over UART
-Command parsing and execution
-Flash logging on reset
-RTC-based timestamped prompt
-Internal ADC readings
-Timer input capture setup for signal measurement

--
