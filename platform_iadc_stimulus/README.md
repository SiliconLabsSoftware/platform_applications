# Platform - IADC Stimulus #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v4.5.0-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-9.14%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-4.58%20KB-blue)

## Summary ##

The sole purpose of this project is to generate stimulus for the EFR32xG21 Timestamped IADC in EM2 example, which expects conversion start pulses on the selected digital input pin and some analog voltage on the selected analog input pin.

All this program does is, in response to PB0 being pressed, generate a set number of 1-microsecond pulses spaced at 10 microseconds (100 Hz) while a fixed analog voltage is output by the VDAC. After the specified number of pulses are output, the program again waits for PB0 to be pressed and repeats the pulse output sequence.

While not very useful by itself, the project could serve as the basis for a testbed that generates other kinds of synchronized analog or digital stimulus for another device, such as another EFM32 or EFR32 family member that needs to collect some kind of data while, ideally, remaining in EM2 to minimize energy use.
  
Modules used: CMU, EMU, GPIO, SYSTICK, and VDAC0.

## SDK version ##

* [Gecko SDK v4.5.0](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

## Hardware Required ##

* EFM32PG12 Starter Kit (SLSTK3402A)
* Jumper wires to connect the outputs to another (W)STK

## Connections Required ##

* Connect the kit to the PC and the outputs to another (W)STK.

## Setup ##

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **IADC**.

3. Click the **Create** button on the **Platform - IADC Stimulus** example. The example project creation dialog pops up -> click Create and Finish and the project should be generated.

4. Build and flash this example to the board.

## How It Works ##

1. Initialize the DCDC.

2. Set up VDAC0 and output a voltage that is 50% of full scale (0.625V @ VREF = 1.25V) on the selected analog output pin (PC10, which is pin 16 on the EFM32PG12 STK expansion header).

3. Configure SYSTICK for 1 ms ticks.

   This code includes a set of SYSTICK utility functions in the Drivers/systick.* files that can be useful for other purposes.

4. Set up the two GPIOs used (PC9 for the pulse output and the BSP-specified pin connected to push button 0).

5. Wait in EM1 for PB0 to be pressed.

6. Disable the PB0 interrupt.

7. Output the set number of pulses at 100 Hz on PC9 by driving the pin high for 1 microsecond and then low for 9 microseconds.

8. Re-enable the PB0 interrupt and go back to step 5.

## Porting to Another EFR32 Series 1 Device ##

Apart from any issues of pin availability on the expansion header for a given board, this code should run as-is on any device with a VDAC. This would preclude devices that only have the IDAC (e.g. EFM32JG1, EFM32PG1, and EFR32xG1), although it would be a simple matter to use the IDAC for the same purpose by connecting an appropriately sized resistor to the output pin.

To change the target board, navigate to Project -> Properties -> C/C++ Build -> Board/Part/SDK. Start typing in the Boards search box and locate the desired radio board, then click Apply to change the project settings, and go from there.
