<table border="0">
  <tr>
    <td align="left" valign="middle">
    <h1>EFM32 and EFR32 Platform Application Examples</h1>
  </td>
  <td align="left" valign="middle">
    <a href="https://www.silabs.com/mcu/32-bit">
      <img src="https://pages.silabs.com/rs/634-SLU-379/images/WGX-transparent.png"  title="Silicon Labs Gecko and Wireless Gecko MCUs" alt="EFM32 32-bit Microcontrollers" width="250"/>
    </a>
  </td>
  </tr>
</table>

# Silicon Labs Platform Applications

[![Version Badge](https://img.shields.io/badge/-v3.0.0-green)](https://github.com/SiliconLabsSoftware/platform_applications/releases)
[![SDK Badge](https://img.shields.io/badge/SiSDK-v2025.6.2-green)](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2025.6.2)
![License badge](https://img.shields.io/badge/License-Zlib-green)

This repo contains example projects that demonstrate various applications using the peripherals of Silicon Labs EFM32 and EFR32 parts.

This repository provides both SLCP projects (as External Repositories) and SLS projects as standalone projects, which are configured for development boards.

## SoC and SDK Compatibility

This repository maintains multiple versions to support different SDK releases and chip families. Choose the version that matches your development environment and target device.

| Version | SDK | Chip Family |
|---|:---:|:---:|
| [v2.x.x](https://github.com/SiliconLabsSoftware/platform_applications/tree/release/v2) | [![SDK Badge](https://img.shields.io/badge/GSDK-v4.5.0-green)](https://github.com/SiliconLabs/gecko_sdk/releases) | Series 1 <br> Series 2|
| [v3.x.x](https://github.com/SiliconLabsSoftware/platform_applications/tree/master) | [![SDK Badge](https://img.shields.io/badge/SiSDK-v2025.6.2-green)](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2025.6.2) | Series 2 |

A full list of all examples and drivers in this repository is defined below.

## Examples

| No | Example name |
|:--:|:-------------|
| 1 | [Platform - ACMP Hysteresis Mode](./platform_acmp_hysteresis_mode) |
| 2 | [Platform - Asynchronous EM01GRPA Clock Output](./platform_async_em01grpaclk_out) |
| 3 | [Platform - Brushless DC Motor Control](./platform_bldc_motor_controller) |
| 4 | [Platform - DALI Communication](./platform_dali) |
| 5 | [Platform - Timing EM4 Duration with the BURTC](./platform_em4_duration) |
| 6 | [Platform - EMU Temperature Sensor](./platform_emu_temp) |
| 7 | [Platform - EMU Temperature Linearization](./platform_emu_temp_linearization) |
| 8 | [Platform - Energy Harvesting Examples](./platform_energy_harvesting) |
| 9 | [Platform - Fixed Gate Time - Frequency Measurement](./platform_fixed_gate_time_frequency_measurement) |
| 10 | [Platform - Flash blank checking using the GPCRC](./platform_gpcrc_blank_check) |
| 11 | [Platform - I2C Fast Plus Mode](./platform_i2c_fast_plus_mode) |
| 12 | [Platform - I2C Leader Interrupt](./platform_i2c_leader_interrupt) |
| 13 | [Platform - I2C Smbus Scl Low Timeout](./platform_i2c_smbus_scl_low_timeout) |
| 14 | [Platform - I2C Test for EFM8 IOExpander](./platform_i2c_test_for_efm8_ioexpander) |
| 15 | [Platform - I2C Slave Bootloader](./platform_i2cslave_bootloader) |
| 16 | [Platform - I2S Microphone VDAC](./platform_i2s_mic_vdac) |
| 17 | [Platform - IADC High Accuracy Examples](./platform_iadc_high_accuracy) |
| 18 | [Platform - IADC High-Speed Mode](./platform_iadc_high_speed) |
| 19 | [Platform - IADC with LDMA Ping-Pong](./platform_iadc_ldma_ping_pong) |
| 20 | [Platform - IADC Loopback with VDAC](./platform_iadc_loopback) |
| 21 | [Platform - IADC scan multiple external inputs](./platform_iadc_scan_multiple_external_input) |
| 22 | [Platform - IADC synchronous PRS with LDMA](./platform_iadc_synch_prs) |
| 23 | [Platform - IADC Tailgating](./platform_iadc_tailgating) |
| 24 | [Platform - KeyScan](./platform_keyscan) |
| 25 | [Platform - LCD Animation Blink](./platform_lcd_animation_blink) |
| 26 | [Platform - LDMA trigger with TIMER](./platform_ldma_trigger_with_timer) |
| 27 | [Platform - Lean Watchdog](./platform_lean_watchdog) |
| 28 | [Platform - Multi Channel PWM](./platform_multi_channel_pwm) |
| 29 | [Platform - NVM3 Integrity Test](./platform_nvm3_integrity_test) |
| 30 | [Platform - Interrupt-Driven HFRCO Calibration](./platform_hfrco_cal_interrupt) |
| 31 | [Platform - Polled HFRCO Calibration](./platform_hfrco_cal_polled) |
| 32 | [Platform - Executing Code from RAM](./platform_code_execution_ram) |
| 33 | [Platform - Wake M33 using PRS](./platform_prs_wfe) |
| 34 | [Platform - QI PRx communication protocol](./platform_qi_rx_base) |
| 35 | [Platform - Si7021 RHT Sensor Bare-metal](./platform_rht_baremetal) |
| 36 | [Platform - Segment LCD with LC Sensor](./platform_segmentLCD_lcsense) |
| 37 | [Platform - Segment LCD with LDMA](./platform_segmentLCD_ldma) |
| 38 | [Platform - SegmentLCD Low Power](./platform_segmentLCD_lowpower) |
| 39 | [Platform - Segment LCD and Temperature Sensor](./platform_segmentLCD_tempsensor) |
| 40 | [Platform - Segment LCD with Timer](./platform_segmentLCD_timer) |
| 41 | [Platform - Sine null points detector](./platform_sine_null_points_detector) |
| 42 | [Platform - SPI Throughput](./platform_spi_flash_bandwidth) |
| 43 | [Platform - SPI FreeRTOS](./platform_spi_freertos) |
| 44 | [Platform - Sysrtc Wakeup](./platform_sysrtc_wakeup) |
| 45 | [Platform - Timer cascading](./platform_timer_cascade) |
| 46 | [Platform - Timestamped IADC EM2 Acquisition with Timestamps](./platform_timestamped_adc_em2) |
| 47 | [Platform - UART DFU Multislot](./platform_uart_dfu_multislot) |
| 48 | [Platform - EFP01 Feature Demo](./platform_efp01) |
| 49 | [Platform - EUSART RX Timeout](./platform_eusart_rxto/) |
| 50 | [Platform - Direct Digital Synthesis (DDS) DAC](./platform_dac_dds/) |

## Requirements

1. Silicon Labs EFR32 Development Kit
2. Simplicity Studio 5
3. Compatible SDK version as specified in the README file of each project. You can install it via Simplicity Studio or download it from our GitHub [simplicity_sdk](https://github.com/SiliconLabs/simplicity_sdk).

## Working with Projects

### Adding as an External Repository

To add an external repository, perform the following steps:

- From Simplicity Studio 5, go to **Preferences > Simplicity Studio > External Repos**. Here you can add the repository `https://github.com/SiliconLabsSoftware/platform_applications.git`.

- Clone and then select the branch, tag, or commit to add. The default branch is Master. The repository will be cloned to `<path_to_the_SimplicityStudio_v5>\developer\repos\`

### Creating an Example Project

- From Launcher, select your device from the "Debug Adapters" on the left before creating a project.
- Click the **EXAMPLE PROJECTS & DEMOS** tab → check **platform_applications** under **Provider** to show a list of platform example projects compatible with the selected device.
- Click **CREATE** on a project to generate a new application from the selected template.

## Legacy Projects - Importing *.sls projects

1. Place the *.sls file(s) to be imported in a folder.

2. From Simplicity Studio 5, select **File > Import**, select the folder containing *.sls file(s). Select a project from the detected projects list and click on **Next**. Name the project and click **Finish**.

See [Import and Export](https://docs.silabs.com/simplicity-studio-5-users-guide/5.6.0/ss-5-users-guide-about-the-simplicity-ide/import-and-export) for more information.

## Porting to Another Board

To change the target board, navigate to **Project > Properties > C/C++ Build > Board/Part/SDK**. Start typing in the Boards search box and locate the desired development board, then click **Apply** to change the project settings. Ensure that the board specifics include paths, found in **Project > Properties > C/C++ General > Paths and Symbols**, correctly match the target board.

## Documentation

Official documentation can be found at our [Developer Documentation](https://docs.silabs.com/#section-mcu-wireless) page.

## Reporting Bugs/Issues and Posting Questions and Comments

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of this repository. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of this repository.

## Disclaimer

The Silicon Labs Simplicity SDK supports development with Silicon Labs IoT SoC and module devices. Unless otherwise specified in the specific directory, all examples are considered to be EXPERIMENTAL QUALITY which implies that the code provided in the repos has not been formally tested and is provided as-is. It is not suitable for production environments. In addition, this code will not be maintained and there may be no bug maintenance planned for these resources. Silicon Labs may update projects from time to time.
