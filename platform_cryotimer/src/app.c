/***************************************************************************//**
 * @file app.c
 * @version 1.0.0
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
#include "em_core.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "em_cryotimer.h"

/*
 * Toggles PA11 (LCD EXTCOMIN) at approximately 1s interval.
 * Drops to EM3 after setup.  Consumes 10 uA, with 100 uA pulses
 */

void setup_cryotimer(void (*callback)(void));

void cryo_callback(void)
{
  GPIO->P[0].DOUTTGL = 1 << 11; /* toggle PA11 */
}

static void (*cryo_irq_callback)(void);

void setup_cryotimer(void (*callback)(void))
{
  cryo_irq_callback = callback;
  CMU_ClockEnable(cmuClock_CRYOTIMER, true);
  CRYOTIMER_Init_TypeDef init = CRYOTIMER_INIT_DEFAULT;
  init.enable = 1;
  init.debugRun = 0;
  init.em4Wakeup = 0;
  init.osc = cryotimerOscULFRCO; // Use the 1 kHz ULFRCO
  init.presc = cryotimerPresc_1; // No prescaling, it is slow enough
  init.period = 10;              // 1 kHz/1024 = 977 mHz
  CRYOTIMER_Init(&init);

  // Enable cryotimer interrupts
  CRYOTIMER_IntEnable(CRYOTIMER_IF_PERIOD);
  NVIC_EnableIRQ(CRYOTIMER_IRQn);
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  /* Chip errata */
  CHIP_Init();
  CMU_ClockEnable(cmuClock_GPIO, 1);
  GPIO_PinModeSet(gpioPortA, 11, gpioModePushPull, 0);
  setup_cryotimer(cryo_callback);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  EMU_EnterEM3(0);
}

void CRYOTIMER_IRQHandler(void)
{
  uint32_t flags = CRYOTIMER_IntGet();
  CRYOTIMER_IntClear(flags);
  cryo_irq_callback();
}
