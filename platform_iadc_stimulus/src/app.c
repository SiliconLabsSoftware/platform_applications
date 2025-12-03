/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
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
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_vdac.h"

#include "systick.h"

// EXP header pin 3 (PC9)
#define PULSE_OUT_PORT  gpioPortC
#define PULSE_OUT_PIN   9
#define PULSE_CNT       256

// EXP header pin 16 (PC10)
#define VDAC_APORT_OUT  VDAC_OPA_OUT_APORTOUTSEL_APORT2YCH10

// Counts 1 ms SysTicks
static volatile uint32_t msTicks = 0;
uint32_t pcount;

void SysTick_Handler(void)
{
  msTicks++;
}

static void delay(uint32_t dlyTicks)
{
  uint32_t curTicks = msTicks;

  SysTick_SetValue(0);

  SysTick_IntEnable();

  while ((msTicks - curTicks) < dlyTicks) {
    // Do nothing until the delay time has elapsed
  }

  SysTick_IntDisable();
}

void vdacInit(void)
{
  CMU_ClockEnable(cmuClock_VDAC0, true);

  // Modify default initialization
  VDAC_Init_TypeDef init = VDAC_INIT_DEFAULT;

  // Alternate output calibration because APORT to be used
  init.mainCalibration = false;

  // Use the 12 MHz captive VDAC oscillator for the converter clock
  init.asyncClockMode = true;

  // Determine the prescaler needed for the VDAC clock to be 1 MHz
  init.prescaler = VDAC_PrescaleCalc(1000000, false, 0);

  VDAC_Init(VDAC0, &init);

  // Disable unused main VDAC output
  VDAC0->OPA[0].OUT &= ~(VDAC_OPA_OUT_MAINOUTEN);

  // Use APORT2Y output channel 10 (PC10)
  VDAC0->OPA[0].OUT |= VDAC_APORT_OUT;

  // Zero settle time for maximum update rate;
  VDAC0->OPA[0].TIMER &= ~(_VDAC_OPA_TIMER_SETTLETIME_MASK);

  // Default channel initialization
  VDAC_InitChannel_TypeDef initChannel = VDAC_INITCHANNEL_DEFAULT;

  VDAC_InitChannel(VDAC0, &initChannel, 0);

  // Enable APORT output
  VDAC0->OPA[0].OUT |= VDAC_OPA_OUT_APORTOUTEN;

  // Enable the VDAC
  VDAC_Enable(VDAC0, 0, true);
}

void gpioInit(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Use button 0 as input; configure for rising edge interrupt
  GPIO_PinModeSet(gpioPortB, 0, gpioModeInputPull, 1);

  GPIO_ExtIntConfig(gpioPortB, 0, 0, true, false, true);
  GPIO_IntClear(1 << 0);

  // Enable appropriate odd/even interrupt source depending on the pin number
#if (BSP_GPIO_PB0_PIN & 1)
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
#else
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
#endif // (BSP_GPIO_PB0_PIN & 1)

  // Use PC9 (EXP pin 3) as level output for target pulse
  GPIO_PinModeSet(PULSE_OUT_PORT, PULSE_OUT_PIN, gpioModePushPull, 0);
}

void app_init(void)
{
  CHIP_Init();

  // Init DCDC regulator with kit specific parameters
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  EMU_DCDCInit(&dcdcInit);

  vdacInit();

  // Set VDAC output to 50% (0.625V with VREF = 1.25V)
  VDAC_ChannelOutputSet(VDAC0, 0, 0x800);

  // Setup SysTick for 1 msec interval
  SysTick_Start();
  SysTick_SetPeriod(CMU_ClockFreqGet(cmuClock_CORE) / 1000);

  gpioInit();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  EMU_EnterEM1();

  // Disable button 0 interrupts
  GPIO_IntDisable(1 << 0);

  for (pcount = 0; pcount < PULSE_CNT; pcount++)
  {
    // Pulse output for 1 ms
    GPIO_PinOutSet(PULSE_OUT_PORT, PULSE_OUT_PIN);
    delay(1);
    GPIO_PinOutClear(PULSE_OUT_PORT, PULSE_OUT_PIN);
    delay(9);
  }

  // Re-enable button 0 interrupts
  GPIO_IntEnable(1 << 0);
}
