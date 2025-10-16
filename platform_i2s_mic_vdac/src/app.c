/***************************************************************************//**
 * @file
 * @brief Platform - I2S Microphone VDAC example
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#include "app.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "dmadrv.h"
#include "em_vdac.h"
#include "stddef.h"

/********************************  DEFINES  ***********************************/
#define BUFFER_SIZE               1024

/***************************  LOCAL VARIABLES   *******************************/
static unsigned int I2S_LEFT_CHANNEL;
static unsigned int I2S_RIGHT_CHANNEL;
static unsigned int VDAC_CHANNEL;

static int16_t leftBuffer1[BUFFER_SIZE];
static int16_t leftBuffer2[BUFFER_SIZE];
static int16_t rightBuffer1[BUFFER_SIZE];
static int16_t rightBuffer2[BUFFER_SIZE];

static uint16_t dacBuffer1[BUFFER_SIZE];
static uint16_t dacBuffer2[BUFFER_SIZE];

static volatile uint8_t leftDescFlg = 0;

/*******************************  CALLBACKS ***********************************/

/*
 * The left channel callback uses the parity of sequenceNo to determine which buffer descriptor flag to set:
 * If sequenceNo is odd, it sets bit 0x01 in the leftDescFlg flag.
 * If sequenceNo is even, it sets bit 0x02 in the leftDescFlg flag.
 */
bool leftDMACallback(unsigned int channel,
                     unsigned int sequenceNo,
                     void *userParam)
{
  (void) channel;
  (void) userParam;
  if (sequenceNo & 1) {
    leftDescFlg |= 0x01;
  } else {
    leftDescFlg |= 0x02;
  }
  return true;
}

bool rightDMACallback(unsigned int channel,
                      unsigned int sequenceNo,
                      void *userParam)
{
  (void) channel;
  (void) sequenceNo;
  (void) userParam;
  return true;
}

bool vdacDMACallback(unsigned int channel,
                     unsigned int sequenceNo,
                     void *userParam)
{
  (void) channel;
  (void) sequenceNo;
  (void) userParam;
  return true;
}

