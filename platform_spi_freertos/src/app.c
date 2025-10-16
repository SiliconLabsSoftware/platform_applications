/*******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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

#include "blink.h"
#include "sl_clock_manager.h"
#include "sl_interrupt_manager.h"
#include "sl_device_gpio.h"
#include "sl_gpio.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

// SPI ports and pins
#define US0MOSI_PORT              gpioPortC
#define US0MOSI_PIN               0
#define US0MISO_PORT              gpioPortC
#define US0MISO_PIN               1
#define US0CLK_PORT               gpioPortC
#define US0CLK_PIN                2
#define US0CS_PORT                gpioPortC
#define US0CS_PIN                 3

#define BUTTN0_PORT               gpioPortD
#define BUTTN0_PIN                2

// Size of the data buffers and task stack
#define BUFLEN                    10

#define SPI_TASK_STACK_SIZE       ((uint16_t)(250))

// #define TEST

// Outgoing data
uint8_t outbuf[BUFLEN];

// Incoming data
uint8_t inbuf[BUFLEN];

// Position in the buffer
uint32_t bufpos = 0;

// GPIO structure
sl_gpio_t btn = {
  .port = BUTTN0_PORT,
  .pin = BUTTN0_PIN
};

// External interrupt number used for button to start the SPI task
int32_t intno = 3;

// Task handle
TaskHandle_t xHandle = NULL;

static StaticTask_t xTaskBuffer;
static StackType_t  xStack[SPI_TASK_STACK_SIZE];

// Semaphore handler
xSemaphoreHandle xSemaphore;

void init_USART_GPIO(void);

void init_USART0(void);

void SPI_Task(void *arg);

void button_cb(uint8_t, void *);

void app_init(void)
{
  blink_init();

  init_USART_GPIO();

  init_USART0();

  xSemaphore = xSemaphoreCreateBinary( );

  // Create SPI Task without using any static memory allocation
  xHandle = xTaskCreateStatic(SPI_Task,
                              "SPI task",
                              SPI_TASK_STACK_SIZE,
                              ( void * ) NULL,
                              tskIDLE_PRIORITY + 2,
                              xStack,
                              &xTaskBuffer);
}

void init_USART_GPIO(void)
{
  // Configure MOSI pin as an output
  GPIO_PinModeSet(US0MOSI_PORT, US0MOSI_PIN, gpioModePushPull, 0);

  // Configure MISO pin as an input
  GPIO_PinModeSet(US0MISO_PORT, US0MISO_PIN, gpioModeInput, 0);

  // Configure CLK pin as an output
  GPIO_PinModeSet(US0CLK_PORT, US0CLK_PIN, gpioModePushPull, 0);

  // Configure CS pin as an output initially high
  GPIO_PinModeSet(US0CS_PORT, US0CS_PIN, gpioModePushPull, 1);

  // Configure button 0 pin as an input
  GPIO_PinModeSet(BUTTN0_PORT, BUTTN0_PIN, gpioModeInputPull, 1);

  // Interrupt on button 0 rising edge to start transfers
  // GPIO_ExtIntConfig(BUTTN0_PORT, BUTTN0_PIN, 3, true, false, true);
  sl_gpio_configure_external_interrupt(&btn,
                                       &intno,
                                       SL_GPIO_INTERRUPT_RISING_EDGE,
                                       button_cb,
                                       NULL);
}

void init_USART0(void)
{
  sl_clock_manager_enable_bus_clock(SL_BUS_CLOCK_USART0);

  // Default asynchronous initializer (main mode, 1 Mbps, 8-bit data)
  USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;

  // MSB first transmission for SPI compatibility
  init.msbf = true;

  // Configure and enable USART0
  USART_InitSync(USART0, &init);

#ifdef TEST
  USART0->CTRL |= (0x1 << 1);
#endif

  /*
   * Route USART0 RX, TX, and CLK to the specified pins.  Note that CS
   * is not controlled by the USART in this case but as a GPIO under
   * software control in the main loop.
   */

  GPIO->USARTROUTE[0].TXROUTE = (US0MOSI_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
                                | (US0MOSI_PIN <<
                                   _GPIO_USART_TXROUTE_PIN_SHIFT);

  GPIO->USARTROUTE[0].RXROUTE = (US0MISO_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
                                | (US0MISO_PIN <<
                                   _GPIO_USART_RXROUTE_PIN_SHIFT);

  GPIO->USARTROUTE[0].CLKROUTE =
    (US0CLK_PORT << _GPIO_USART_CLKROUTE_PORT_SHIFT)
    | (US0CLK_PIN <<
       _GPIO_USART_CLKROUTE_PIN_SHIFT);

  // Enable USART interface pins
  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN      // MOSI
                                | GPIO_USART_ROUTEEN_TXPEN    // MISO
                                | GPIO_USART_ROUTEEN_CLKPEN;

  if (sl_interrupt_manager_is_irq_disabled(USART0_TX_IRQn)) {
    sl_interrupt_manager_clear_irq_pending(USART0_TX_IRQn);
    sl_interrupt_manager_enable_irq(USART0_TX_IRQn);
  }

  // Enable transmit complete interrupt
  USART_IntEnable(USART0, USART_IEN_TXC);
}

void SPI_Task(void *arg)
{
  (void)&arg;

  int i;
  // Populate outgoing data array
  for (i = 0; i < BUFLEN; i++) {
    outbuf[i] = (uint8_t)i;
  }

  // Wait for button press and disable button interrupt hence it served just for starting the SPI task
  (void) xTaskNotifyWait(0x00, 0x00, NULL, portMAX_DELAY);
  GPIO_IntDisable(BUTTN0_PIN);

  while (1) {
    // Start at the beginning of the buffer
    if (bufpos > BUFLEN - 1) {
      for (i = 0; i < BUFLEN; i++) {
        inbuf[i] = 0;
      }
      bufpos = 0;
    }

    // Assert chip select (drive low)
    GPIO_PinOutClear(US0CS_PORT, US0CS_PIN);

    /*
     * Because this example is most likely going to be running with
     * another EFM32/EFR32 device on the secondary side, it must insert
     * a delay between chip select assertion and sending the first
     * byte.
     *
     * On Series 1 and Series 2 EFM32/EFR32 devices, this delay needs
     * to be between 7 and 10 us in order for the downstream firmware
     * to enable SPI reception and pre-load the first byte to be
     * transmitted.
     *
     * Similar delays are not uncommon for things like high-precision
     * delta-sigma A-to-D converters where the falling chip select
     * wakes the device from a low-power state, starts a conversion,
     * and can return data after some set delay.
     */
    for (i = 0; i < 25; i++) {}

    USART0->TXDATA = outbuf[bufpos];

    // Waiting for TX termination
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    inbuf[bufpos++] = USART0->RXDATA;

    // De-assert chip select upon transfer completion (drive high)
    GPIO_PinOutSet(US0CS_PORT, US0CS_PIN);

    /*
     * Insert a short delay after CS de-assertion.  When this code is
     * running on a faster main (e.g. either higher clock rate or a
     * faster CPU such as a Cortex-M3), the secondary implementation
     * running on a slower device (e.g. either lower clock rate or a
     * slower CPU such as a Cortex-M0+) needs extra time to prepare
     * the input and output buffers for the next round of bytes to be
     * transferred.
     */

    vTaskDelay(1);
  }
}

void button_cb(uint8_t intno, void *context)
{
  (void)&context;

  if (intno == 3) {
    xTaskNotify(xHandle, 0, eNoAction);
  }
}

void USART0_TX_IRQHandler(void)
{
  portBASE_TYPE xHigherPTWoken = 0;

  /*
   * Once TX terminated notify the task about the TX termination fact so it can read the RX buffer.
   */

  xSemaphoreGiveFromISR(xSemaphore, &xHigherPTWoken);

  portYIELD_FROM_ISR(xHigherPTWoken);

  // Clear the requesting interrupt before exiting the handler
  USART_IntClear(USART0, USART_IF_TXC);
}
