/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_threadx.c
 * @author  MCD Application Team
 * @brief   ThreadX applicative file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2020-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"
#include "tx_api.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmd.h"
#include "rx.h"
#include "tx.h"
#include "tx_port.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RX_TH_STACK_SIZE 512
#define RX_TH_PRIORITY 0
#define RX_TH_PREEMPTION_THRESHOLD                                             \
  (TX_MAX_PRIORITIES - 1) // everyone is allow to do preemption

#define CMD_TH_STACK_SIZE 512
#define CMD_TH_PRIORITY 0
#define CMD_TH_PREEMPTION_THRESHOLD                                            \
  (TX_MAX_PRIORITIES - 1) // everyone is allow to do preemption

#define TX_TH_STACK_SIZE 512
#define TX_TH_PRIORITY 0
#define TX_TH_PREEMPTION_THRESHOLD                                             \
  (TX_MAX_PRIORITIES - 1) // everyone is allow to do preemption
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD rx_th;
TX_THREAD cmd_th;
TX_THREAD tx_th;

static ULONG rx_th_stack[RX_TH_STACK_SIZE / sizeof(ULONG)];
static ULONG cmd_th_stack[CMD_TH_STACK_SIZE / sizeof(ULONG)];
static ULONG tx_th_stack[TX_TH_STACK_SIZE / sizeof(ULONG)];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
 * @brief  Application ThreadX Initialization.
 * @param memory_ptr: memory pointer
 * @retval int
 */
UINT App_ThreadX_Init(VOID *memory_ptr) {
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
  ret = tx_thread_create(&rx_th, "RX Thread", rx_entry_task, 0, rx_th_stack,
                         RX_TH_STACK_SIZE, RX_TH_PRIORITY,
                         RX_TH_PREEMPTION_THRESHOLD, TX_NO_TIME_SLICE,
                         TX_AUTO_START);
  ret = tx_thread_create(&cmd_th, "CMD Thread", cmd_entry_task, 0, cmd_th_stack,
                         CMD_TH_STACK_SIZE, CMD_TH_PRIORITY,
                         CMD_TH_PREEMPTION_THRESHOLD, TX_NO_TIME_SLICE,
                         TX_AUTO_START);
  ret = tx_thread_create(&tx_th, "TX Thread", tx_entry_task, 0, tx_th_stack,
                         TX_TH_STACK_SIZE, TX_TH_PRIORITY,
                         TX_TH_PREEMPTION_THRESHOLD, TX_NO_TIME_SLICE,
                         TX_AUTO_START);
  /* USER CODE BEGIN App_ThreadX_Init */
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
 * @brief  Function that implements the kernel's initialization.
 * @param  None
 * @retval None
 */
void MX_ThreadX_Init(void) {
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
