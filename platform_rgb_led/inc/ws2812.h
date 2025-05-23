/***************************************************************************//**
 * @file ws2812.h
 * @brief Header file for ws2812.c
 * @version v1.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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
 * # Experimental Quality
 * This code has not been formally tested and is provided as-is. It is not
 * suitable for production environments. In addition, this code will not be
 * maintained and there may be no bug maintenance planned for these resources.
 * Silicon Labs may update projects from time to time.
 ******************************************************************************/

#ifndef WS2812_H_
#define WS2812_H_

#include <stdint.h>
#include "em_usart.h"
#include "em_ldma.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "app.h"

#ifdef __cplusplus
extern "C" {
#endif

// NEEDS TO BE USER DEFINED
#ifndef NUMBER_OF_LEDS
#define NUMBER_OF_LEDS         0
#endif

// OPTIONAL USER DEFINES
#ifndef TX_DMA_CHANNEL
#define TX_DMA_CHANNEL         1
#endif

#ifndef LDMA_PERIPHERAL_SIGNAL
#define LDMA_PERIPHERAL_SIGNAL ldmaPeripheralSignal_USART0_TXBL
#endif

#ifndef USART_PERIPHERAL
#define USART_PERIPHERAL       USART0
#endif

#ifndef USART_CMU_CLK
#define USART_CMU_CLK          cmuClock_USART0
#endif

#ifndef USART_TX_LOCATION
#define USART_TX_LOCATION      USART_ROUTELOC0_TXLOC_LOC0
#endif

#ifndef USART_TX_PORT
#define USART_TX_PORT          gpioPortE
#endif

#ifndef USART_TX_PIN
#define USART_TX_PIN           10
#endif

void set_color_buffer(const uint8_t *input_color_buffer);
void init_ws2812_driver (void);
void init_serial_output (void);
void init_LDMA(void);

#ifdef __cplusplus
}
#endif

#endif /* WS2812_H_ */
