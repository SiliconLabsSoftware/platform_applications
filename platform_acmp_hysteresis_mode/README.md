# Platform - ACMP Hysteresis Mode #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v2024.12.2-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-34.41%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-6.35%20KB-blue)


## Overview ##

This project configures the ACMP peripheral to compare the voltages on GPIO inputs with a specified hysteresis mode.

The ACMP peripheral is configured to use GPIO pins as positive and negative inputs. The internal reference voltages can also be used as either the positive or negative input. Any ACMP hysteresis mode can be specified in the CLI, and sleep mode can also be enabled. The ACMP output is routed through PRS to a GPIO pin controlling the LED. 

Peripherals used: ACMP, GPIO and PRS.

## SDK version ##

- SiSDK v2024.12.2

## Hardware Required ##

* Boards: Silicon Labs Wireless Pro Kit Mainboard (SLWMB4002A, formerly BRD4002A) with one of the following supported Radio Boards, or the EFR32xG27 Development Kit.
* Devices: EFR32xG21, EFR32xG24, EFR32xG25, EFR32xG27, EFR32xG28 

| Board ID | Description  |
| ---------------------- | ------ |
| BRD4180B | [EFR32xG21A Wireless Gecko 2.4 GHz +20 dBm Radio Board](https://www.silabs.com/development-tools/wireless/slwrb4180b-efr32xg21-wireless-gecko-radio-board?tab=overview)|	  
| BRD4181C | [EFR32xG21B Wireless Gecko 2.4 GHz +10 dBm Radio Board with Secure Vault](https://www.silabs.com/development-tools/wireless/slwrb4181c-efr32xg21-wireless-gecko-radio-board?tab=overview)|	  
| BRD4186C | [EFR32xG24 Wireless 2.4 GHz +10 dBm Radio Board](https://www.silabs.com/development-tools/wireless/xg24-rb4186c-efr32xg24-wireless-gecko-radio-board?tab=overview)|
| BRD4270B | [EFR32FG25 902-928 MHz +16 dBm Radio Board](https://www.silabs.com/development-tools/wireless/proprietary/fg25-rb4270b-efr32fg25-radio-board?tab=overview)|
| BRD4271A | [EFR32FG25 863-876 MHz +16 dBm Radio Board](https://www.silabs.com/development-tools/wireless/proprietary/fg25-rb4271a-efr32fg25-radio-board?tab=overview)|
| BRD4400c | [EFR32xG28 2.4 GHz BLE and +14 dBm Radio Board](https://www.silabs.com/development-tools/wireless/xg28-rb4400c-efr32xg28-2-4-ghz-ble-and-14-dbm-radio-board?tab=overview)|
| BRD2602A | [EFR32xG27 Development Kit](https://www.silabs.com/development-tools/wireless/efr32xg27-development-kit?tab=overview)|
	  
## Connections Required ##

Connect the board via a micro-USB cable to your PC to flash the example and virtual serial port for the CLI.
GPIO pins should be connected to external voltage sources using the expansion header on the mainboard.
A jumper wire is needed on some boards to route an EM2/3-capable GPIO pin to control the LED.

Default connections for each supported boards are listed ih the following table.

| Board ID | Positive input | Negative input | Additional wiring |
| ------ | ------ | ------ | ------ |
| BRD4180B, BRD4181C | PB0 (expansion header pin 7) | PB1 (expansion header pin 9) | Short F8 and F10 on the main board. |
| BRD4186C |  PA6 (expansion header pin 11) | PA7 (expansion header pin 13) | |
| BRD4270B, BRD4271A | PA6 (expansion header pin 11) | PA7 (expansion header pin 13) | Short F8 and F10 on the main board. |
| BRD4400C | PB4 (expansion header pin 11) | PB5 (expansion header pin 13) | |
| BRD2602A | PA8 (expansion header pin 3) | PA7 (expansion header pin 5) | |

## Setup ##
To test this application, you can either create a project based on an example project or start with an CLI Bare Metal Application example project.

### Create a project based on an example project ###

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **'acmp'**.

3. Click on the **Create** button on the **Platform - ACMP Hysteresis Mode** example. Example project creation dialog pops up -> click **Finish** and the project should be generated.

4. In the SOFTWARE COMPONENTS tab, navigate to [Services] → [IO Stream] → [Driver] → [IOStream: EUSART]. Set **Flow Control** to None, and uncheck **Restrict the energy mode to allow the reception.**

5. Optionally, change the ACMP_POS_INPUT_TYPE or ACMP_NEG_INPUT_TYPE in clc.h to use one of the internal voltage references.

6. Build and flash this example to the board.

### Start with a CLI Bare Metal Application example project ###

1. Create a **CLI Bare Metal Application** project for your hardware using Simplicity Studio 5.

2. Replace the cli.c file with the provided file located in the **src** folder.

3. Replace the cli.h file with the provided file located in the **inc** folder's subfolder that matches you board name.

4. Optionally, change the ACMP_POS_INPUT_TYPE or ACMP_NEG_INPUT_TYPE in clc.h to use one of the internal voltage references.

5. Open the .slcp file. Select the SOFTWARE COMPONENTS tab and install the following software components:

   - [Platform] → [Peripheral] → [ACMP]
   - [Platform] → [Services] → [Power Manager] → [Power Manager]

6. In the SOFTWARE COMPONENTS tab, navigate to [Services] → [IO Stream] → [Driver] → [IOStream: EUSART]. Set **Flow Control** to None, and uncheck **Restrict the energy mode to allow the reception.**

7. Build and flash this project to your device.

## How It Works ##

The ACMP0 peripheral is configured by the ACMP_Init() function, with the hysteresis mode specified in the structure passed as the second argument. The ACMP_ChannelSet() function is used to select the input sources. The ACMP output is routed to a GPIO pin using PRS_SourceAsyncSignalSet() and PRS_PinOutput().

Since Ports A and B are required for EM2/3 operation, the input sources are selected from pins on these ports that are available on the expansion header. If the GPIO pin powering the LED is from Ports A or B, it is used as the ACMP output. Otherwise, the output pin is selected from Ports A or B and should be jumper-wired to power the LED. In this case, the original GPIO pin for the LED is disabled, and the GPIO pin for the ACMP output is enabled.

The ACMP input sources can be changed by modifying the ACMP_POS_INPUT and ACMP_NEG_INPUT macros. The code is designed to have both sources in the same port; therefore, make sure to change the GPIOBUS macro to match the port used. Ports C and D can be used if EM2/3 is not required.

The ACMP output can be routed to another pin by modifying the LED0_PORT and LED0_PIN macros. Ports C and D can be used if EM2/3 is not required, but in that case, the PRS channel must be chosen from 6 to 11.

Due to the limited number of pins available on some boards, the GPIO pin for the VCOM CTS signal is used as the ACMP output. That is why hardware flow control is disabled in the IO Stream driver. The terminal application on the PC should also disable hardware flow control to free this signal.

## Testing ##

There are three commands in the CLI:

1. **help**: list available commands with details including available hysteresis modes.
2. **hys \<mode>**: initialize the ACMP with the specified hysteresis mode and enable it. The board needs to be reset to use a new hysteresis mode.
3. **sleep**: enter EM2/3 for low-power operation. There's no command to wake up the board, as the serial port is disabled during the sleep.

After the ACMP is enabled, the LED should turn on when the positive input is higher than the negative input plus the hysteresis (if the voltage difference is increasing), and turn off when the positive input is lower than the negative input minus the hysteresis (if the voltage difference is decreasing). This operation should not be affected if the sleep command is entered.