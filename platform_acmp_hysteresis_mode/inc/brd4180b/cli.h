/***************************************************************************//**
 * @file
 * @brief ACMP hysteresis mode examples functions
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

#ifndef CLI_H
#define CLI_H

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/* Input types */
#define GPIO_PIN            1
#define REF1V25             2
#define REF2V5              3

/* Choose one for ACMP POS input */
#define ACMP_POS_INPUT_TYPE GPIO_PIN
// #define ACMP_POS_INPUT_TYPE REF1V25
// #define ACMP_POS_INPUT_TYPE REF2V5

/* Choose one for ACMP NEG input */
#define ACMP_NEG_INPUT_TYPE GPIO_PIN
// #define ACMP_NEG_INPUT_TYPE REF1V25
// #define ACMP_NEG_INPUT_TYPE REF2V5

// #define ACMP_TO_GPIO // ACMP output to GPIO directly

/* The following are configured for BRD4180B */

// ACMP POS input
#if ACMP_POS_INPUT_TYPE == GPIO_PIN
#define ACMP_POS_INPUT      acmpInputPB0 // WSTK EXP header pin 7
#else
#if ACMP_POS_INPUT_TYPE == REF1V25
#define ACMP_POS_INPUT      acmpInputVREFDIV1V25 // Divided 1V25 reference
#elif ACMP_POS_INPUT_TYPE == REF2V5
#define ACMP_POS_INPUT      acmpInputVREFDIV2V5 // Divided 2V5 reference
#endif
#endif

// ACMP NEG input
#if ACMP_NEG_INPUT_TYPE == GPIO_PIN
#define ACMP_NEG_INPUT      acmpInputPB1 // WSTK EXP header pin 9
#else
#if ACMP_NEG_INPUT_TYPE == REF1V25
#define ACMP_NEG_INPUT      acmpInputVREFDIV1V25 // Divided 1V25 reference
#elif ACMP_NEG_INPUT_TYPE == REF2V5
#define ACMP_NEG_INPUT      acmpInputVREFDIV2V5 // Divided 2V5 reference
#endif
#endif

// GPIO BUS allocation
#define GPIOBUS             BBUSALLOC

// POS input GPIO bus
#if ACMP_POS_INPUT_TYPE == GPIO_PIN // Using even GPIO pin
#define ACMP_POS_BUS_ALLOC  GPIO_BBUSALLOC_BEVEN0_ACMP0
#else
#define ACMP_POS_BUS_ALLOC  0 // Using internal reference
#endif

// NEG input GPIO bus
#if ACMP_NEG_INPUT_TYPE == GPIO_PIN  // Using odd GPIO pin
#define ACMP_NEG_BUS_ALLOC  GPIO_BBUSALLOC_BODD0_ACMP0
#else
#define ACMP_NEG_BUS_ALLOC  0 // Using internal reference
#endif

// LED0 pin definition
#define LED0_PORT           SL_GPIO_PORT_A
#define LED0_PIN            4
#define LED0_WIRING         // Short F8 with F10

/* Voltage divisions for internal reference: 0 to 63. */

#define ACMP_VREF_DIV       63

/***************************************************************************//**
 * Initialize example
 ******************************************************************************/
void cli_app_init(void);

/***************************************************************************//**
 * Ticking function
 ******************************************************************************/
void cli_app_process_action();

#endif // CLI_H
