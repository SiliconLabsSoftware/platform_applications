/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_core.h"
#include "sl_gpio.h"
#include "sl_hal_gpio.h"

#define BSP_GPIO_PB0_PORT   gpioPortA
#define BSP_GPIO_PB0_PIN    5

#define BSP_GPIO_PB1_PORT   gpioPortB
#define BSP_GPIO_PB1_PIN    4

#define BSP_GPIO_LED0_PORT  gpioPortC
#define BSP_GPIO_LED0_PIN   8

#define BSP_GPIO_LED1_PORT  gpioPortC
#define BSP_GPIO_LED1_PIN   9

extern void Default_Handler(void);
void RAM_Default_Handler(void);
void ram_interrupt_vector_update(void);
void gpio_setup(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  ram_interrupt_vector_update();
  gpio_setup();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}

/**************************************************************************//**
 * @brief
 *   Setup GPIO for pushbuttons and LEDs
 *****************************************************************************/
void gpio_interrupt_callback(uint8_t interrupt_number, void *context)
{
  (void)interrupt_number;
  (void)context;
  GPIO_PinOutToggle(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
}

void gpio_setup(void)
{
  // Configure GPIO Clock.
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Configure Button PB0 as input and enable interrupt
  sl_gpio_t gpio_pin = {
    .port = BSP_GPIO_PB0_PORT,
    .pin = BSP_GPIO_PB0_PIN
  };

  // Enable interrupt on BSP_GPIO_PB0_PIN
  sl_hal_gpio_set_pin_mode(&gpio_pin, gpioModeInputPull, 1);

  int32_t int_no = SL_GPIO_INTERRUPT_UNAVAILABLE;  // Let the function assign an interrupt number

  sl_gpio_configure_external_interrupt(
    &gpio_pin,                                // GPIO structure with port and pin
    &int_no,                                  // Pointer to interrupt number (input/output)
    SL_GPIO_INTERRUPT_FALLING_EDGE,            // Trigger on rising edge
    gpio_interrupt_callback,                  // Callback function
    NULL                                      // Context (optional)
    );

  // Configure LED0 and LED1 as a push pull output
  GPIO_PinModeSet(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN, gpioModePushPull, 0);
}

/***************************************************************************//**
 * @brief
 *   Place Default Handler for Exceptions / Interrupts in RAM
 ******************************************************************************/
void RAM_Default_Handler(void)
{
  GPIO_PinOutSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
  while (true) {
  }
}

/***************************************************************************//**
 * @brief
 *   Configures the vector table in RAM to point to the new Default handler
 *   address that is located in RAM.
 *   The new Default handler can be used to catch exceptions
 *   and external interrupts which are not handled by specific ISRs.
 ******************************************************************************/
void ram_interrupt_vector_update(void)
{
  sl_interrupt_manager_irq_handler_t *current;
  current = (sl_interrupt_manager_irq_handler_t *)SCB->VTOR;

  for (uint32_t i = 0; i < (16 + EXT_IRQ_COUNT); i++) {
    // Overwrite target entries.
    if (current[i] == Default_Handler) {
      sl_interrupt_manager_set_irq_handler(i, RAM_Default_Handler);
    }
  }
}
