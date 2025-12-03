/***************************************************************************//**
 * @file main_task.c
 * @brief Main task implementation
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
#include "os.h"
#include "em_emu.h"
#include "main_task.h"
#include "tick_rtcc.h"

#include "sl_simple_led.h"
#include "sl_simple_led_instances.h"

#define  EX_MAIN_START_TASK_PRIO              21u
#define  EX_MAIN_START_TASK_STK_SIZE          512u

/**********************************USER CODE**********************************/
#define  USER_IDLE_TASK_PRIO                  OS_CFG_PRIO_MAX - 2
#define  USER_IDLE_TASK_STK_SIZE              128u

static  void  Ex_MainStartTask (void  *p_arg);

static  void  UserIdle  (void *p_arg);

// Start Task Stack.
static  CPU_STK  Ex_MainStartTaskStk[EX_MAIN_START_TASK_STK_SIZE];

// Start Task TCB.
static  OS_TCB   Ex_MainStartTaskTCB;

/**********************************USER CODE**********************************/
static  CPU_STK  UserIdleStk[USER_IDLE_TASK_STK_SIZE];

static  OS_TCB   UserIdleTCB;

uint32_t         idleCount;

void main_task_init(void)
{
  RTOS_ERR  err;

  OS_TRACE_INIT();
  OSInit(&err);
  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

  OSTaskCreate(&Ex_MainStartTaskTCB,
               "Ex Main Start Task",
               Ex_MainStartTask,
               DEF_NULL,
               EX_MAIN_START_TASK_PRIO,
               &Ex_MainStartTaskStk[0],
               (EX_MAIN_START_TASK_STK_SIZE / 10u),
               EX_MAIN_START_TASK_STK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);

  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

/**********************************USER CODE**********************************/
  OSTaskCreate(&UserIdleTCB,
               "User Implemented Idle",
               UserIdle,
               DEF_NULL,
               USER_IDLE_TASK_PRIO,
               &UserIdleStk[0],
               (USER_IDLE_TASK_STK_SIZE / 10u),
               USER_IDLE_TASK_STK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR | OS_OPT_TASK_NO_TLS),
               &err);

  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

/********************************END USER CODE********************************/

  OSStart(&err);
  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

static  void  Ex_MainStartTask(void  *p_arg)
{
  RTOS_ERR  err;

  PP_UNUSED_PARAM(p_arg);

  RTCC_TickInit();

#if (OS_CFG_STAT_TASK_EN == DEF_ENABLED)
  OSStatTaskCPUUsageInit(&err); // Initialize CPU Usage.
  // Check error code.
  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE),; );
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
  CPU_IntDisMeasMaxCurReset(); // Initialize interrupts disabled measurement.
#endif

  // Call common module initialization example.
  Common_Init(&err);
  APP_RTOS_ASSERT_CRITICAL(err.Code == RTOS_ERR_NONE,; );

  while (DEF_ON) {
    sl_led_toggle(&sl_led_led0);

    // Delay Start Task execution for 1000 OS Ticks from now
    OSTimeDly(1000, OS_OPT_TIME_DLY, &err);

    // Check error code.
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE),; );
  }
}

static void UserIdle(void *p_arg)
{
  PP_UNUSED_PARAM(p_arg);
  while (DEF_ON) {
    EMU_EnterEM3(true);
  }
}
