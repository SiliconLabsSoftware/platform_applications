/***************************************************************************//**
 * @file
 * @brief Platform UART DFU Multislot example
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "application_upgrade.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "btl_interface.h"
#include "btl_interface_storage.h"
#include <stdio.h>
#include "string.h"

#define USART0_PORT           gpioPortA
#define USART0_TX_PIN         8
#define USART0_RX_PIN         9

#define UART_BRD_EN_PORT      gpioPortB
#define UART_BRD_EN_PIN       0

#define BUFLEN                16384

#define CFM_MSG_SiZE          2

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/

static uint8_t data_buffer[BUFLEN];                       // Buffer to receive upgrade image from Host PC over UART
static uint8_t cmd_buffer[80];                            // Buffer to receive commands from the Host PC over UART
static volatile uint32_t total_bytes = 0;

static volatile bool firmware_download_start = false;     // State variable to track FW Download Command form Host
static volatile bool firmware_upgrade_start = false;      // State variable to track FW Upgrade Command form Host
static volatile uint8_t fw_slot = 255;                    // Slot for FW Download/Upgrade. Valid values are <0/1>

static char confirm_message[] = "OK";

/*******************************************************************************
 ***************************   LOCAL FUNCTIONS   *******************************
 ******************************************************************************/
static void init_usart0(void)
{
  // Enabling the clock for GPIO and USART0 modules
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART0, true);

  // GPIO Configuration for USART0 Pins
  GPIO_PinModeSet(USART0_PORT, USART0_TX_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(USART0_PORT, USART0_RX_PIN, gpioModeInput, 0);

  // Pin Mapping for GPIO Pins
  GPIO->USARTROUTE[0].TXROUTE = (USART0_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
                                | (USART0_TX_PIN <<
                                   _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].RXROUTE = (USART0_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
                                | (USART0_RX_PIN <<
                                   _GPIO_USART_RXROUTE_PIN_SHIFT);

  // Enable RX and TX signals now that they have been routed
  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN
                                | GPIO_USART_ROUTEEN_TXPEN;

  // Default UART Configuration (115.2 Kbps, 8N1, no flow control)
  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

  // Enable USART0
  USART_InitAsync(USART0, &init);

  // Board Control Enable for USART0
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(UART_BRD_EN_PORT, UART_BRD_EN_PIN, gpioModePushPull, 1);
}

static void usart_rx_msg(uint8_t *rx_buf, uint32_t rx_len)
{
  uint32_t len = 0;
  uint8_t ch;

  while (len < rx_len)
  {
    ch = USART_Rx(USART0);
    rx_buf[len] = ch;
    len++;
  }
}

static void usart_tx_msg(const char *tx_buf, uint32_t tx_len)
{
  for (uint32_t count = 0; count < tx_len; count++) {
    USART_Tx(USART0, tx_buf[count]);
  }
}

// Download the FW image over UART into data buffer
static void download_firmware_image(void)
{
  bool download_complete = false;
  uint32_t bytes_len = 0;

  total_bytes = 0;

  // Download protocol
  // Host sends message "num_bytes\n"
  // Host send actual data bytes
  // When Host has no more data to send, it sets num_bytes to 0xFF to indicate end of download

  while (!download_complete)
  {
    usart_rx_msg(cmd_buffer, 2);
    usart_tx_msg(confirm_message, CFM_MSG_SiZE);
    if (cmd_buffer[0] == 0xFF) {
      download_complete = true;
    } else {
      bytes_len = cmd_buffer[0];
      usart_rx_msg(&data_buffer[total_bytes], bytes_len);
      usart_tx_msg(confirm_message, CFM_MSG_SiZE);
      total_bytes += bytes_len;
    }
  }
}

// Store the FW into the chosen storage slot using Bootloader APIs
static void process_firmware_download(void)
{
  if ((fw_slot == 0) || (fw_slot == 1)) {
    bootloader_init();
    bootloader_eraseStorageSlot(fw_slot);
    download_firmware_image();
    bootloader_writeStorage(fw_slot, 0, data_buffer, total_bytes);
    usart_tx_msg(confirm_message, CFM_MSG_SiZE);
  }
}

// Apply the FW upgrade from chosen slot using Bootloader APIs
static void process_firmware_upgrade(void)
{
  if ((fw_slot == 0) || (fw_slot == 1)) {
    bootloader_setImageToBootload(fw_slot);
    bootloader_rebootAndInstall();
  }
}

// Check for commands from Host PC
static void check_command(void)
{
  // Command Format
  // For FW Download: "1<slot>\n"
  // For FW Upgrade: "2<slot>\n"

  usart_rx_msg(cmd_buffer, 3);

  if ((cmd_buffer[0] == '1') && (cmd_buffer[2] == '\n')) {
    firmware_download_start = true;
    fw_slot = cmd_buffer[1];
    usart_tx_msg(confirm_message, CFM_MSG_SiZE);
  } else if ((cmd_buffer[0] == '2') && (cmd_buffer[2] == '\n')) {
    firmware_upgrade_start = true;
    fw_slot = cmd_buffer[1];
    usart_tx_msg(confirm_message, CFM_MSG_SiZE);
  }
}

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/
void application_upgrade_init(void)
{
  init_usart0();
}

void application_upgrade_process_action(void)
{
  // Check for command from Host PC
  check_command();

  // Process firmware download command
  if (firmware_download_start) {
    process_firmware_download();
    firmware_download_start = false;
  }

  // Process firmware upgrade command
  if (firmware_upgrade_start) {
    process_firmware_upgrade();
    firmware_upgrade_start = false;
  }
}
