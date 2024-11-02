# semaforo_fsm_psm2
A traffic light control system for STM32 microcontroller using a finite state machine (FSM). It controls two LEDs to simulate green and red lights, allowing pedestrians to request crossing by pressing a button. The FSM handles transitions between green, blinking green, red, and blinking red states, ensuring proper vehicle and pedestrian flow.

Made by: Edgar Ramirez & Marcelo Barrios

Attention !!!
To generate the .hex and upload the code through the STM32CUBEProgrammer the following command “arm-none-eabi-objcopy -O ihex ${ProjName}.elf ${ProjName}.hex” was used in the Post-Build Steps section
it can be found in:
Project>Properties>C/C++ Build>Settings>Post-Build Steps
