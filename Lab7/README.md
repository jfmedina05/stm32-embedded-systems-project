(README Version)
**Design Analysis of the Light Sensor**
Lab 7
Brady Adams & Jaiden Medina
April 3rd, 2026


Executive Summary

The goal for this Lab was to design an embedded light sensor system using Autodesk Fusion 360 Electronics. The system is centered around the STM32L432KC microcontroller and uses a TSL237 light to frequency sensor to measure light intensity. The schematic also includes an external low frequency oscillator, power management components, and a debugging interface. We also used the Embedded_Systems_Components v3 library provided by the instructor to complete this project.
The STM32L432KC is evaluated based on a bunch of design considerations that include package size, cost, memory, peripherals, power consumption, and vendor support as stated in the project description. Several alternative microcontrollers are also discussed as per instruction. These alternatives are compared to the STM32L432KC to determine whether the selected part is the best fit for this application.


System Overview

The Light Sensor Schematic showcases a compact, battery powered light sensing system. The core of the design is the STM32L432KC microcontroller, which interfaces with the TSL237 sensor and processes the sensor output digitally. A 32.768 kHz external oscillator is included for accurate timing. Power is supplied through a battery holder, and decoupling capacitors are placed throughout the design to ensure stable voltage levels.
Additional components, such as SWD debug headers and UART connections, are included to allow programming, debugging, and potential future expansion. 


Baseline Processor: STM32L432KC

The STM32L432KC was chosen as the main processor for this design.

Package Size:
The STM32L432KC comes in a UFQFPN 32 package measuring 5 mm × 5 mm and is well suited for compact PCB designs, helping reduce the overall board area due to the small design.

Cost:
The device is reasonably priced and becomes cost effective when purchased in higher quantities, like 1,000 units. Considering its performance and peripheral set, it provides good value for the cost.

Memory Size:
The microcontroller includes 256 KB of flash memory and 64 KB of SRAM, which is enough for the firmware required to read the sensor, process data, and handle communication or debugging features.

Peripherals:
The STM32L432KC includes a wide range of peripherals such as UART, SPI, I²C, timers, ADC channels, DMA support, and SWD debugging. These peripherals allow the system to easily interface with sensors and external devices while leaving room for future upgrades.

Power Utilization:
One of the main reasons this microcontroller was selected is its low power consumption. As part of ST’s L series, it is designed for ultra low power operation and includes multiple low power modes. This makes it a good choice for a coin cell powered system.

Vendor:
STMicroelectronics is a well known vendor with strong documentation, long term product support, and a large development community. We can use CubeIDE to make development and debugging easier.


Discussion of Alternatives
Although the STM32L432KC fits the design well, other microcontrollers could also be used for this project. Three alternatives are discussed:


Alternative 1: NXP LPC804 (ARM Cortex M0+)
The NXP LPC804 is a low cost microcontroller aimed at simple embedded applications.
Advantages:
o	Smaller package options
o	Simple architecture that is easier to understand
o	Lower cost than the STM32L432KC
Disadvantages:
o	Fewer peripherals
o	Lower processing performance
o	Much Less flash and SRAM
While the LPC804 could support basic light-sensing, it would be more limiting if additional features were added later.


Alternative 2: Texas Instruments MSP432P401R
The MSP432P401R from Texas Instruments is a powerful, low power microcontroller based on an ARM Cortex M4 core.
Advantages:
o	Very good low power performance
o	Large memory sizes
o	Strong processing capabilities
Disadvantages:
o	Larger package size
o	More complex power and clock configuration
o	Higher cost
This device offers more performance than is needed for this Lab, which increases cost and complexity without significant benefits.


Alternative 3: Microchip ATSAMD21G18 (ARM Cortex M0+)
The ATSAMD21G18 is a commonly used microcontroller in many low power designs and educational platforms.
Advantages:
o	Decent peripheral support
o	Reasonable cost
o	Nicely documented and widely used
Disadvantages:
o	Lower performance than the STM32L432KC
o	Smaller SRAM
o	Less optimized for ultra low power modes
This device could work for the project, but offers fewer advantages compared to the chosen STM32 device.


Recommendation
After comparing the STM32L432KC to the 3 alternative options, the STM32L432KC is the best overall choice for this design, which makes sense on why we chose it. It provides good balance between performance, power efficiency, memory size, and cost. The small package size supports a compact PCB layout, and the wide range of peripherals allows flexibility for future improvements. The STM32L432KC fits the requirements of this project well and offers room for growth, making it the most appropriate choice.


Conclusion
This light sensor system shows how careful component selection plays a big role in embedded system design. The STM32L432KC provides low power operation, strong peripheral support, and a compact form factor that aligns well with the design goals of the project. 
