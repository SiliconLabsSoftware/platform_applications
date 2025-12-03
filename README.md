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

[![Version Badge](https://img.shields.io/badge/-v2.11.0-green)](https://github.com/SiliconLabsSoftware/platform_applications/releases)
[![SDK Badge](https://img.shields.io/badge/GSDK-v4.5.0-green)](https://github.com/SiliconLabs/gecko_sdk/releases)
![License badge](https://img.shields.io/badge/License-Zlib-green)

This repo contains example projects that demonstrate various applications using the peripherals of Silicon Labs EFM32 and EFR32 parts.

This repository provides both SLCP projects (as External Repositories) and SLS projects as standalone projects, which are configured for development boards.

## Examples ##

| No | Example name |
|:--:|:-------------|
| 1 | [Platform - Executing Code from RAM](./platform_pg23_code_execution_ram) |
| 2 | [Platform - True Random Number Generator (TRNG)](./platform_trng) |
| 3 | [Platform - Peripheral LESENSE ADC](./platform_peripheral_lesense_adc) |
| 4 | [Platform - SegmentLCD Low Power](./platform_segmentLCD_lowpower) |
| 5 | [Platform - Segment LCD and Temperature Sensor](./platform_segmentLCD_tempsensor) |
| 6 | [Platform - IADC High Accuracy Mode - PG23 (BRD2504A)](./platform_iadc_high_accuracy_PG23) |
| 7 | [Platform - Lean Watchdog](./platform_lean_watchdog) |
| 8 | [Platform - Edge Counting Using the EFM32/EFR32 Series 1 Pulse Counter (PCNT)](./platform_pcnt_edge_counter_series1) |
| 9 | [Platform - Si7021 RHT Sensor Bare-metal](./platform_rht_baremetal) |
| 10 | [Platform - Using Autonomous Peripherals in Low Power EM2 Mode](./platform_peripheral_low_energy) |
| 11 | [Platform - DALI Communication using EUSART (Main device)](./platform_dali) |
| 12 | [Platform - DALI Communication using EUSART (Secondary device)](./platform_dali) |
| 13 | [Platform - DALI Communication using bitbang SPI (Main device)](./platform_dali) |
| 14 | [Platform - DALI Communication using bitbang SPI (Secondary device)](./platform_dali) |
| 15 | [Platform - DALI Communication using bitbang SPI with DMADRV (Main device)](./platform_dali) |
| 16 | [Platform - DALI Communication using bitbang SPI with DMADRV (Secondary device)](./platform_dali) |
| 17 | [Platform - Segment LCD with LDMA](./platform_segmentLCD_ldma) |
| 18 | [Platform - IADC High-speed Mode](./platform_iadc_high_speed) |
| 19 | [Platform - Segment LCD with Timer](./platform_segmentLCD_timer) |
| 22 | [Platform - Segment LCD with LC Sensor](./platform_segmentLCD_lcsense) |
| 23 | [Platform - IADC with LDMA Ping-Pong](./platform_iadc_ldma_ping_pong) |
| 24 | [Platform - UART Circular Buffer with LDMA](./platform_uart_ldma_circular_buffer) |
| 25 | [Platform - Sisnake](./platform_sisnake) |
| 26 | [Platform - PG28 LCD Animation Blink](./platform_lcd_animation_blink_pg28) |
| 27 | [Platform - PG23 LCD Animation Blink](./platform_lcd_animation_blink_pg23) |
| 28 | [Platform - FG23 LCD Animation Blink](./platform_lcd_animation_blink_fg23) |
| 29 | [Platform - Asynchronous EM01GRPA Clock Output](./platform_async_em01grpaclk_out) |
| 30 | [Platform - Flash Read-While-Write](./platform_flash_read_while_write) |
| 31 | [Platform - I2C Read to NVM3 Test](./platform_i2c_to_nvm3_test) |
| 32 | [Platform - I2C Test for EFM8 IOExpander (IOEXP)](./platform_i2c_test_for_efm8_ioexpander) |
| 33 | [Platform - RTCC and Watchdog Bare-metal](./platform_rtcc_lcd_wdog) |
| 34 | [Platform - I2C Slave Bootloader](./platform_i2cslave_bootloader) |
| 35 | [Platform - WS2812 RGB LED Driver](./platform_rgb_led) |
| 36 | [Platform - SensiML IMU Data Capture](./platform_SensiML/platform_SensiML_DataCaptureLab/SensiML_IMU) |
| 37 | [Platform - SensiML IMU Data Capture with BLE](./platform_SensiML/platform_SensiML_DataCaptureLab/SensiML_IMU_BLE) |
| 38 | [Platform - SensiML Microphone Data Capture](./platform_SensiML/platform_SensiML_DataCaptureLab/SensiML_Microphone) |
| 39 | [Platform - SensiML IMU Recognition](./platform_SensiML/platform_SensiML_Recognition/SensiML_IMU) |
| 40 | [Platform - SensiML IMU Recognition with BLE](./platform_SensiML/platform_SensiML_Recognition/SensiML_IMU_BLE) |
| 41 | [Platform - SensiML Microphone Recogniton](./platform_SensiML/platform_SensiML_Recognition/SensiML_Microphone) |
| 42 | [Platform - Squash FreeRTOS Glib](./platform_squash_freertos_glib) |
| 43 | [Platform - NVM3 Integrity Test](./platform_nvm3_integrity_test) |
| 44 | [Platform - Micrium Multiple ADC LMDA Task](./platform_micrium_multiple_adc_ldma_task) |
| 45 | [Platform - Linked DMA (LDMA) Arbitration](./platform_peripheral_ldma_arbitration) |
| 46 | [Platform - Parse GBL Metadata in Single Shot](./platform_bootloader_interface/parse_gbl_metadata_singleshot) |
| 47 | [Platform - Parse GBL Metadata in BufferSize Steps](./platform_bootloader_interface/parse_gbl_metadata_buffersize_steps) |
| 48 | [Platform - QI PRx communication protocol](./platform_qi_rx_base) |
| 49 | [Platform - QI PRx communication protocol with fast response](./platform_qi_rx_base) |
| 50 | [Platform - EFR32xG21 LDMA SPI Throughput](./platform_spi_flash_bandwidth/ldma) |
| 51 | [Platform - EFR32xG21 Polled SPI Throughput](./platform_spi_flash_bandwidth/polled) |
| 52 | [Platform - Flash blank checking using the GPCRC](./platform_gpcrc_blank_check) |
| 53 | [Platform - Timer cascading](./platform_timer_cascade) |
| 54 | [Platform - Sine null points detector](./platform_sine_null_points_detector) |
| 55 | [Platform - IADC Tailgating](./platform_iadc_tailgating) |
| 56 | [Platform - IADC scan multiple external inputs](./platform_iadc_scan_multiple_external_input) |
| 57 | [Platform - LDMA trigger with TIMER](./platform_ldma_trigger_with_timer) |
| 58 | [Platform - Multi channel PWM](./platform_multi_channel_pwm) |
| 59 | [Platform - BLDC Motor Controller](./platform_bldc_motor_controller) |
| 60 | [Platform - KeyScan](./platform_keyscan) |
| 61 | [Platform - PG26 LCD Animation Blink](./platform_lcd_animation_blink_pg26) |
| 62 | [Platform - IADC loopback with VDAC](./platform_iadc_loopback) |
| 63 | [Platform - Polled HFRCO Calibration](./platform_peripheral_hfrco_cal_polled) |
| 64 | [Platform - Report Time Spent in EM4](./platform_em4_duration) |
| 65 | [Platform - Wake M33 using PRS](./platform_prs_wfe) |
| 66 | [Platform - IADC Synchronous Triggers with PRS](./platform_iadc_synch_prs) |
| 67 | [Platform - EMU Temperature Linearization](./platform_emu_temp_linearization) |
| 68 | [Platform - I2C Fast Plus Mode](./platform_i2c_fast_plus_mode) |
| 69 | [Platform - ACMP Hysteresis Mode](./platform_acmp_hysteresis_mode) |
| 70 | [Platform - Sysrtc Wakeup](./platform_sysrtc_wakeup) |
| 71 | [Platform - Fixed Gate Time Frequency Measurement](./platform_fixed_gate_time_frequency_measurement/) |
| 72 | [Platform - UART DFU Multislot](./platform_uart_dfu_multislot/) |
| 73 | [Platform - I2S Microphone VDAC](./platform_i2s_mic_vdac/) |
| 74 | [Platform - SPI FreeRTOS](./platform_spi_freertos/) |
| 75 | [Platform - Guitar and Ukulele Tuner](./platform_guitar_and_ukulele_tuner/) |
| 76 | [Platform - Random Number Generator using ADC](./platform_adc_rng) |
| 77 | [Platform - CryoTimer](./platform_cryotimer) |
| 78 | [Platform - EFP01 Feature Demo](./platform_efp01_feature_demo) |
| 79 | [Platform - Sensor Alive Check in Low Power EM2 Mode](./platform_em2_sensor_alive_check) |
| 80 | [Platform - EMU Temperature Sensor](./platform_emu_temp) |
| 81 | [Platform - I2C Leader Interrupt](./platform_i2c_leader_interrupt) |
| 82 | [Platform - I2C SMbus SCL Low Timeout](./platform_i2c_smbus_scl_low_timeout) |
| 83 | [Platform - IADC Stimulus](./platform_iadc_stimulus) |
| 84 | [Platform - Micrium Cryotimer EM3 Sleep](./platform_micrium_cryotimer_em3_sleep) |

## Requirements ##

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
