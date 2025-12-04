/***************************************************************************//**
 * @file
 * @brief Top level application functions
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#include "em_eusart.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include <stddef.h>

#define EUSART_TX_PORT       gpioPortA
#define EUSART_TX_PIN        5

#define EUSART_RX_PORT       gpioPortA
#define EUSART_RX_PIN        6

#define EUSART_IF_CHECK_PORT gpioPortB
#define EUSART_IF_CHECK_PIN  0

static volatile uint8_t buffer[8];

// Current position in buffer
static volatile uint32_t inpos = 0;
static volatile uint32_t outpos = 0;

// True while receiving data (waiting for CR or BUFLEN characters)
static volatile bool receive = true;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
static void eusart_init(void)
{
  EUSART_UartInit_TypeDef init = EUSART_UART_INIT_DEFAULT_HF;
  init.enable = eusartDisable;

  // Configure the TX pin
  GPIO_PinModeSet(EUSART_TX_PORT, EUSART_TX_PIN, gpioModePushPull, 1);

  // Configure the RX pin
  GPIO_PinModeSet(EUSART_RX_PORT, EUSART_RX_PIN, gpioModeInput, 0);

  // Use to visualize RXTO
  GPIO_PinModeSet(EUSART_IF_CHECK_PORT,
                  EUSART_IF_CHECK_PIN,
                  gpioModePushPull, 1);

  // Enable RX and TX signals now that they have been routed
  GPIO->EUSARTROUTE[1].ROUTEEN = GPIO_EUSART_ROUTEEN_RXPEN
                                 | GPIO_EUSART_ROUTEEN_TXPEN;
  // Route EUSART1 TX and RX to the board controller TX and RX pins
  GPIO->EUSARTROUTE[1].TXROUTE = (EUSART_TX_PORT
                                  << _GPIO_EUSART_TXROUTE_PORT_SHIFT)
                                 | (EUSART_TX_PIN
                                    << _GPIO_EUSART_TXROUTE_PIN_SHIFT);
  GPIO->EUSARTROUTE[1].RXROUTE = (EUSART_RX_PORT
                                  << _GPIO_EUSART_RXROUTE_PORT_SHIFT)
                                 | (EUSART_RX_PIN
                                    << _GPIO_EUSART_RXROUTE_PIN_SHIFT);

  // Configure and enable EUSART1
  CMU_ClockEnable(cmuClock_EUSART1, true);
  EUSART_UartInitHf(EUSART1, &init);

  // Enable NVIC USART sources
  NVIC_ClearPendingIRQ(EUSART1_RX_IRQn);
  NVIC_ClearPendingIRQ(EUSART1_TX_IRQn);

  NVIC_EnableIRQ(EUSART1_TX_IRQn);
  NVIC_EnableIRQ(EUSART1_RX_IRQn);

  EUSART1->CFG1 |= EUSART_CFG1_RXTIMEOUT_FOURFRAMES; // enable RXTO in 4 frames
  EUSART_Enable(EUSART1, eusartEnable);
}

void app_init(void)
{
  eusart_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  // Zero out buffer
  for (uint8_t i = 0; i < 8; i++) {
    buffer[i] = 0;
  }

  // Enable receive FIFO level interrupt (defaults to one frame)
  EUSART_IntEnable(EUSART1, EUSART_IEN_RXFL);

  while (receive) {}

  // Disable receive FIFO level interrupt
  EUSART_IntDisable(EUSART1, EUSART_IEN_RXFL);

  // Enable transmit FIFO level interrupt (defaults to one frame)
  EUSART_IntEnable(EUSART1, EUSART_IEN_TXFL);

  while (!receive) {}

  // Enable RXTO interrupt
  EUSART_IntEnable(EUSART1, EUSART_IEN_RXTO);

  // Reset buffer
  inpos = outpos = 0;
}

/**************************************************************************//**
 * @brief
 *    The EUSART1 receive interrupt saves incoming characters.
 *****************************************************************************/
void EUSART1_RX_IRQHandler(void)
{
  // check if RXTO occur
  if (EUSART_IntGet(EUSART1) & EUSART_IF_RXTO) {
    GPIO_PinOutClear(EUSART_IF_CHECK_PORT, EUSART_IF_CHECK_PIN);
    EUSART_IntClear(EUSART1, EUSART_IF_RXTO);
    return;
  }

  GPIO_PinOutSet(EUSART_IF_CHECK_PORT, EUSART_IF_CHECK_PIN);

  // Get the character just received
  buffer[inpos] = EUSART1->RXDATA;

  // Exit loop on new line or buffer full
  if ((buffer[inpos] != '\r') && (inpos < 7)) {
    inpos++;
  } else {
    receive = false; // Stop receiving on CR
  }
  EUSART_IntClear(EUSART1, EUSART_IF_RXFL);
}

/**************************************************************************//**
 * @brief
 *    The EUSART1 transmit interrupt outputs characters.
 *****************************************************************************/
void EUSART1_TX_IRQHandler(void)
{
  // Send a previously received character
  if (outpos < inpos) {
    EUSART1->TXDATA = buffer[outpos++];
    EUSART_IntClear(EUSART1, EUSART_IF_TXFL);
  } else {
    receive = true;  // Go back into receive when all is sent
    EUSART_IntDisable(EUSART1, EUSART_IEN_TXFL);
  }
}
