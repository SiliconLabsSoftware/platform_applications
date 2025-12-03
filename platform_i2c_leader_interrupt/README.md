# Platform - I2C Leader Interrupt

![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v4.5.0-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-13.11%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-4.5%20KB-blue)


## Summary

This project shows how to configure the I2C periphreal in leader mode,
and use interrupt based routine to perform I2C transfers with a
secondary device. The I2C state machine is implemented in the I2C interrupt
handler. Both read and write operations are supported and can be configured
using the push buttons.

## SDK version

- [Gecko SDK v4.5.0](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

## Hardware Required

- Board:  2x Silicon Labs EFR32xG24 Starter Kit + WSTK (BRD4186C + BRD4001A)
  - Device: EFR32MG24B210F1536IM48

## Connections Required

Connect the GND, I2C_SCL, and I2C_SDA pins between the two boards and use a
logic analyzer to probe the SCL and SDA pin. Or run the project in debug mode
and check the receiveBuffer for read operations.

I2C_SCL -> PC05 EXP 15  
I2C_SDA -> PC07 EXP 16

Press Push Button 0 for read request from leader to follower
Press Push Button 1 for write request from leader to follower

## Setup

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **leader interrupt**.

3. Click the **Create** button on the **Platform - I2C Leader Interrupt** example. The example project creation dialog pops up -> click Create and Finish and the project should be generated.

4. Build and flash this example to the board.

## How It Works

The I2C leader state machine is configured in the I2C handler using different
I2C interrupt. The state machine itself is documented in the EFR32xg24 reference
manual's I2C chapter.
The I2C follower example is used to test the leader project, but ideally any I2C
follower device should work as long as the follower's address is written correctly.
