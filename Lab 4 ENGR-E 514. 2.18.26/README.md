# Lab 4 – Real-Time Clock (RTC) CLI Implementation

## Summary
In this lab, I extended my existing UART-based Command Line Interface (CLI) on the STM32L476RG to integrate the Real-Time Clock (RTC) peripheral. The system allows a user to configure the system time and date directly from the terminal and displays the current RTC time and date within the command prompt.

Additionally, RTC Alarm A was configured to generate an interrupt once per minute at 30 seconds past the start of the minute. When this interrupt occurs, the onboard LED (LD3) is flashed for 100 ms. In accordance with real-time embedded system design practices, the LED blinking logic is handled in the main loop using a flag set inside the interrupt service routine (ISR).

---

## Implemented Features
- RTC peripheral enabled and initialized using STM32 HAL drivers  
- UART CLI extended with new RTC configuration commands  
- Ability to set system time from terminal  
- Ability to set system date from terminal  
- Command prompt dynamically updates with current RTC time/date  
- RTC Alarm A configured to trigger periodically  
- Alarm interrupt used to blink LD3 LED for 100 ms  
- Interrupt handler kept non-blocking using flag-based design  

---

## CLI Commands Added

### Set Time
Sets RTC time in 24-hour format.

**Format:**
```
ts,hour,min,sec
```

**Example:**
```
IULS> ts,13,45,10
OK
```

Input validation ensures:
- 0 ≤ Hour ≤ 23  
- 0 ≤ Minute ≤ 59  
- 0 ≤ Second ≤ 59  

---

### Set Date
Sets RTC date.

**Format:**
```
ds,month,day,year
```

**Example:**
```
IULS> ds,5,31,19
OK
```

---

## Dynamic Command Prompt

The CLI prompt was modified to include the current RTC time and date.  
Each time a command completes, the RTC is queried and the updated prompt is printed.

**Prompt Format:**
```
MM/DD/YYYY HH:MM:SS IULS>
```

**Example Output:**
```
02/07/2026 09:48:52 IULS>
```

---

## RTC Alarm Functionality

RTC Alarm A is configured to trigger:
```
At 30 seconds past every minute
```

### Interrupt Callback
The RTC alarm interrupt does not directly control hardware.  
Instead, it sets a flag:

```c
alarm_flag = 1;
```

### Main Loop Handling
The main loop polls this flag and blinks the LED for 100 ms:

```c
if(alarm_flag)
{
    alarm_flag = 0;
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}
```

This ensures the ISR remains short and non-blocking.

---

## System Architecture

UART Command Flow:
```
UART RX Interrupt
        ↓
Circular Buffer
        ↓
Line Buffer
        ↓
Command Parser
        ↓
RTC Command Execution
        ↓
Prompt Update
```

RTC Alarm Flow:
```
RTC Alarm ISR
      ↓
Set alarm_flag
      ↓
Main Loop
      ↓
Blink LED (100 ms)
```

---

## Hardware Used
- STM32L476RG Nucleo Board  
- USART2 for CLI communication  
- RTC Peripheral  
- LD3 Onboard LED  

---

## Demonstrated Functionality
- CLI-based RTC time setting  
- CLI-based RTC date setting  
- Time/date displayed in command prompt  
- RTC alarm-driven LED blinking  
- ISR-safe LED handling using flag polling  

---