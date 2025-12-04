# Platform - EMU Temperature Sensor #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v2025.6.2-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-23.05%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-5.59%20KB-blue)

## Summary ##

This project demonstrates the use of low-energy periodic temperature sensor measurement in the EMU block of EFR32xG22 devices. The example project uses TIMER0 to periodically sample the EMU temperature sensor at 1 sample per second using the TEMPDRV driver, and then transmit the temperature information over VCOM/USART1.

Peripherals used: TIMER0, GPIO, EMU, USART1

## SDK version ##

- [Simplicity SDK v2025.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2025.6.2)

## Hardware Required ##

- One SLWSTK6021A EFR32xG22 Wireless Gecko Starter Kit <https://www.silabs.com/development-tools/wireless/efr32xg22-wireless-starter-kit>:
  - BRD4002A Wireless Starter Kit
  - BRD4182A EFR32xG22 Radio Board

## Connections Required ##

- Connect the Kit to the PC through a USB cable.

## Setup ##

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **temperature**.

3. Click the **Create** button on the **Platform - EMU Temperature Sensor** example. The example project creation dialog pops up -> click Create and Finish and the project should be generated.

4. Build and flash this example to the board.

## How It Works ##

Open a terminal program such as [Tera Term](https://ttssh2.osdn.jp/index.html.en), and connect to the Jlink CDC UART Port for the WSTK using 115200 baud, 8-bit data, 1 stop bit, no parity and no flow control. The terminal application will display the example version number out of reset, and then periodically (1 Hz) update with the latest EMU temperature reading in degrees Celsius as shown below.

```bash
EMU_Temp example v0.01
  EMU_Temp      34.10
  EMU_Temp      34.35
  EMU_Temp      34.10
```
