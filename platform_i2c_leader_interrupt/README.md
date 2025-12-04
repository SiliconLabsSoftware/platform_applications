# Platform - I2C Leader Interrupt #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v2025.6.2-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-25.13%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-5.2%20KB-blue)

## Overview ##

This project demonstrates configuring the I2C peripheral in leader mode and using an interrupt-driven state machine to perform read and write transfers with a follower device. Both transaction types are triggered via push buttons. The state machine logic executes inside the I2C interrupt handler.

Peripherals used: I2C, GPIO, NVIC.

## SDK version ##

- [Simplicity SDK v2025.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2025.6.2)

## Hardware Required ##

* 2 x EFR32xG24 Starter Kit (BRD4186C Radio Board + BRD4001A WSTK Mainboard)
* Logic analyzer (optional) for observing SCL/SDA

| Board ID | Description |
|----------|------------|
| BRD4186C | EFR32xG24 Wireless Gecko Radio Board |
| BRD4001A | Wireless Starter Kit Mainboard |

## Connections Required ##

Tie the following pins between the leader and follower boards:

| Signal | Leader Pin | Follower Pin |
|--------|------------|--------------|
| GND    | GND        | GND          |
| I2C_SCL | PC05 (EXP 15) | PC05 (EXP 15) |
| I2C_SDA | PC07 (EXP 16) | PC07 (EXP 16) |

Push Button 0: Initiate read request (leader reads from follower)

Push Button 1: Initiate write request (leader writes to follower)

## Setup ##

To test this application, you can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. Ensure this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).
2. From the Launcher Home, add your board to My Products, open it, and select **EXAMPLE PROJECTS & DEMOS**. Filter by **'i2c'**.
3. Create the **Platform - I2C Leader Interrupt** example and finish the dialog.
4. Build and flash to the leader board.
5. Import the follower project (e.g. [I2C_Follower peripheral example](https://github.com/SiliconLabs/peripheral_examples/tree/master/series2/i2c/i2c_follower)) for the second board and flash it.

### Start with an empty example project ###

1. Create an **Empty C Project** for your hardware in Simplicity Studio 5.
2. Copy source files from this example's `src` directory into your project.
3. Install software components:
	- [Platform] → [Peripheral] → [I2C]
	- [Platform] → [Services] → [Sleep]
4. Build and flash to the leader board; flash follower code to the second board.

## How It Works ##

The leader initiates an I2C transfer upon a button press. An interrupt-driven state machine manages start condition, address phase, data byte transfers, ACK/NACK handling, and stop condition. The receive or transmit buffer pointer advances inside the ISR until the transaction completes. A read transaction collects bytes into `receiveBuffer`; a write transaction sends a predefined buffer.

High-level state flow:
1. IDLE → BUTTON triggers START
2. ADDRESS sent → wait for ACK
3. WRITE: send data bytes sequentially / READ: receive bytes into buffer
4. Final byte processed → send STOP
5. Return to IDLE

If a NACK or bus error occurs, the state machine aborts, issues a STOP, and returns to IDLE for retry.

## Testing ##

1. Flash leader and follower boards.
2. Connect SCL/SDA/GND per pin table.
3. Open debugger or attach logic analyzer to PC05/PC07.
4. Press PB0 (read): verify `receiveBuffer` populates with follower data.
5. Press PB1 (write): verify follower receives expected byte pattern.
6. Observe bus waveforms (optional) confirming proper ACK/NACK sequence.

## Additional Notes ##

* Adjust follower address constant in leader code to match your peripheral.
* Pull-up resistors may be present on the kit; if not, add external 4.7 kΩ resistors from SCL/SDA to VDD.
* For larger payloads, consider DMA-driven I2C if available.
* Error handling can be extended to retry automatically on arbitration loss.