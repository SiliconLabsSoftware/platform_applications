/***************************************************************************/ /**
 * @file app.c
 * @brief TRNG baremetal example
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_vdac.h"
#include "math.h"
#include "sl_gpio.h"
#include "sl_simple_button_instances.h"

#define DAC_CHANNEL_NUM 0

#define SAMPLE_RATE     100000
#if SAMPLE_RATE > 100000
#  error "Cannot set sample rate higher than 100Ksps"
#endif
#define OUTPUT_FREQ_MIN 1000
#define OUTPUT_FREQ_MAX (SAMPLE_RATE / 10)
#define DDS_FREQ_STEP   ((OUTPUT_FREQ_MAX - OUTPUT_FREQ_MIN) / 10)

#define DAC_OUTPUT_PORT vdacChPortB
#define DAC_OUTPUT_PIN  0

enum DDS_WAVE_FORM { DDS_SINE, DDS_SQUARE, DDS_TRIANGLE };

static uint8_t LUT[] = {
  128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170,
  173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211,
  213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240,
  241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254,
  254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251,
  250, 250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232,
  230, 228, 226, 224, 222, 220, 218, 215, 213, 211, 208, 206, 203, 201, 198,
  196, 193, 190, 188, 185, 182, 179, 176, 173, 170, 167, 165, 162, 158, 155,
  152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 109,
  106, 103, 100, 97, 93, 90, 88, 85, 82, 79, 76, 73, 70, 67, 65,
  62, 59, 57, 54, 52, 49, 47, 44, 42, 40, 37, 35, 33, 31, 29,
  27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11, 10, 9, 7, 6,
  5, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11,
  12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35, 37,
  40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,
  79, 82, 85, 88, 90, 93, 97, 100, 103, 106, 109, 112, 115, 118, 121,
  124
};

static volatile uint32_t output_freq =
  OUTPUT_FREQ_MIN; // target frequency to generate in Hz
static uint32_t                    ph_acc = 0;
static volatile uint32_t           tuning_word = 0;
static volatile enum DDS_WAVE_FORM wave_form = DDS_SINE;

void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    if (handle == &sl_button_btn0) {
      ph_acc = 0;
      output_freq += DDS_FREQ_STEP;
      if (output_freq > OUTPUT_FREQ_MAX) {
        output_freq = OUTPUT_FREQ_MIN;
      }
      tuning_word = (uint32_t) (pow(2, 32) / SAMPLE_RATE * output_freq);
    }

    if (handle == &sl_button_btn1) {
      wave_form++;
      if (wave_form > DDS_TRIANGLE) {
        wave_form = DDS_SINE;
      }
    }
  }
}

void init_vdac(void)
{
  // Use default settings
  VDAC_Init_TypeDef        init = VDAC_INIT_DEFAULT;
  VDAC_InitChannel_TypeDef init_channel = VDAC_INITCHANNEL_DEFAULT;

  sl_gpio_set_pin_mode(&pb0, SL_GPIO_MODE_DISABLED, 1);
  CMU_ClockSelectSet(cmuClock_VDAC0, cmuSelect_HFRCOEM23);
  CMU_ClockEnable(cmuClock_HFRCOEM23, true);
  CMU_ClockEnable(cmuClock_VDAC0, true);

  init.prescaler = VDAC_PrescaleCalc(VDAC0, (uint32_t) (SAMPLE_RATE * 2));

  init.onDemandClk = false;

  // Set reference to internal 1.25V reference
  init.reference = vdacRefAvdd;

  // Disable High Capacitance Load mode
  init_channel.highCapLoadEnable = false;

  // Initialize the VDAC and VDAC channel
  VDAC_Init(VDAC0, &init);
  VDAC_InitChannel(VDAC0, &init_channel, DAC_CHANNEL_NUM);

  // Enable the VDAC
  VDAC_Enable(VDAC0, DAC_CHANNEL_NUM, true);

  VDAC_ChannelOutputSet(VDAC0, DAC_CHANNEL_NUM, 0);
}

void app_init(void)
{
  init_vdac();

  tuning_word = (uint32_t) (pow(2, 32) / SAMPLE_RATE * output_freq);
  sl_gpio_set_pin_mode(&pc8, SL_GPIO_MODE_PUSH_PULL, 0);
}

/***************************************************************************/ /**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (!(VDAC_GetStatus(VDAC0) & VDAC_STATUS_CH0FIFOFULL)) {
    uint32_t out_val;
    uint8_t  count = (ph_acc >> 24);
    ph_acc += tuning_word;
    switch (wave_form) {
      default:
      case DDS_SINE:
        out_val = LUT[count] << 4;
        break;
      case DDS_SQUARE:
        out_val = (count > 127) ? 4095 : 0;
        break;
      case DDS_TRIANGLE:
        out_val = count * 4095 / 255;
        break;
    }
    VDAC_ChannelOutputSet(VDAC0, DAC_CHANNEL_NUM, out_val);
    sl_gpio_toggle_pin(&pc8);
  }
}