/*******************************  INIT FUNCTIONS ******************************/
void initI2S(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);

  GPIO_PinModeSet(I2S_SD_PORT, I2S_SD_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(I2S_SCK_PORT, I2S_SCK_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(I2S_WS_PORT, I2S_WS_PIN, gpioModePushPull, 1);

  CMU_ClockEnable(cmuClock_USART0, true);

  USART_InitI2s_TypeDef initI2S = USART_INITI2S_DEFAULT;
  initI2S.sync.autoCsEnable = true;
  initI2S.sync.databits = usartDatabits16;
  initI2S.format = usartI2sFormatW32D16;
  initI2S.sync.enable = usartDisable;
  initI2S.sync.autoTx = true;
  initI2S.delay = true;
  initI2S.justify = usartI2sJustifyLeft;
  initI2S.sync.baudrate = BCLK_FREQUENCY_3MHz;
  initI2S.dmaSplit = true;

  USART_InitI2s(USART0, &initI2S);

  USART0->I2SCTRL |= USART_I2SCTRL_DELAY;

  GPIO->USARTROUTE[0].RXROUTE = (I2S_SD_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
                                | (I2S_SD_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].CLKROUTE = (I2S_SCK_PORT
                                  << _GPIO_USART_CLKROUTE_PORT_SHIFT)
                                 | (I2S_SCK_PIN <<
                                    _GPIO_USART_CLKROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].CSROUTE = (I2S_WS_PORT << _GPIO_USART_CSROUTE_PORT_SHIFT)
                                | (I2S_WS_PIN << _GPIO_USART_CSROUTE_PIN_SHIFT);

  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN
                                | GPIO_USART_ROUTEEN_CLKPEN
                                | GPIO_USART_ROUTEEN_CSPEN;

  GPIO_PinModeSet(GPIO_MIC_EN_PORT, GPIO_MIC_EN_PIN, gpioModePushPull, 1);
  USART_Enable(USART0, usartEnable);
}

void initDMA()
{
  DMADRV_Init();

  DMADRV_AllocateChannel(&I2S_LEFT_CHANNEL, NULL);
  DMADRV_AllocateChannel(&I2S_RIGHT_CHANNEL, NULL);
  DMADRV_AllocateChannel(&VDAC_CHANNEL, NULL);

  DMADRV_PeripheralMemoryPingPong(I2S_LEFT_CHANNEL,
                                  dmadrvPeripheralSignal_USART0_RXDATAV,
                                  leftBuffer1,
                                  leftBuffer2,
                                  (void *) &(USART0->RXDOUBLE),
                                  true,
                                  BUFFER_SIZE,
                                  dmadrvDataSize2,
                                  leftDMACallback,
                                  NULL);

  DMADRV_PeripheralMemoryPingPong(I2S_RIGHT_CHANNEL,
                                  dmadrvPeripheralSignal_USART0_RXDATAVRIGHT,
                                  rightBuffer1,
                                  rightBuffer2,
                                  (void *) &(USART0->RXDOUBLE),
                                  true,
                                  BUFFER_SIZE,
                                  dmadrvDataSize2,
                                  rightDMACallback,
                                  NULL);

  DMADRV_MemoryPeripheralPingPong(VDAC_CHANNEL,
                                  dmadrvPeripheralSignal_USART0_RXDATAV,
                                  (void *) &(VDAC0->CH1F),
                                  dacBuffer1,
                                  dacBuffer2,
                                  true,
                                  BUFFER_SIZE,
                                  dmadrvDataSize2,
                                  vdacDMACallback,
                                  NULL);
}

void initVDAC(void)
{
  CMU_ClockEnable(cmuClock_VDAC0, true);

  VDAC_Init_TypeDef vdacInit = VDAC_INIT_DEFAULT;
  VDAC_InitChannel_TypeDef vdacChInit = VDAC_INITCHANNEL_DEFAULT;

  vdacInit.reference = vdacRefAvdd;
  vdacInit.prescaler = VDAC_PrescaleCalc(VDAC0, CLK_VDAC_FREQ);
  vdacInit.biasKeepWarm = true;

  vdacChInit.enable = true;
  vdacChInit.warmupKeepOn = true;
  vdacChInit.port = VDAC_OUTPUT_PORT;
  vdacChInit.pin = VDAC_OUTPUT_PIN;
  GPIO->VDAC_OUTPUT_0_BUS |= VDAC_OUTPUT_0_BUSALLOC;
  vdacChInit.mainOutEnable = false;
  vdacChInit.auxOutEnable = true;

  VDAC_Init(VDAC0, &vdacInit);
  VDAC_IntClear(VDAC0, 0xFFFFFFFFu);
  VDAC_InitChannel(VDAC0, &vdacChInit, CHANNEL_NUM);
  VDAC_Enable(VDAC0, CHANNEL_NUM, true);
}

static inline void map_buffer_to_dac12(const int16_t *src,
                                       uint16_t *dst,
                                       size_t n)
{
  for (size_t i = 0; i < n; i++) {
    dst[i] = (uint16_t) (((((int32_t) src[i]) + 32767) >> 4) & 0x0FFF);
  }
}

/******************************* APP INIT  ************************************/
void app_init(void)
{
  // Initialize peripherals
  initVDAC();
  initI2S();
  initDMA();
}

/****************************** APP PROCESS ***********************************/
void app_process_action(void)
{
  uint8_t _leftDescFlg = 0;

  // The value of the leftDescFlg flag is safely transferred to the _leftDescFlg variable while ensuring atomic access within a critical section.
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  _leftDescFlg = leftDescFlg;
  leftDescFlg = 0;
  CORE_EXIT_CRITICAL();

  // The value of _leftDescFlg determines which left audio buffer should be mapped to the VDAC channel.
  if ((_leftDescFlg  & 0x01)) {
    map_buffer_to_dac12(leftBuffer1, dacBuffer1, BUFFER_SIZE);
  }
  if ((_leftDescFlg  & 0x02)) {
    map_buffer_to_dac12(leftBuffer2, dacBuffer2, BUFFER_SIZE);
  }
}
