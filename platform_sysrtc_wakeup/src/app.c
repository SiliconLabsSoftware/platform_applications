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

/***************************************************************************//**
 * Inclusions.
 ******************************************************************************/
#include "peripheral_sysrtc.h"
#include "peripheral_sysrtc_compat.h"
#include "app.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "sl_simple_led_led0_config.h"
#include "em_emu.h"
#include "sl_enum.h"
#include "em_prs.h"

/***************************************************************************//**
 * Local definitons.
 ******************************************************************************/
#define SYSRTC_GRP_IF_COMP0 (1 << 1)

/***************************************************************************//**
 * Local variables
 ******************************************************************************/
uint32_t period_in_ticks = 32768;   // 1 sec is equal to  32768 ticks LFRCO.
static uint8_t  sysrtc_group_number = 0;
static uint8_t  sysrtc_compare_channel = 0;

/***************************************************************************//**
 * Initialize GPIO peripheral.
 ******************************************************************************/
void initGPIO(void)
{
  // Enable the clock of the CMU peripheral.
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Set the LED GPIO to push-pull output.
  GPIO_PinModeSet(SL_SIMPLE_LED_LED0_PORT,
                  SL_SIMPLE_LED_LED0_PIN,
                  gpioModePushPull,
                  0);
}

/***************************************************************************//**
 * Initialize SYSRTC peripheral.
 ******************************************************************************/
void initSYSRTC(void)
{
  sl_sysrtc_config_t sysrtc_config = SYSRTC_CONFIG_DEFAULT;

  sysrtc_config.enable_debug_run = true;

  // Enable the clock of the SYSRTC peripheral.
  CMU_ClockEnable(cmuClock_SYSRTC, true);

  // Initialization of SYSRTC
  sl_sysrtc_init(&sysrtc_config);

  // Set the SYSRTC compare event to toggle action.
  const sl_hal_sysrtc_group_channel_compare_config_t compare_config_channel0 = {
    .compare_match_out_action = SL_HAL_SYSRTC_COMPARE_MATCH_OUT_ACTION_TOGGLE
  };

  sl_sysrtc_group_config_t sysrtc_group_config = SYSRTC_GROUP_CONFIG_DEFAULT;

  sysrtc_group_config.p_compare_channel0_config = &compare_config_channel0;

  // Initialization of the group settings with the toggle channel settings.
  sl_sysrtc_init_group(0u, &sysrtc_group_config);

  // Initialization the compare value with the first period value.
  sl_sysrtc_set_group_compare_channel_value(sysrtc_group_number,
                                            sysrtc_compare_channel,
                                            period_in_ticks);
  // Compare 0 interrupt enable.
  sl_sysrtc_enable_group_interrupts(0u, SYSRTC_GRP0_IEN_CMP0);

  // Enable SYSRTC
  sl_sysrtc_enable();

  // Clear SYSRTC interrupts.
  NVIC_ClearPendingIRQ(SYSRTC_APP_IRQn);

  // Enable SYSRTC interrupts.
  NVIC_EnableIRQ(SYSRTC_APP_IRQn);

  // Starting the SYSRTC
  sl_sysrtc_start();
}

/***************************************************************************//**
 * Initialize PRS peripheral.
 ******************************************************************************/
void initPRS(void)
{
  // Enable the clock of the GPIO peripheral.
  CMU_ClockEnable(cmuClock_PRS, true);

  // Set the source and asynchronous signal for the PRS channel.
  PRS_SourceAsyncSignalSet(
    PRS_CH,
    PRS_ASYNC_CH_CTRL_SOURCESEL_SYSRTC0,
    0);

  // Set the GPIO output for the PRS asynchronous channel, route to LED.
  PRS_PinOutput(PRS_CH,
                prsTypeAsync,
                SL_SIMPLE_LED_LED0_PORT,
                SL_SIMPLE_LED_LED0_PIN);
}

/***************************************************************************//**
 * Initialize the application.
 ******************************************************************************/
void app_init(void)
{
  // Initializing the peripherals.
  initGPIO();
  initSYSRTC();
  initPRS();
}

/***************************************************************************//**
 * SYSRTC compare event interrupt handler.
 ******************************************************************************/
void SYSRTC_APP_IRQHandler(void)
{
  // Clear SYSRTC interrupts.
  sl_sysrtc_clear_group_interrupts(0, SYSRTC_GRP_IF_COMP0);

  // Update the value of a compare variable.
  uint32_t next_compare = sl_sysrtc_get_counter() +  period_in_ticks;
  // Set the compare value for the next compare event blink event.
  sl_sysrtc_set_group_compare_channel_value(sysrtc_group_number,
                                            sysrtc_compare_channel,
                                            next_compare);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  // Send the device to EM3.
  EMU_EnterEM3(true);
}
