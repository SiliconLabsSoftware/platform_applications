/***************************************************************************//**
 * @file app.h
 * @brief Top level application functions
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

#ifndef APP_H
#define APP_H

#define I2S_SCK_PORT              gpioPortD
#define I2S_SCK_PIN               3

#define I2S_SD_PORT               gpioPortD
#define I2S_SD_PIN                4

#define I2S_WS_PORT               gpioPortD
#define I2S_WS_PIN                5

#define GPIO_MIC_EN_PORT          gpioPortC
#define GPIO_MIC_EN_PIN           8

#define LRCLK_FREQUENCY_48kHz     48000
#define BCLK_FREQUENCY_3MHz       64 * LRCLK_FREQUENCY_48kHz

#define CHANNEL_NUM               1
#define CLK_VDAC_FREQ             100000

// EXP12
#define VDAC_OUTPUT_PORT          vdacChPortA
#define VDAC_OUTPUT_PIN           5
#define VDAC_OUTPUT_0_BUS         ABUSALLOC
#define VDAC_OUTPUT_0_BUSALLOC    GPIO_ABUSALLOC_AODD1_VDAC0CH1

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void);

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void);

#endif // APP_H
