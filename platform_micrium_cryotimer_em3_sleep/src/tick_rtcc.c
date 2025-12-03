/***************************************************************************//**
 * @file tick_rtcc.c
 * @brief Dynamic Tick using cryotimer
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
#include "tick_rtcc.h"

#if (OS_CFG_TICK_EN == DEF_ENABLED)

#define  CRYO_OSCILLATOR_FREQ                     (1024u)
#define  CRYO_PRESCALER                           (1u)
#define  OS_CRYO_TICK_RATE_HZ                     (CRYO_OSCILLATOR_FREQ \
                                                   / CRYO_PRESCALER)
#define  OS_CRYOTICK_TO_OSTICK(rtcctick)      (((rtcctick)           \
                                                * OSCfg_TickRate_Hz) \
                                               / OS_RTCC_TICK_RATE_HZ)
#define  OS_OSTICK_TO_CRYOTICK(ostick)        (((ostick)                \
                                                * OS_CRYO_TICK_RATE_HZ) \
                                               / OSCfg_TickRate_Hz)

// The number of OS ticks that have passed at the last time
// we updated the OS time. This is stored in OS Tick units
static OS_TICK OS_LastTick;
static OS_TICK OSTickCtrPend = 0;

static void CRYO_UpdateTicks(uint32_t tickElapsed);

/***************************************************************************//**
 *
 * @brief Initialize the RTCC module to enable dynamic ticking.
 *
 * @note This function is called at the start of the Application Start Task,
 *       and so other tasks with more priority could be executed before
 *       this Initialization. Therefore, this function can be use for a lazy
 *       Initialization by being called when a RTC timer is needed.
 ******************************************************************************/
void RTCC_TickInit(void)
{
  EMU_EM23Init_TypeDef em23Init;
  CRYOTIMER_Init_TypeDef cryoInit;

  CMU_ClockEnable(cmuClock_CRYOTIMER, true);        // Initialize the cryotimer to wake the device from EM3.

  cryoInit.enable = false;                          // Initialize the cryo timer
  cryoInit.debugRun = false;
  cryoInit.em4Wakeup = false;
  cryoInit.osc = cryotimerOscULFRCO;
  cryoInit.presc = cryotimerPresc_1;

  CRYOTIMER_Init(&cryoInit);
  CRYOTIMER_IntEnable(CRYOTIMER_IEN_PERIOD);       // Enable cryotimer interrupts
  NVIC_EnableIRQ(CRYOTIMER_IRQn);

  em23Init.em23VregFullEn = false;
  em23Init.vScaleEM23Voltage = emuVScaleEM23_FastWakeup;
  EMU_EM23Init(&em23Init);
}

/***************************************************************************//**
 *
 * @brief    Get the OS Tick Counter as if it was running continuously.
 *
 * @return   The effective OS Tick Count.
 ******************************************************************************/
OS_TICK OS_TickGet(void)
{
  uint32_t tickElapsed;

  tickElapsed = OS_LastTick + CRYOTIMER_CounterGet();

  return (tickElapsed);
}

/***************************************************************************//**
 *
 * @brief    Set the number of OS Ticks to wait before calling OSTimeDynTick.
 *
 * @param    ticks   Number of OS Ticks to wait.
 *
 * @return   Number of effective OS Ticks until next OSTimeDynTick.
 ******************************************************************************/
OS_TICK OS_TickNextSet(OS_TICK ticks)
{
  OS_TICK tick_rate = 0;
  OS_TICK tick_shift = 0;
  OS_TICK period = 0;
  CPU_INT32U counter;

  if ((ticks != (OS_TICK) -1) && (ticks != 0)) {
    CRYOTIMER_IntDisable(CRYOTIMER_IEN_PERIOD);                 // Disable the period interrupt

    CORE_CRITICAL_SECTION(ticks -= OSTickCtrPend; );

    counter = CRYOTIMER_CounterGet();                           // Check if there are any ticks unaccounted for

    if (counter > 0u) {                                         // If there are ticks unaccoutned for, update the tick counter
      CORE_CRITICAL_SECTION(CRYO_UpdateTicks(counter); );
    }

    CRYOTIMER_Enable(false);                                    // Clear the Cryo Timer config

    for (tick_rate = ticks; tick_rate >= 1; tick_rate--)        // Since we can only interrupt on powers of 2, find the
    {                                                           // next closest power of 2 to delay for.
      if ((tick_rate & (tick_rate - 1)) == 0) {
        tick_shift = tick_rate;
        break;
      }
    }

    while (tick_shift) {                                        // Count the bits, off by one to match the enum
      tick_shift >>= 1;
      if (!tick_shift) {
        break;
      }
      period++;
    }

    CRYOTIMER_PeriodSet(period);                                // Set the new period for the cryo timer
    CRYOTIMER_IntEnable(CRYOTIMER_IEN_PERIOD);                  // Re-enable the period interrupt
    CRYOTIMER_Enable(true);                                     // Clear the Cryo Timer config
  }

  return tick_rate;
}

/***************************************************************************//**
 *
 * @brief Updates the number of pending OS Ticks and the internal time reference.
 *
 ******************************************************************************/
static void CRYO_UpdateTicks(uint32_t tickElapsed)
{
  if (tickElapsed > 0u) {
    OS_LastTick += tickElapsed;                                 // Increment last tick
    CORE_CRITICAL_SECTION(OSTickCtrPend += tickElapsed; );      // Signal the number of ticks elapsed
  }
}

/***************************************************************************//**
 *
 * @brief    Callback for cryo timer.
 *
 ******************************************************************************/
void  CRYOTIMER_IRQHandler(void)
{
  uint32_t period;
  uint32_t tickElapsed;
  uint32_t flags;

  flags = CRYOTIMER_IntGet();      // Acknowledge the interrupt
  CRYOTIMER_IntClear(flags);

  __DSB();                         // Put a barrier here to ensure interrupts are not retriggered.

  period = CRYOTIMER_PeriodGet();  // Get the period for the interrupt
  tickElapsed = (1 << period);

  CRYOTIMER_Enable(false);         // Disable the cryo timer until the next tick set

  CRYO_UpdateTicks(tickElapsed);   // Update dynamic tick count
}

#endif // (OS_CFG_DYN_TICK_EN == DEF_ENABLED)
