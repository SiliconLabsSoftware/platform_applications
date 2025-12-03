# Platform - VDAC Calibration

![Type badge](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/SiliconLabs/application_examples_ci/master/platform_applications/platform_peripheral_vdac_calibration_common.json&label=Type&query=type&color=green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v4.5.0-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-16.04%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-4.12%20KB-blue)

## Summary

This project demonstrates how to use the internal ADC to calibrate the DAC. The project is written for and tested on an EFM32TG11 starter kit. Both VDAC channels are enabled and channel 0's main output and alternate output 1 are enabled. Note that channel 0 main output has no breakout pin available for the EFM32TG11 starter kit. Therefore, testing measurement for channel 0 is only taken on channel 0 alternate output 1.

## SDK version

- [Gecko SDK v4.5.0](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

## Hardware Required

SLSTK3301A EFM32TG11 Starter Kit

## Connections Required

Connect the kit to the PC.

## Setup

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **VDAC**.

3. Click the **Create** button on the **Platform - VDAC Calibration** example. The example project creation dialog pops up -> click Create and Finish and the project should be generated.

4. Build and flash this example to the board.

## How It Works

The DAC calibration routine for this project is as follows:

1. Initialize VDAC with the desired configuration
2. Initialize ADC with high accuracy setting
3. Output 80 percent of VFS (0xCCC) on both VDAC channel 0 and channel 1
4. Calibrate GAINERRTRIM - set GAINERRTRIM to max value 0x3F.
5. Use ADC internal DACOUT input to read DAC channel 0 output value.
6. Calculate the error. error = abs(Vout / (Vref × 0.8)-1) or 
   abs((Vout / Vref × 4096) / (Vref × 0.8 / Vref × 4096) - 1)
7. Decrease GAINERRORTRIM by 1, recalculate the error. Continue to do so
   until the smallest error is found.
8. Repeat the same process for GAINERRORTRIMCH1.
9. Update the VDAC0 calibration register.
10. Change VDAC output to the desired output value.

## How to Port to Another Part

Open the "Project Properties" and navigate to the "C/C++ Build -> Board/Part/SDK" item. Select the new board or part to target and "Apply" the changes.
This example can only be ported to Series 1 devices.
Note: there may be dependencies that need to be resolved when changing the target architecture.

## Special Notes

This project uses the internal ADC to measure the VDAC output and calibrate accordingly. This method will introduce measurement error caused by the ADC, so it is important to verify whether the result will meet the requirements needed.
