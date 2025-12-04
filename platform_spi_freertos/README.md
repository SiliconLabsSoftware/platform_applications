# Platform - SPI FreeRTOS #

![Type badge](https://img.shields.io/badge/Type-Virtual%20Application-green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v2025.6.2-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-21.64%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-6.53%20KB-blue)


## Summary ##

This project demonstrates the interrupt-driven operation of the USART peripheral in
synchronous main mode integrated into a FreeRTOS environment.  
USART0 is configured for SPI-compatible operation at 1 Mbps.  
Compatibility with the Synchronous Peripheral Interface standard 
implies a word size of 8 data bits transmitted and received MSB-first.

## SDK Version ##

- [Simplicity SDK v2025.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2025.6.2)

## Hardware Required ##

* Board: [EFR32xG21A Wireless Gecko Starter Kit](https://www.silabs.com/development-tools/wireless/efr32xg21-wireless-starter-kit?tab=overview)

## Connections Required ##
* Connect the board via a micro-USB or USB-C cable to your PC to flash the example.
* The peer Starter kit is connected via PIN wire jumper referred at test procedure

## Setup ##

### Project is based on an existing example project ###

1. From the Launcher Home, add the BRD4180A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with the filter "SPI".

2. Click **Create** button on the **Platform - SPI FreeRTOS** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

3. Build and flash this example to the board.

## How It Works ##

An SPI trask performs the USART operation.

An output buffer is filled with 10 bytes of data (which can be changed by
modifying BUFLEN).  The USART chip select pin (controlled as a GPIO), is
asserted (driven low) to notify a secondary device that data will soon be on
the way.

This example inserts a delay of around 15 us after chip select
assertion in order to allow the secondary device (expected to be one of the
equivalent EFM32/EFR32 interrupt-driven examples) to exit its low-power
state and pre-load its transmit buffer with the first byte of data to be
returned.

The first byte of data to be transmitted (outbuf[0]) is written to the
USART0_TXDATA register then the task waits for TX termination while 
other tasks are allowed to be scheduled or the device may enter the 
lowest enabled EM low-energy mode when it is in idle state<BR>
(No other task is scheduled).

While the CPU is halted, USART0 drives 
the  CLK pin such that each pulse causes the byte written to TXDATA 
to be shifted out while an incoming byte is shifted into RXDATA one
bit at a time.

Once the last CLK pulse has been driven, USART0 asserts its transmit
complete flag (USART0_STATUS_TXC) and the associated interrupt request
(USART0_IF_TXC).  This causes the processor to exit the EM low-energy
mode (unless it was operational as other tasks were performed)
and enter the USART0_TX_IRQHandler() where a semaphore is set
to let continue running the SPI task that reads the incoming data from RXDATA.

The TXC interrupt flag is cleared before exiting the IRQ handler.

Until BUFLEN characters have been transferred, the device repeatedly
may enter to EM, waiting for the transmit complete interrupt to wake it up.

After each BUFLEN character has been transmitted and received, the chip
select is de-asserted (driven high).  A breakpoint can be set at the
call to GPIO_PinOutSet() when the chip selected is de-asserted to
inspect the received data.

Before repeating the process, another delay (around 1 ms) is inserted
because the secondary implementations running on slower devices (e.g. either
lower clock rate or lower IPC CPU such as the Cortex-M0+) need extra time
to prepare the input and output buffers for the next round of bytes to be
transferred.  Without this delay, these devices effectively skip
receiving data on every other chip select assertion.

If **TEST** macro is defined in app.c the SPI opeartes in loopback mode'

Note: This example uses inclusive lexicon wherever possible. For more
information, visit https://www.silabs.com/about-us/inclusive-lexicon-project

-------

Peripherals Used:

GPIO<BR>
USART0

The CMU is used indirectly via the USART_InitSync() function to calculate
the divisor necessary to derive the desired bit rate.

## Test Procedure ##

Test Procedure:

1. This example requires two Starter Kits (STKs) for any two EFM32 or
   EFR32 devices. Build the spi_secondary_interrupt example and download
   it to the one board (secondary_device).

2. Build this project and download it to the another board (primary board).

3. Connect the Expansion Header pins from the primary to the secondary board
   as follows:

    | Primary WSTK   |       | Secondary WSTK|
    |-------------|-------|-------------|
    |Pin 1  (GND) | <---> |Pin 1  (GND)<BR>|
    |Pin 4  (MOSI)| <---> |Pin 4  (MOSI)<BR>|
    |Pin 6  (MISO)| <---> |Pin 6  (MISO)<BR>|
    |Pin 8  (SCLK)| <---> |Pin 8  (SCLK)<BR>|
    |Pin 10 (CSn) | <---> |Pin 10 (CSn)     |

   NOTE: Some radio boards do not follow this convention for SPI
   pin-mapping. See the supported board list below for specific
   assignments.

4. Before running the example, set a breakpoint on the GPIO_PinOutSet
   function call that de-asserts the chip select. Examine the _inbuf_[]
   array to see the received data from the secondary. This example
   transfers data continuously but will not start until PB0 is pressed!

-----------

Listed below is the port and pin mappings for working with this example.

Board: Silicon Labs EFR32xG21 2.4 GHz 20 dBm Radio Board (BRD4180A) 
       + Wireless Starter Kit Mainboard (BRD4001A) OR Wireless Pro Kit Mainboard (BRD4002A)<br>
|Device: EFR32MG21A020F1024IM32<BR>|
|----------------------------------|
|PC00  - USART0_TX (MOSI)  - Expansion Header pin 4<BR>|
|PC01  - USART0_RX (MISO)  - Expansion Header pin 6<BR>|
|PC02  - USART0_CLK (SCLK) - Expansion Header pin 8<BR>|
|PC03  - USART0_CS (CSn)   - Expansion Header pin 10<BR>|