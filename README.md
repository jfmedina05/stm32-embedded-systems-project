# ES-SPRING-2026  
**Course Repository – Spring 2026**

**Team Members:**  
Jaiden Medina  
Brady Adams  

---

## Lab 1 — GitHub Setup & Light Sensor Project  
**Due:** January 26, 2025  

### Completed Tasks
- Created private IU GitHub repository: `ES-SPRING-2026`  
- Added required collaborators  
- Created and committed `Lab1_LightSensor_Report.pdf`  

---

## Lab 2 — UART Communication & GPIO Control  

### Completed Tasks
- Configured STM32 system clock and peripheral initialization  
- Initialized **USART2** for serial communication  
- Retargeted `printf`/`scanf` to UART terminal  
- Configured GPIO for onboard LED control  
- Implemented UART command interpreter  

### Program Features
- Prints startup status message on boot  
- Accepts terminal commands:  
  - `LON` → LED ON  
  - `LOF` → LED OFF  
- Displays error message for invalid input  

### Outcome
System successfully controls onboard LED through UART commands, demonstrating embedded serial I/O and digital output control.
