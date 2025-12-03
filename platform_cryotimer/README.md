# Platform - Cryotimer #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v4.5.0-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-8.3%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-4.1%20KB-blue)

## Summary ##

This project generates a 0.5 Hz square wave on PA11 while in EM3 and wakes on an interrupt at 977 mHz to toggle PA11 which is connected to the EXTCOMIN pin of the display on the GG11 STK. The application goes into EM3 after setup.

Peripherals used: CRYOTIMER, EMU

## SDK version ##

- [Gecko SDK v4.5.0](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

## Hardware Required ##

- One SLSTK3701A Giant Gecko GG11 Starter Kit
<https://www.silabs.com/products/development-tools/mcu/32-bit/efm32-giant-gecko-gg11-starter-kit>

## Connections Required ##

- Simply connect the kit to the PC

## Setup ##

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **Cryotimer**.

3. Click the **Create** button on the **Platform - Cryotimer** example. The example project creation dialog pops up -> click Create and Finish and the project should be generated.

4. Build and flash this example to the board.

## How It Works ##

The intent is to provide an example of how to use the cryotimer to provide the LCD display with a low-power EXTCOMIN signal it requires. If the display EXTMODE pin is HIGH, the polarity inversion is armed for every rising edge of the EXTCOMIN pin. The toggling frequency should be at least 1 Hz. If EXTMODE is LOW, this pin is ignored.

## How to Port to Another Part ##

Open the "Project Properties" and navigate to the "C/C++ Build -> Board/Part/SDK" item. Select the new board or part to target and "Apply" the changes. Note: there may be dependencies that need to be resolved when changing the target architecture.
