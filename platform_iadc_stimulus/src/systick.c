/***************************************************************************//**
 * @file systick.c
 * @brief SysTick driver implementation.
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
#include "systick.h"

void SysTick_Start(void)
{
  //
  // Enable SysTick.
  //
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Stop(void)
{
  //
  // Disable SysTick.
  //
  SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
}

void SysTick_IntEnable(void)
{
  //
  // Enable the SysTick interrupt.
  //
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void SysTick_IntDisable(void)
{
  //
  // Disable the SysTick interrupt.
  //
  SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);
}

void SysTick_SetPeriod(uint32_t period)
{
  //
  // Check the arguments.
  //
  EFM_ASSERT((period > 0) && (period <= 16777216));

  //
  // Set the period of the SysTick counter.
  //
  SysTick->LOAD = period - 1;
}

uint32_t SysTick_GetPeriod(void)
{
  //
  // Return the period of the SysTick counter.
  //
  return (SysTick->LOAD + 1);
}

uint32_t SysTick_GetValue(void)
{
  //
  // Return the current value of the SysTick counter.
  //
  return (SysTick->VAL);
}

void SysTick_SetValue(uint32_t val)
{
  //
  // Check the argument
  //
  EFM_ASSERT((val > 0) && (val <= 16777216));

  //
  // Set the value of the SysTick counter.
  //
  SysTick->VAL = val;
}
