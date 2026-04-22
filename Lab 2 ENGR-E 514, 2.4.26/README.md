# Lab 2 – UART Command-Based LED Control

## Summary
In this lab, I implemented basic UART communication on the STM32L476RG to control the onboard LED (LD3) using commands entered through a serial terminal. The system uses USART2 to receive user input and `printf` retargeting to transmit feedback over UART.

User-entered commands are read using `scanf()` and compared against expected input strings. Based on the received command, the onboard LED is either turned ON or OFF.

---

## Implemented Features
- USART2 configured for serial communication  
- UART TX/RX enabled using STM32 HAL drivers  
- `printf()` retargeted to USART2 using `RetargetInit()`  
- Command input received from terminal using `scanf()`  
- LED control based on user-entered command  
- Terminal feedback provided for valid and invalid commands  

---

## Supported Commands

| Command | Description |
|--------|-------------|
| `LON`  | Turns LD3 LED ON |
| `LOF`  | Turns LD3 LED OFF |

---

## CLI Interaction

After startup, the system prompts the user to enter a command through the serial terminal.

Startup message:
```
System Up and Running
Enter command (LON / LOF):
```

### Valid Commands

Example:
```
LON
LED ON
```

Example:
```
LOF
LED OFF
```

### Invalid Command Handling

Example:
```
ABC
Unknown command: ABC (use LON or LOF)
```

After each command is processed, the system prompts the user again:
```
Enter command (LON / LOF):
```

---

## LED Control Logic

The user command is stored in a character buffer:

```c
char cmd[4] = {0};
```

UART input is received using:

```c
scanf("%3s", cmd);
```

The command is compared using `strcmp()`:

```c
if (strcmp(cmd, "LON") == 0)
{
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
    printf("LED ON\n\r");
}
else if (strcmp(cmd, "LOF") == 0)
{
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
    printf("LED OFF\n\r");
}
else
{
    printf("Unknown command: %s (use LON or LOF)\n\r", cmd);
}
```

---

## Hardware Used
- STM32L476RG Nucleo Board  
- USART2 for UART communication  
- LD3 Onboard LED  

---

## Serial Terminal Settings
To interact with the system, open a serial terminal with the following settings:

```
Baud Rate: 115200
Data Bits: 8
Parity: None
Stop Bits: 1
```

---

## Demonstrated Functionality
- UART-based command input from terminal  
- LED ON/OFF control using CLI commands  
- Serial feedback for valid and invalid commands  
- Continuous command prompting  

---