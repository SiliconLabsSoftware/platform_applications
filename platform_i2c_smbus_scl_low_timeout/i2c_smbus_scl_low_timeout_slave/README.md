# Platform - I2C SMBus SCL Low Timeout Slave #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v2025.6.2-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-9.63%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-4.43%20KB-blue)

## Overview ##

This project implements an SMBus-compliant I2C slave with SCL low timeout detection on an EFR32xG21 device. The SMBus specification requires that devices detect when SCL is held low for a defined timeout (typically 25 ms) and reset their communication interface to recover. This example extends the basic slave behavior (based on the peripheral example) with a TIMER-based timeout mechanism that triggers a software reset of the I2C peripheral when SCL remains low beyond the configured window.

Peripherals used: I2C, TIMER0, GPIO, EMU, CMU.

## SDK version ##

- [Simplicity SDK v2025.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2025.6.2)

## Hardware Required ##

* Wireless Starter Kit Mainboard (SLWMB4001A / BRD4001A)
* EFR32xG21 Radio Board (BRD4181A)
* Second board (same type) running I2C master example
* Optional: Logic analyzer / Energy Profiler

| Board ID | Description |
|----------|-------------|
| BRD4181A | EFR32xG21 2.4 GHz 10 dBm Radio Board |
| BRD4001A | Wireless Starter Kit Mainboard |

## Connections Required ##

| Signal | Pin (Slave) | EXP Header |
|--------|-------------|------------|
| LED0   | PB00        | Pin 11 |
| LED1   | PB01        | Pin 13 |
| I2C_SDA | PA05       | Pin 12 |
| I2C_SCL | PA06       | Pin 14 |

Connect SDA, SCL and GND between master and slave boards. Add 4.7 kΩ pull‑ups from SDA and SCL to VMCU if not already present.

## Setup ##

To test this application, you can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###
1. Add this repository to External Repos in Simplicity Studio.
2. Import the slave example (`platform_xg21_i2c_smbus_scl_low_timeout_slave.sls`).
3. Import the standard I2C master example on the second board.
4. Build and flash both projects to their respective boards.

### Start with an empty example project ###
1. Create an **Empty C Project** for BRD4181A.
2. Add source from `main_xg21_i2c_smbus_scl_low_timeout_slave.c` and required headers.
3. Install software components:
	- [Platform] → [Peripheral] → [I2C]
	- [Platform] → [Peripheral] → [TIMER]
4. Build and flash to the board.

## How It Works ##

The master continuously performs: read current slave buffer → increment data → write back → re-read to verify. The slave manages transactions, toggling LED0 while active and LED1 on error.

### SCL Low Timeout Mechanism ###
* `SCL_TIMEOUT_MS` defines timeout length.
* Falling edge on SCL starts one‑shot TIMER0 (TOP computed from timeout and clock).
* Rising edge on SCL stops & resets TIMER0 (normal activity).
* TIMER overflow ISR indicates timeout → I2C peripheral reset sequence executes → status LEDs blink.

### Main Loop Behavior ###
1. Monitor transfer in progress flag.
2. Clear LED0 when idle.
3. Check timeout/reset request flag; perform I2C reinit if set (blink LED0/LED1 twice).
4. Enter EM1 between events for reduced power.

## Testing ##

1. Connect boards and pull‑ups.
2. Flash master and slave projects.
3. Press PB0 on master to trigger transactions; observe LED0 activity on slave.
4. Use logic analyzer to confirm ACK / data sequence.
5. Force SCL low (jumper SCL to GND) > timeout period → observe LED0 & LED1 blink twice and I2C recovery.
6. Resume normal operation: remove jumper and continue PB0 presses.
7. Profile energy: slave should remain mostly in EM1.

## Additional Notes ##

* For different SMBus timeout values adjust `SCL_TIMEOUT_MS` and recompute TIMER TOP (handled in code macros).
* Ensure only one pair of pull‑ups present on bus to avoid excessive current.
* Extend error handling to log occurrences or increment diagnostic counters.
* Porting to other Series 2 devices requires enabling peripheral clocks if on‑demand gating differs.
