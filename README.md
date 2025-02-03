# LTC4150 Module
English | [简体中文](README_zh.md)

---
## Environment and Relevant Pins：
This program is based on *STM32F103*, with the **INT** pin connected to PB7, the **POL** pin to PB6, and the **SHDN** pin to PB5.

## Functionality:
By inputting the total battery capacity in mAh and the current battery percentage, the program calculates the remaining battery capacity and, optionally, the current. It also provides an I2C-connected OLED screen to display the data.

## Notes:
The program uses interrupts, so you need to **close SJ1** on the LTC4150. 

If you want the output voltage to be set to 3.3V, **close SJ2 and SJ3**. For 5V output, leave them **open**.

## Description:
This program is a port of the official Arduino code to STM32F103.

## Reference:
https://github.com/sparkfun/LTC4150_Coulomb_Counter_BOB

https://learn.sparkfun.com/tutorials/ltc4150-coulomb-counter-hookup-guide#solderjumpers
