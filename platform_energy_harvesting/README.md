# Platform - Energy Harvesting Examples #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v2025.6.2-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-51.81%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-4.28%20KB-blue)


## Overview ##

This directory contains example applications and reference designs showcasing Ambient IoT energy harvesting techniques on Silicon Labs EFR32 platforms. These projects demonstrate how ultra‑low power wireless devices can operate from harvested energy sources (e.g. photovoltaic cells, indoor lighting) while broadcasting useful telemetry using proprietary beaconing built on RAIL.

Peripherals used: RAIL, GPCRC, ADC, EMU, GPIO.

## SDK version ##

- [Simplicity SDK v2025.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2025.6.2)

## Hardware Required ##

* Board: Silicon Labs EFR32 development board (e.g. EFR32MG22)
* Device: EFR32MG22
* Energy harvesting front-end (PV cell + storage element)
* Optional: Debug adapter (Simplicity Commander / J-Link)

## Connections Required ##

Connect the board via a micro-USB cable to your PC to flash the example. Connect the energy harvesting front-end according to the reference design schematic.

## Contents ##

| Folder | Description |
|--------|-------------|
| `PV_Asset_Tag/` | Photovoltaic (PV) powered asset tag reference design source (RAIL based beacon). |
| `RF_Asset_Tag/` | RF harvesting powered tag or placeholder for alternative harvesting modality. |
| `Python_Reader_GUI/` | Python utility / GUI for receiving and interpreting beacon packets. |

## Setup ##

To test this application, you can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. Make sure that this repository is added to [Preferences > Simplicity Studio > External Repos](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-about-the-launcher/welcome-and-device-tabs).

2. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **'energy harvesting'**.

3. Click on the **Create** button on the **Platform - Energy Harvesting Examples** example. Example project creation dialog pops up -> click **Finish** and the project should be generated.

4. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an **Empty C Project** project for your hardware using Simplicity Studio 5.

2. Copy all files from the `PV_Asset_Tag/Firmware_source/pv_asset_tag_reference_design/` folder into the project root folder.

3. Install the software components:

   - [Platform] → [Radio] → [RAIL Library]
   - [Platform] → [Peripheral] → [GPCRC]
   - [Platform] → [Peripheral] → [ADC]
   - [Platform] → [Peripheral] → [EMU]

4. Build and flash this project to your device.

## How It Works ##

The energy harvesting reference design implements an ultra-low power asset tag that operates from harvested energy sources.

### Key Implementation Features ###

Key implementation aspects inside `PV_Asset_Tag/Firmware_source/pv_asset_tag_reference_design`:

- Adaptive beacon scheduling based on storage capacitor voltage
- Dynamic transmit power decision (`apply_power_settings` and related logic)
- EM4 wake-up configuration (see `main.c` EM4WU pin setup)
- CRC32 generation using GPCRC peripheral to ensure payload integrity
- Voltage measurement (`measure_voltages`) and temperature measurement integration
- Minimal BLE-like advertising payload construction using proprietary RAIL beacons

### Beacon Operation ###

1. The device wakes up from EM4 sleep mode based on a configured interval.
2. Voltage and temperature measurements are taken.
3. Based on the storage capacitor voltage, the beacon interval and transmit power are adjusted.
4. A beacon packet is transmitted using RAIL with the collected sensor data.
5. The device returns to EM4 sleep mode to conserve energy.

### Payload Format ###

Harvester payload (when `ADV_PAYLOAD_HARVESTER` is defined) includes:

- Company ID (Silicon Labs)
- Optional packet type
- Device ID (derived + modified by address bytes)
- Temperature
- Storage capacitor voltage & delta
- Light intensity estimate
- Power level / state and DIP switch state
- Optional next interval (little endian)
- Optional counter
- CRC32 (if enabled)

Short payload variant includes a subset for reduced energy cost.

## Testing ##

1. Build and flash the project to your device with energy harvesting hardware attached.
2. Use a compatible receiver (another EFR32 board or Python GUI tool from `Python_Reader_GUI/`) to observe beacon packets.
3. Monitor the beacon interval and transmit power adjustments based on available harvested energy.

## Customization ##

- Adjust defines at top of `main.c` to enable/disable payload fields (`ADV_*` feature flags)
- Modify power decision logic in `power.c` to tune thresholds
- Extend measurement code in `adc.c` / `temp.c` for additional sensors

## Troubleshooting ##

| Issue | Possible Cause | Remedy |
|-------|----------------|-------|
| No beacon observed | HFXO not selected or radio init failed | Ensure `prepare_hfxo()` and `select_hfxo()` executed before `radio_init()`. |
| Payload CRC mismatch | Feature flag mismatch between sender and parser | Align `ADV_*` defines and recompute parser expectations. |
| Device sleeps too long | Mode selection pins misconfigured | Verify MODE_SWITCH pin states and EM4 wake sources. |
| High energy consumption | Excess TX power or interval too short | Adjust `decide_power_settings_and_update_buram` thresholds. |

## Additional Notes ##

- Licensed under Zlib (see `LICENSE.md`). Examples are provided as-is and considered Experimental Quality unless otherwise stated.
- For manual builds outside Studio, ensure linker script `linkerfile.ld` is used and GCC flags are optimized for low power (`-Os`).
- For more context and updates refer to the [Silicon Labs Energy Harvesting portal](https://www.silabs.com/wireless/energy-harvesting/ambient-iot-energy-harvesting-reference-designs).

