# Ultrasonic-Radar - ATmega8A

#### Description
This project is the comprehensive design project for the electronic design practice basic course, named "Ultrasonic-Radar", which utilizes multiple modules such as ultrasonic and servo motors to measure distances from multiple directions, achieving the function of "radar". The author is Bonjir. The entire project was completed in 26 days.

The main part of this project is a C++ program developed in Atmel Studio 7 development environment running on an ATmega8A chip with an external 16MHz crystal oscillator. The upper computer part is a graphical interface program developed in Visual Studio 2022 development environment with the application of OpenGL 3D graphics library, which can run on 64-bit Windows systems. This project uses ultrasonic HC-SR04 module, servo motor MG90S module, OLED screen 12864 module, keypad matrix, CH340G serial module, and encapsulated all modules to shield the hardware-level operations from the upper interface, avoiding complexity and greatly improving code readability, generality and flexibility.

This project has self-designed and printed shells for servo motors and ultrasonic modules. Compared with existing two-degree-of-freedom servo motor platforms, it can rotate at larger angles, and can be easily disassembled and carried, with higher flexibility and portability.

This project focuses on optimizing code performance, effect, and size, trying to put as many functions as possible in one program file, and maximizing the use of various storage units of the MCU, such as Flash, EEPROM, and SRAM. Due to the limited storage size of the chip used in this project, the method of trading time for space has been adopted in part of the code to reduce space usage. 

All codes in this project are open-sourced under the MIT license.

#### Contributors
Project author: Bonjir  
Supervising teacher: lyh
