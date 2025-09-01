/***************************************************************************//**
 * @file app.c
 * @brief Main application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_timer.h"

// Can be changed depending on the application
#define GPIO_INPUT_PORT      gpioPortC
#define GPIO_INPUT_PIN       0
#define GATE_TIME_INTERVAL   10
#define TOP_COUNTER_VALUE    CMU_ClockFreqGet(cmuClock_EM01GRPACLK) \
  * GATE_TIME_INTERVAL

static volatile bool measurement_ready = false;
static volatile uint32_t edge_counts = 0;
static volatile bool first_starting_edge = false;
static volatile uint32_t estimated_frequency = 0;

/**************************************************************************//**
 * @brief
 *    CMU initialization
 *****************************************************************************/
void initCMU(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
}

/**************************************************************************//**
 * @brief
 *    GPIO initialization
 *****************************************************************************/
void initGPIO(void)
{
  GPIO_PinModeSet(GPIO_INPUT_PORT, GPIO_INPUT_PIN, gpioModeInputPull, 0);

  GPIO_ExtIntConfig(GPIO_INPUT_PORT,
                    GPIO_INPUT_PIN,
                    GPIO_INPUT_PIN,
                    true,
                    false,
                    true);

  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

/**************************************************************************//**
 * @brief
 *    TIMER initialization
 *****************************************************************************/
void initTIMER0(void)
{
  // Initialize the timer
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;

  TIMER_Init(TIMER0, &timerInit);

  TIMER_TopSet(TIMER0, TOP_COUNTER_VALUE);

  // Enable TIMER0 interrupts
  TIMER_IntClear(TIMER0, _TIMER_IF_MASK);
  TIMER_IntEnable(TIMER0, TIMER_IEN_OF);
  NVIC_EnableIRQ(TIMER0_IRQn);

  // Enable the TIMER
  TIMER_Enable(TIMER0, true);
  first_starting_edge = true;
}

void TIMER0_IRQHandler(void)
{
  // Get the interrupt flags
  uint32_t flags = TIMER_IntGet(TIMER0);
  TIMER_IntClear(TIMER0, flags);

  measurement_ready = true;
}

void GPIO_EVEN_IRQHandler(void)
{
  // Get and clear all pending GPIO interrupts
  uint32_t interruptMask = GPIO_IntGet();
  GPIO_IntClear(interruptMask);

  if (first_starting_edge) {
    first_starting_edge = false;
    edge_counts = 1; // First starting edge, therefore edge_counts starts at 1
    TIMER_CounterSet(TIMER0, 1);
    return;
  }

  edge_counts++;
}

void app_init(void)
{
  initCMU();
  initGPIO();
  initTIMER0();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (measurement_ready) {
    estimated_frequency = edge_counts / GATE_TIME_INTERVAL;
    measurement_ready = false;
    first_starting_edge = true;
  }
}
