# Platform - EM3 Sleep with Micrium Using the Cryotimer #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v4.5.0-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-20.67%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-16.62%20KB-blue)

## Summary ##

This project allows an MCU running Micrium to enter EM3 sleep using the cryotimer.

## SDK version ##

- [Gecko SDK v4.5.0](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

## Hardware Required ##

- SLSTK3701A EFM32 Giant Gecko GG11

## Connections Required ##

- Connect the Kit to the PC.

## Setup ##

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **Cryotimer**.

3. Click the **Create** button on the **Platform - Micrium Cryotimer EM3 Sleep** example. The example project creation dialog pops up -> click Create and Finish and the project should be generated.

4. Build and flash this example to the board.

## How It Works ##

This project works by replacing the standard tick_rtcc.c with a custom one that uses the cryotimer and ULFRCO as the RTOS's system timer. The device will enter EM3 sleep from the idle task hook. You will see the least current consumption if you sleep for time periods which are powers of two. You can see the effect of this by changing the value of the dly parameter in the call to OSTimeDly() on line 237 of app.c.

## How to Port to Another Part ##

1. Create a new example based on the Micrium blink example
2. Replace the contents of the generated app.c with the one from the repo.
3. You will need to make a copy of os_cfg.h from the SDK before modifying it.
   - It can be found by opening os.h in "kernel/include".
   - On line 97 you will see os_cfg.h referenced.
   - Right-click on os_cfg.h and click "Open Declaration".
   - Try making a change to the file and Simplicity will ask if you'd like to make a copy of the file. Allow it to make the copy, then you will find the file in "external_copied_files".
4. Replace the contents of the copied os_cfg.h with the one from the repo.
5. Add tick_rtcc.c from this repo to your workspace.
6. Add em_cryotimer.c to the "emlib" folder.
