/***************************************************************************//**
 * @file app.c
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 * # Experimental Quality
 * This code has not been formally tested and is provided as-is. It is not
 * suitable for production environments. In addition, this code will not be
 * maintained and there may be no bug maintenance planned for these resources.
 * Silicon Labs may update projects from time to time.
 ******************************************************************************/
#include <stdio.h>
#include "em_gpio.h"

#include "btl_interface.h"
#include "application_properties.h"

#define GPIO_LED1_PORT      gpioPortF
#define GPIO_LED1_PIN       5
#define GPIO_PB1_PORT       gpioPortF
#define GPIO_PB1_PIN        7

static volatile uint8_t flag_verify = 0;

static const uint32_t buffer_size = 64;

// Metadata parse function
static void metadata_parsing_function(void);

// Parse callback function
static void metadata_parsing_callback(uint32_t offset,
                                      uint8_t *buffer,
                                      size_t length,
                                      void *ctx);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  // Configure PB1 as input with glitch filter enabled
  GPIO_PinModeSet(GPIO_PB1_PORT, GPIO_PB1_PIN, gpioModeInputPullFilter, 1);

  // Configure LED1 as output
  GPIO_PinModeSet(GPIO_LED1_PORT, GPIO_LED1_PIN, gpioModePushPull, 0);

  // Enable IRQ for odd numbered GPIO pins
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  // Enable falling-edge interrupts for PB1 pin
  GPIO_ExtIntConfig(GPIO_PB1_PORT, GPIO_PB1_PIN, GPIO_PB1_PIN, 0, 1, true);

  printf("Welcome\r\n");
  printf("OTA/flash the metadata GBL file at the beginning of slot0\r\n");
  printf("When ready press PB1 for GBL Signature verification "
         "and Parsing\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  int32_t ret;

  if (flag_verify == 1) {
    flag_verify = 0;

    // Turn on LED1
    GPIO_PinOutSet(GPIO_LED1_PORT, GPIO_LED1_PIN);

    ret = bootloader_verifyImage(0, NULL);

    if (ret != BOOTLOADER_OK) {
      printf("GBL verification ERROR: 0x%08lX\n\r", ret);
    } else {
      printf("GBL verification SUCCESS\n\r");

      // Parsing the metadata in the slot0
      metadata_parsing_function();
    }
  }
}

/**************************************************************************//**
 * @brief GPIO Odd IRQ for pushbuttons on odd-numbered pins and to start
 * OTA GBL image verification and parsing for metadata
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  // Clear all odd pin interrupt flags
  GPIO_IntClear(0xAAAA);

  // turn flag on
  flag_verify = 1;
}

/**************************************************************************//**
 * @brief Parser Callback Function
 *****************************************************************************/
void metadata_parsing_callback(uint32_t offset,
                               uint8_t *buffer,
                               size_t length,
                               void *ctx)
{
  (void) offset;
  (void) ctx;
  // buffer contains the raw data parsed. offset does not really matter here.
  for (uint32_t i = 0; i < length; i++)
  {
    if ((i % 16) == 0) {
      printf("\r");
    }
    printf("%02X ", buffer[i]);
  }
}

/**************************************************************************//**
 * @brief Metadata GBL Parsing Function
 *****************************************************************************/
void metadata_parsing_function(void)
{
  int32_t ret = 0;
  BootloaderStorageSlot_t slot;
  uint32_t offset = 0, new_address;
  uint8_t ctx[BOOTLOADER_STORAGE_VERIFICATION_CONTEXT_SIZE];
  BootloaderParserCallbacks_t callback = {
    .metadataCallback = metadata_parsing_callback
  };

  bootloader_getStorageSlotInfo(0, &slot);
  new_address = slot.address;

  // Application interface image parser for decrypting and parsing GBL image
  ret = bootloader_initParser((BootloaderParserContext_t *)ctx,
                              BOOTLOADER_STORAGE_VERIFICATION_CONTEXT_SIZE);

  while (offset < slot.length) {
    if (offset < slot.length - buffer_size) {
      // Parse the complete GBL file and send the raw data to callback function
      ret = bootloader_parseBuffer((BootloaderParserContext_t *)ctx,
                                   &callback,
                                   (uint8_t *)new_address,
                                   buffer_size);
      offset += buffer_size;
      new_address = new_address + buffer_size;
    } else {
      // Parse the complete GBL file and send the raw data to callback function
      ret = bootloader_parseBuffer((BootloaderParserContext_t *)ctx,
                                   &callback,
                                   (uint8_t *)new_address,
                                   4);
      offset += 4;
      new_address = new_address + 4;
    }

    if (ret == BOOTLOADER_ERROR_PARSE_SUCCESS) {
      printf("GBL metadata parse: SUCCESS\n\r");
      break;
    } else if (ret != BOOTLOADER_ERROR_PARSE_CONTINUE) {
      printf("GBL metadata parse failed, ERROR: 0x%08lX\n\r", ret);
      break;
    }
  }
}
