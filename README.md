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

> [!CAUTION]
> You are viewing an older version of the repository. This major version is no longer maintained and may only receive critical bug fixes. Please check out the [main](https://github.com/SiliconLabsSoftware/platform_applications) branch for the latest version.

# Silicon Labs Platform Applications

[![Version Badge](https://img.shields.io/badge/-v2.12.0-green)](https://github.com/SiliconLabsSoftware/platform_applications/releases)
[![SDK Badge](https://img.shields.io/badge/GSDK-v4.5.0-green)](https://github.com/SiliconLabs/gecko_sdk/releases)
![License badge](https://img.shields.io/badge/License-Zlib-green)

This repo contains example projects that demonstrate various applications using the peripherals of Silicon Labs EFM32 and EFR32 parts.

This repository provides both SLCP projects (as External Repositories) and SLS projects as standalone projects, which are configured for development boards.

## Examples

| No | Example name |
|:--:|:-------------|
| 1 | [Platform - Random Number Generator using ADC](platform_adc_rng/) |
| 2 | [Platform - Asynchronous EM01GRPA Clock Output](./platform_async_em01grpaclk_out/) |
| 3 | [Platform - Bootloader Interface](./platform_bootloader_interface/) |
| 4 | [Platform - Executing Code from RAM](./platform_code_execution_ram/) |
| 5 | [Platform - Cryotimer](./platform_cryotimer/) |
| 6 | [Platform - DALI Communication](./platform_dali/) |
| 7 | [Platform - EFP01](./platform_efp01/) |
| 8 | [Platform - Sensor Alive Check in Low Power EM2 Mode](./platform_em2_sensor_alive_check/) |
| 9 | [Platform - EMU Temperature Sensor](./platform_emu_temp/) |
| 10 | [Platform - Flash Read-While-Write](./platform_flash_read_while_write/) |
| 11 | [Platform - Flash blank checking using the GPCRC](./platform_gpcrc_blank_check/) |
| 12 | [Platform - Guitar and Ukulele Tuner](./platform_guitar_and_ukulele_tuner/) |
| 13 | [Platform - I2C Leader Interrupt](./platform_i2c_leader_interrupt/) |
| 14 | [Platform - I2C Smbus Scl Low Timeout](./platform_i2c_smbus_scl_low_timeout/) |
| 15 | [Platform - I2C Test for EFM8 IOExpander](./platform_i2c_test_for_efm8_ioexpander/) |
| 16 | [Platform - I2C Read to NVM3 Test](./platform_i2c_to_nvm3_test/) |
| 17 | [Platform - I2C Slave Bootloader](./platform_i2cslave_bootloader/) |
| 18 | [Platform - IADC High Accuracy Examples](./platform_iadc_high_accuracy/) |
| 19 | [Platform - IADC High-Speed Mode](./platform_iadc_high_speed/) |
| 20 | [Platform - IADC with LDMA Ping-Pong](./platform_iadc_ldma_ping_pong/) |
| 21 | [Platform - IADC scan multiple external inputs](./platform_iadc_scan_multiple_external_input) |
| 22 | [Platform - IADC Stimulus](./platform_iadc_stimulus/) |
| 23 | [Platform - IADC Tailgating](./platform_iadc_tailgating/) |
| 24 | [Platform - KeyScan](./platform_keyscan/) |
| 25 | [Platform - LCD Animation Blink](./platform_lcd_animation_blink/) |
| 26 | [Platform - Linked DMA (LDMA) Arbitration](./platform_ldma_arbitration/) |
| 27 | [Platform - LDMA trigger with TIMER](./platform_ldma_trigger_with_timer/) |
| 28 | [Platform - Lean Watchdog](./platform_lean_watchdog/) |
| 29 | [Platform - Peripheral LESENSE ADC](./platform_lesense_adc/) |
| 30 | [Platform - EM3 Sleep with Micrium Using the Cryotimer](./platform_micrium_cryotimer_em3_sleep/) |
| 31 | [Platform - Micrium Multiple ADC LMDA Task](./platform_micrium_multiple_adc_ldma_task/) |
| 32 | [Platform - Multi Channel PWM](./platform_multi_channel_pwm/) |
| 33 | [Platform - NVM3 Integrity Test](./platform_nvm3_integrity_test/) |
| 34 | [Platform - Edge Counting Using Pulse Counter (PCNT)](./platform_pcnt_edge_counter/) |
| 35 | [Platform - Using Autonomous Peripherals in Low Power EM2 Mode](./platform_peripheral_low_energy/) |
| 36 | [Platform - QI PRx communication protocol](./platform_qi_rx_base/) |
| 37 | [Platform - WS2812 RGB LED Driver](./platform_rgb_led/) |
| 38 | [Platform - Si7021 RHT Sensor Bare-metal](./platform_rht_baremetal/) |
| 39 | [Platform - RTCC and Watchdog Bare-metal](./platform_rtcc_lcd_wdog/) |
| 40 | [Platform - Segment LCD with LC Sensor](./platform_segmentLCD_lcsense/) |
| 41 | [Platform - SensiML](./platform_SensiML/) |
| 42 | [Platform - Sisnake](./platform_sisnake/) |
| 43 | [Platform - SPI Throughput](./platform_spi_flash_bandwidth/) |
| 44 | [Platform - Squash FreeRTOS Glib](./platform_squash_freertos_glib/) |
| 45 | [Platform - Timer cascading](./platform_timer_cascade/) |
| 46 | [Platform - Timestamped IADC EM2 Acquisition with Timestamps](./platform_timestamped_adc_em2) |
| 47 | [Platform - True Random Number Generator (TRNG)](./platform_trng/) |
| 48 | [Platform - UART Circular Buffer with LDMA](./platform_uart_ldma_circular_buffer/) |
| 49 | [Platform - VDAC Calibration](./platform_vdac_calibration/) |

## Requirements

1. Silicon Labs EFR32 Development Kit
2. Simplicity Studio 5
3. Compatible GSDK version as specified in each project's README file. You can install it via Simplicity Studio or download it from our GitHub [gecko_sdk](https://github.com/SiliconLabs/gecko_sdk)

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

The Gecko SDK suite supports development with Silicon Labs IoT SoC and module devices. Unless otherwise specified in the specific directory, all examples are considered to be EXPERIMENTAL QUALITY which implies that the code provided in the repos has not been formally tested and is provided as-is. It is not suitable for production environments. In addition, this code will not be maintained and there may be no bug maintenance planned for these resources. Silicon Labs may update projects from time to time.
