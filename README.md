# Voltage controll with External Encoder
This is a project for getting familiar with `DAC` on  `STM32L152RE` by controlling the voltage with an Incremental Encoder `PEC11R` sensor and display on Realterm through `USART`




Table of Content  
	
	1. Equipment and Component  
	
	2. Description 

	3. Circuit   

	4. Testing
	
	
1. Equipment and Component

* 1 x NUCLEO-L152RE (development board)
* 1 x USB-cable (A - miniB connectors)
* 1 x Laboratory PC, with Atollic TrueSTUDIO and RealTerm installed
* 1 x 2-bit quadrature code incremental encoder
* 7 x 10 kΩ resistor
* 2 x 10 nF ceramic capacitor
* 1 x LM741 operational amplifier
* 1 x BC547 transistor

2. Description

The purpose of this project is to learn how to combine the external encoder with the internal DAC peripheral on `STM32L152RE` and learn how to create an industry standard 0-10V message.

The quadrature encoder will be used in this Project, because an encoder with one set of pulses would not be useful because it could not indicate the direction of rotation. Using two
code tracks with sectors positioned 90 deg out of phase (Figur below), the two output channels of the quadrature encoder
indicate both position and direction of rotation. If A leads B, for example, the disk is rotating in a clockwise direction. If B
leads A, then the disk is rotating in a counter-clockwise direction. Therefore, by monitoring both the number of pulses
and the relative phase of signals A and B, you can track both the position and direction of rotation.

![quadrature](https://github.com/ThinhLe279/Voltage_Control_External_Encoder/blob/main/picture/quadrature.png)

We will select Rising Edge trigger for out Interupt to detect when the Encoder rotated in which direction 

![rising_edge](https://github.com/ThinhLe279/Voltage_Control_External_Encoder/blob/main/picture/detect.png)

3. Circuit

* Make a circuit of encoder accoording to 


 ![encoder](https://github.com/ThinhLe279/Voltage_Control_External_Encoder/blob/main/picture/Encoder_schematic.png)


* Make a circuit to convert 0-3V to 0-10V

![amplifier](https://github.com/ThinhLe279/Voltage_Control_External_Encoder/blob/main/picture/Amplifier.png)

4. Testing

open RealTerm to monitor the result

![pulse](https://github.com/ThinhLe279/Voltage_Control_External_Encoder/blob/main/picture/pulse.png)



