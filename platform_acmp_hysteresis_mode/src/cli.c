/***************************************************************************//**
 * @file
 * @brief ACMP hysteresis mode example functions
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
#include <string.h>
#include <stdio.h>
#include "cli.h"
#include "sl_cli.h"
#include "sl_cli_instances.h"
#include "sl_cli_arguments.h"
#include "sl_cli_handles.h"
#include "sl_board_control.h"
#include "sl_assert.h"
#include "sl_power_manager.h"
#include "sl_sleeptimer.h"
#include "sl_gpio.h"
#include "sl_simple_led_led0_config.h"
#include "em_cmu.h"
#include "em_acmp.h"
#include "em_gpio.h"
#include "em_prs.h"

/*******************************************************************************
 *********************   LOCAL FUNCTION PROTOTYPES   ***************************
 ******************************************************************************/

static void sleep_cmd(const sl_cli_command_arg_t *arguments);
static void hys_cmd(const sl_cli_command_arg_t *arguments);

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/

static const sl_cli_command_info_t cmd__sleep = \
  SL_CLI_COMMAND(sleep_cmd,
                 "Enter sleep mode (CLI will be unresponsive)\r\n",
                 "",
                 { SL_CLI_ARG_END, });

static const sl_cli_command_info_t cmd__hys = \
  SL_CLI_COMMAND(hys_cmd,
                 "Select a hysteresis mode and start the ACMP",
                 "\r\n\t\t\t\t0 Hysteresis disabled\r\n"
                 "\t\t\t\t1 10mV symmetrical hysteresis\r\n"
                 "\t\t\t\t2 20mV symmetrical hysteresis\r\n"
                 "\t\t\t\t3 30mV symmetrical hysteresis\r\n"
                 "\t\t\t\t4 10mV hysteresis on positive edge transitions\r\n"
                 "\t\t\t\t5 20mV hysteresis on positive edge transitions\r\n"
                 "\t\t\t\t6 30mV hysteresis on positive edge transitions\r\n"
                 "\t\t\t\t8 10mV hysteresis on negative edge transitions\r\n"
                 "\t\t\t\t9 20mV hysteresis on negative edge transitions\r\n"
                 "\t\t\t\t10 30mV hysteresis on negative edge transitions\r\n",
                 { SL_CLI_ARG_UINT8, SL_CLI_ARG_END, });

static sl_cli_command_entry_t a_table[] = {
  { "sleep", &cmd__sleep, false },
  { "hys", &cmd__hys, false },
  { NULL, NULL, false },
};

static sl_cli_command_group_t a_group = {
  { NULL },
  false,
  a_table
};

/*******************************************************************************
 *************************  EXPORTED VARIABLES   *******************************
 ******************************************************************************/

sl_cli_command_group_t *command_group = &a_group;

/*******************************************************************************
 ***************************   LOCAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * Callback for hys
 *
 * This function is used as a callback when the hys command is called
 * in the cli. It sets the ACMP hysteresis mode to the provided integer.
 ******************************************************************************/
static void hys_cmd(const sl_cli_command_arg_t *arguments)
{
  static bool started = false;
  if (started) {
    printf("Reset the system to change the ACMP hysteresis mode\r\n");
    return;
  }

  int mode = sl_cli_get_argument_int8(arguments, 0);
  if ((mode < 0) || (mode > 10) || (mode == 7)) {
    printf("Please select a valid mode.\r\n");
    return;
  }
  printf("ACMP hysteresis mode set to %d\r\n", mode);

  started = true;

  // Allocating GPIO buses for ACMP
  GPIO->GPIOBUS = ACMP_NEG_BUS_ALLOC | ACMP_POS_BUS_ALLOC;

  // Enabling the ACMP0 clock
  CMU_ClockEnable(cmuClock_ACMP0, true);

  // Initializing the ACMP
  ACMP_Init_TypeDef acmp_init = ACMP_INIT_DEFAULT;

  // Change additional settings if desired, for example:
#ifdef ACMP_VREF_DIV
  acmp_init.vrefDiv = ACMP_VREF_DIV;
#endif
  acmp_init.hysteresisLevel = mode;
  ACMP_Init(ACMP0, &acmp_init);
  ACMP_ChannelSet(ACMP0, ACMP_NEG_INPUT, ACMP_POS_INPUT);

#ifdef LED0_WIRING // Disable LED0 output to use pins from Ports A/B
  sl_gpio_t gpio;
  gpio.port = SL_SIMPLE_LED_LED0_PORT;
  gpio.pin = SL_SIMPLE_LED_LED0_PIN;
  sl_gpio_set_pin_mode(&gpio,
                       SL_GPIO_MODE_INPUT,
                       0);
  gpio.port = LED0_PORT;
  gpio.pin = LED0_PIN;
  sl_gpio_set_pin_mode(&gpio,
                       SL_GPIO_MODE_PUSH_PULL,
                       0);
#endif

#ifdef ACMP_TO_GPIO // Using GPIO directly
  ACMP_GPIOSetup(ACMP0, LED0_PORT, LED0_PIN, true, false);
#else // Using PRS to route the ACMP output to GPIO
  // Enabling the PRS clock
  CMU_ClockEnable(cmuClock_PRS, true);

  // Using a PRS channel to route the ACMP output to the LED.
  int ch = PRS_GetFreeChannel(prsTypeAsync);
  PRS_SourceAsyncSignalSet(ch, PRS_ASYNC_CH_CTRL_SOURCESEL_ACMP0,
                           PRS_ASYNC_CH_CTRL_SIGSEL_ACMP0OUT);
  PRS_PinOutput(ch, prsTypeAsync, LED0_PORT, LED0_PIN);
#endif
}

/***************************************************************************//**
 * Callback for sleep
 *
 * This function is used as a callback when the sleep command is called
 * in the cli. It puts the system into deep sleep.
 ******************************************************************************/
static void sleep_cmd(const sl_cli_command_arg_t *arguments)
{
  (void)arguments;
  printf(
    "Sleeping... CLI will be unresponsive. Reset the system in order to run a new test.\r\n");
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM2); // Remove this line for EM3
  sl_sleeptimer_delay_millisecond(10);
  sl_board_disable_vcom();
}

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/*******************************************************************************
 * Initialize cli example.
 ******************************************************************************/
void cli_app_init(void)
{
  bool status;

  status = sl_cli_command_add_command_group(sl_cli_inst_handle, command_group);
  EFM_ASSERT(status);

  printf(
    "\r\nACMP Hysteresis Mode Example Started.\r\nType \"help\" for available commands.\r\n\r\n");
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
}

/***************************************************************************//**
 * Ticking function
 ******************************************************************************/
void cli_app_process_action(void)
{
  // Do Nothing
}
