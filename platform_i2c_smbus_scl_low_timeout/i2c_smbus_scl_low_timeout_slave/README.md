# Platform - I2C SMBus SCL Low Timeout Slave #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![SDK badge](https://img.shields.io/badge/SDK-v4.5.0-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-7.4%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-4.07%20KB-blue)

## Summary ##

The Silicon Labs EFR32 devices contain I2C modules for serial communication that enable these devices to be interfaced with SMBus devices. The SMBus protocol is similar to the I2C protocol but has additional specifications and requirements. One of these requirements is that any SMBus master can signal an error condition by holding SCL low for 25 ms, which constitutes an SCL low timeout period. Accordingly, any device participating in SMBus communication must detect this timeout condition and reset its SMBus communication module to recover from the error condition. This project demonstrates the slave configuration of the EFR32xG21 I2C peripheral (based on the [I2C Slave Peripheral Example](https://github.com/SiliconLabs/peripheral_examples/tree/master/series2/i2c/i2c_follower)) with the addition of an implementation of the SCL low timeout required by the SMBus standard.

Modules used: CMU, EMU, GPIO, I2C, TIMER0.

## SDK version ##

- [Gecko SDK v4.5.0](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

## Hardware Required ##

- Wireless Starter Kit (WSTK) Mainboard (SLWMB4001A, formerly BRD4001A)
- EFR32xG21 2.4 GHz 10 dBm Radio Board (BRD4181A)

## Connections Required ##

- Connect the Kit to your PC.

## Setup ##

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **SMBus**.

3. Click the **Create** button on the **Platform - I2C SMBus SCL Low Timeout Slave** example. The example project creation dialog pops up -> click Create and Finish and the project should be generated.

4. Build and flash this example to the board.

## How It Works ##

Two EFR32xG21 modules are connected together, one running the [I2C Master Peripheral Example Project](https://github.com/SiliconLabs/peripheral_examples/tree/master/series2/i2c/i2c_leader_dma), the other running this slave project. The master reads the slave's current buffer values, increments each value, and writes the new values back to the slave device. The master then reads back the slave values again and verifies the new values match what was previously written. This program runs in a continuous loop, entering and exiting EM1 to handle I2C transmissions. The slave toggles LED0 on during I2C transactions and off when complete. The slave will set LED1 if an I2C transmission error is encountered.

The following are the correct connections and pin assignments for the WSTK with BRD4181A:

Board:  Silicon Labs EFR32xG21 Radio Board (BRD4181A) +
        Wireless Starter Kit Mainboard (SLWMB4001A, formerly BRD4001A)  
Device: EFR32MG21A010F1024IM32  
PB00 - LED0, Expansion Header Pin 11, WSTK Pin 8  
PB01 - LED1, Expansion Header Pin 13, WSTK Pin 10  
PA05 - I2C_SDA, Expansion Header Pin 12, WSTK Pin 9  
PA06 - I2C_SCL, Expansion Header Pin 14, WSTK Pin 11  

The code and configuration parameters are contained in the source file main_xg21_i2c_smbus_scl_low_timeout_slave.c. The first section of this file (from lines 49-89) contain #includes, #defines, and global variables.

The #define SCL_TIMEOUT_MS is used to configure the timeout period for SCL low timeout detection. This timeout period is used to calculate the TOP value for a one-shot timer that is started on the falling edge of the SCL pin (PA06). If the TOP value is reached, a TIMER overflow interrupt is generated, indicating that an SCL low timeout has occurred. The TIMER ISR is used to initiate a reset of the I2C peripheral.

A rising edge on the SCL pin (as would occur in normal communications) will stop and reset the timer.

The code flow is as follows:

1. Initialize the chip with errata-specific initializations, if appropriate.

2. Initialize the Systick timer, which is used to help generate delays used as part of an LED status indicator when the I2C is reset.

3. Initialize the I2C and GPIO.

4. Initialize TIMER0 as a one-shot timer for detecting SCL low timeout.

5. The main loop will check if an I2C/SMBus transfer is in progress, and if not, will turn off LED0, which is set at the beginning of a transfer. Additionally, it will check to see if an I2C reset is required, and if so, initiate the I2C reset. Otherwise, the main loop will put the device into EM1. Note that if an I2C reset occurs, both LED0 and LED1 will flash on and off twice.

## Testing ##

1. Connect the SDA, SCL and GND lines between two kits via the EXP header
2. Jumper 4.7kΩ pull-up resistors from VMCU to the SDA and SCL lines (only one
pull-up needed for each line).
3. Open Simplicity Studio and update each kit's firmware from the Simplicity
Launcher (if necessary)
4. Build both the master and slave projects and download to two Starter Kits
5. For the slave kit, in the drop-down menu, select "Profile As Simplicity Energy
Profiler Target"
6. The project will compile, load and start-up, proceeding to the main loop
which sits in EM1
7. Observe the current consumption in Energy Profiler of the kit in EM1
8. Toggle push button PB0 on the master kit
9. Observe the current spike as the slave kit wakes to EM0/1 to handle the I2C
transaction and then returns to EM1 consumption with each button press of PB0 on
the master kit.
10. Connect the SCL pin to GND to simulate an SCL low event and observe that the
slave device initiates an I2C reset, indicated by the flashing of LED0 and LED1
twice.
11. Following I2C reset, communications can resume

## Porting to Another EFR32 Device ##

This code is designed to run on the BRD4181A.

The code can also run on EFR32xG22 and other EFR32 devices, although this has not been tested. In the case of these other devices, however, all peripheral or module clocks would need to be specifically enabled because other EFR32 families do not have the on-demand module clock enable functionality that is present on xG21.

To change the target board, navigate to Project -> Properties -> C/C++ Build -> Board/Part/SDK. Start typing in the Boards search box and locate the desired radio board, then click Apply to change the project settings, and go from there.
