# Platform - Timestamped IADC EM2 Acquisition with Timestamps #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v2025.6.2-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-13.0%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-6.58%20KB-blue)

## Overview ##

This project performs low-energy IADC conversions in EM2 and records a timestamp for every sample. Conversions are started by external digital pulses on a GPIO routed through PRS. Two adjacent, fixed-priority LDMA channels transfer (1) the timestamp and then (2) the conversion result, guaranteeing the timestamp is captured before the device returns to EM2.

Energy use is dominated by the sampling rate (number of conversions per second) rather than the IADC conversion clock. The IADC clock is set to 1 MHz from HFRCOEM23; at a 100 Hz sampling rate representative current is in the few‑tens of µA range (see device datasheets for exact numbers).

Peripherals used: CMU, EMU, GPIO, IADC, LDMA, PRS, RTCC.

## SDK version ##

- [Simplicity SDK v2025.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2025.6.2)

## Hardware Required ##

- Wireless Starter Kit (WSTK) Mainboard (SLWMB4001A / BRD4001A)
- One of the following radio boards:
  - BRD4179B (EFR32MG21 + EFP0104)
  - BRD4180A (EFR32MG21 20 dBm)
  - BRD4181A (EFR32MG21 10 dBm)
- External stimulus generator for trigger and analog signal
- Jumper wires for trigger and analog signal

| Radio Board | Target Device Variant | Notes |
|-------------|-----------------------|-------|
| BRD4179B | EFR32MG21A010F1024 + EFP0104 | 1.8 V AVDD from EFP0104 |
| BRD4180A | EFR32MG21A020F1024 (20 dBm) | 3.3 V VMCU supply |
| BRD4181A | EFR32MG21A020F1024 (10 dBm) | 3.3 V VMCU supply |

## Connections Required ##

External stimulus provides both the trigger pulses and an analog voltage between 0 and VDDX.

| Radio Board | Trigger Input (EXP) | Analog Input (EXP) | VDDX Reference (V) |
|-------------|---------------------|--------------------|--------------------|
| BRD4179B | PA5 / EXP12 | PD2 / EXP11 | 1.8 |
| BRD4180A | PA5 / EXP12 | PB0 / EXP11 | 3.3 |
| BRD4181A | PA5 / EXP12 | PB0 / EXP11 | 3.3 |

Use jumper wires from the stimulus generator board's pulse and analog outputs to the corresponding expansion header pins above.

## Setup ##

You can either create a project directly from this example or start from an empty project.

### Create a project based on an example project ###

1. Add this repository to External Repos in Simplicity Studio.
2. Locate **Platform - Timestamped IADC EM2 Acquisition with Timestamps** (filter for `timestamped` or `iadc`).
3. Create, build and flash to the chosen MG21 radio board.

### Start with an empty example project ###

1. Create an **Empty C Project** for the target board (e.g. BRD4179B).
2. Copy this example's `src` contents into your project.
3. Install software components:
   - [Platform] → [Peripheral] → [IADC]
   - [Platform] → [Peripheral] → [GPIO]
   - [Platform] → [Services] → [Power Manager]
   - [Platform] → [Services] → [RTCC]
   - [Platform] → [Services] → [LDMA]
4. Build and flash.

## How It Works ##

Core idea: leverage fixed LDMA channel priority so the timestamp transfer always precedes the conversion result transfer after a wake-up.

Sequence:

1. (Optional) Initialize EFP regulator on BRD4179B for 1.8 V AVDD.
2. Configure low-energy operation (EM2/EM3) and set HFRCOEM23 to selected 1 MHz band.
3. Configure RTCC (prescaler = 1) so each tick = 1 / 32768 s.
4. Configure IADC: 1 MHz clock (10 µs conversion), wake on single FIFO entry.
5. Allocate two adjacent LDMA channels:
   - Channel N: triggered by PRS request (LDMAXBAR_DMA_PRSREQ0) to move RTCC CNT value to `timebuffer`.
   - Channel N+1: triggered by IADC single conversion completion to move result to `valbuffer`.
6. Set up PRS routes from the GPIO trigger pin to both LDMA requester and IADC single trigger consumer.
7. Enter EM2. Each external pulse produces: wake → RTCC timestamp capture → IADC conversion → LDMA transfers (timestamp then sample) → return to EM2.
8. Continue until `NUM_SAMPLES` timestamps and samples captured; then execute a breakpoint for inspection.

Why two channels? The LDMA services the lower-numbered channel first, ensuring the timestamp is safely copied before the conversion result might allow re-entry to EM2.

## Testing ##

1. Program this project onto a MG21 board.
2. Connect an external stimulus generator to provide trigger pulses and analog voltage.
3. Wire stimulus outputs: pulse → EXP12 (PA5), analog → EXP11 (see table). Ensure analog voltage in 0..VDDX range.
4. Set `NUM_SAMPLES` in source (if adjustable) to desired count (e.g. 64).
5. Start debugger, press the stimulus board button to generate bursts.
6. After capture completes, debugger hits breakpoint. Open Expressions view and add `valbuffer` and `timebuffer` to inspect data.
7. Verify timestamp deltas align with stimulus pulse period and analog values reflect applied voltage.

## Additional Notes ##

- Sampling rate drives energy consumption; lower external pulse rates reduce average current.
- Replace external trigger with a low-energy internal source (e.g., RTCC compare) by adjusting PRS producer.
- Porting to EFR32xG22 requires explicit enabling of module clocks (GPIO, IADC, LDMA, PRS, RTCC) because on-demand gating differs.
- Devices without VDAC can still provide analog stimulus via resistor ladder or external DAC.
- Flash/RAM badge estimates; actual usage varies with `NUM_SAMPLES`. Adjust after building if desired.
- Ensure two adjacent LDMA channels remain fixed-priority (do not enable round-robin for them) to preserve ordering.
