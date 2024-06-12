# BLDC Motor Driver Design and Circuit Diagram

###### For our undergraduate final project, we chose to design a BLDC motor driver. First, what is a BLDC? BLDC stands for Brushless Direct Current. Unlike standard motors, these motors do not have brushes. The winding ends are directly connected to the external world. In brushed motors, the driver's task is performed by the brush. BLDC motors have three terminals that connect to the external world. When we energize these terminals in the correct order, the rotor starts to move in the desired direction.

[![](https://burak20007878.github.io/page/9/Resim1.png)](http://https://burak20007878.github.io/page/9/Resim1.png)

## What Differentiates BLDC Motors from Other Motors and Why Are They Used?
###### In brushed motors, energy is supplied to the winding through a part called the carbon brush, which rubs against the commutator to transfer energy. Over time, due to friction, the carbon brush wears out and becomes unusable. Additionally, the dust from the worn carbon brush can fill the commutator, reducing its conductivity. BLDC motors do not have these issues, allowing them to operate longer with less maintenance and reduced loss due to the absence of friction from brushes. However, the complexity of the driving method is a downside. Unlike brushed motors that can be operated directly by supplying energy, BLDC motors require a driver to control the energizing sequence of the windings.

![](https://burak20007878.github.io/page/9/bldc_sargi.gif)

## Energizing the Motor Terminals
###### Three terminals labeled "U", "V", and "W" emerge from the motor. Energizing these terminals in sequence initiates motor movement. This energizing is done using electronic switches called MOSFETs. By applying voltage to the MOSFET's gate terminal, we can control higher voltages to turn the switches on and off.

![](https://burak20007878.github.io/page/9/Resim2.png)

## PWM and MOSFET Configuration
###### MOSFETs are switched on and off using PWM (Pulse Width Modulation). In our design, a switching frequency of 21 kHz was used, which is typical for most drivers. MOSFETs named SW1, SW3, and SW5 are in the high side group, meaning they are connected between the power rail and the load rather than between the load and ground. MOSFETs named SW2, SW4, and SW6 are in the low side group. In our driving method, high side MOSFETs are energized with PWM, while low side MOSFETs are energized with logic levels (1-0).

![](https://burak20007878.github.io/page/9/Resim4.png)

## High Side MOSFET Energization
###### Energizing the low side MOSFETs is straightforward, but the high side MOSFETs cannot be directly operated because their gate voltage needs to be higher than the source voltage. The source terminal of high side MOSFETs is not connected to ground, and the P_IN voltage appears at the source terminal. Using a method called bootstrap, the gate voltage is elevated above the P_IN voltage with the help of a bootstrap capacitor.

![](https://burak20007878.github.io/page/9/Resim6.png)

## Power Supply for the Driver
###### We used linear voltage regulators to provide the necessary voltages for the components on the driver. The MOSFET driver and op-amp are powered by a 12V regulator. Instead of using simple 7812 regulators, we opted for LM2575, which can handle higher input voltages, allowing us to step down voltages up to 65V to 12V. The control unit, STM32, is powered by a simple regulator named LT1117, as it operates on the 12V output from the previous regulator.

## Current and Voltage Sensing
###### A voltage divider (R14 and R15) allows the processor to read the main supply voltage and inform the user. The resistor R3 measures the current drawn by the motor. An op-amp processes the voltage and current values before they reach the processor, amplifying the low current values to readable levels.

![](https://burak20007878.github.io/page/9/Resim7.png)

## Control Unit and Components
###### The control unit features LEDs and buzzers for audio-visual alerts and buttons for speed control, typically done with a potentiometer. The STM32 microcontroller is in the center, and Hall sensor terminals are on the top right.

![](https://burak20007878.github.io/page/9/hall.png)

## Hall Sensors
###### Hall sensors detect the magnetic field and provide a binary output. By interpreting these signals, we determine if the rotor has reached the correct position before initiating the next energizing sequence.

![](https://burak20007878.github.io/page/9/Resim8.jpg)

## STM32 Control Unit
###### The driver control is managed by the STM32 board, known as "BluePill." It uses PWM for MOSFET control, ADC for voltage and current measurements, UART for PC communication, and GPIO for data input and output.

![](https://burak20007878.github.io/page/9/Resim11.png)

## Software Interface
###### We designed an interface using C# in Visual Studio. This interface communicates via serial port to adjust motor speed, direction, and stop the motor under specified current and voltage conditions.

![](https://burak20007878.github.io/page/9/akim.ppm)

## Current Challenges
###### Current Sensing: The current measurement fluctuates due to non-RMS values, making it difficult to set overcurrent limits.
###### RPM Measurement: Discrepancies in RPM readings at low speeds due to the motor’s pole count and electrical to mechanical conversion factors.

![](https://burak20007878.github.io/page/9/Resim9.png)
![](https://burak20007878.github.io/page/9/Resim10.png)