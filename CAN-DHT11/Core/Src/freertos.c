/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DHT11_task.h"
#include "OLED_task.h"
#include "CAN_Config.h"
#include "CAN_task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_ReceiveHandler(hcan);
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for OLEDTask */
osThreadId_t OLEDTaskHandle;
const osThreadAttr_t OLEDTask_attributes = {
  .name = "OLEDTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for DHT11Task */
osThreadId_t DHT11TaskHandle;
const osThreadAttr_t DHT11Task_attributes = {
  .name = "DHT11Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for CANTask */
osThreadId_t CANTaskHandle;
const osThreadAttr_t CANTask_attributes = {
  .name = "CANTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow3,
};
/* Definitions for OLEDQueue */
osMessageQueueId_t OLEDQueueHandle;
const osMessageQueueAttr_t OLEDQueue_attributes = {
  .name = "OLEDQueue"
};
/* Definitions for CANQueue */
osMessageQueueId_t CANQueueHandle;
const osMessageQueueAttr_t CANQueue_attributes = {
  .name = "CANQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void OLEDStartTask(void *argument);
void DHT11StartTask(void *argument);
void CANStartTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of OLEDQueue */
  OLEDQueueHandle = osMessageQueueNew (2, sizeof(DHT11_Data), &OLEDQueue_attributes);

  /* creation of CANQueue */
  CANQueueHandle = osMessageQueueNew (5, sizeof(DHT11_Data), &CANQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of OLEDTask */
  OLEDTaskHandle = osThreadNew(OLEDStartTask, NULL, &OLEDTask_attributes);

  /* creation of DHT11Task */
  DHT11TaskHandle = osThreadNew(DHT11StartTask, NULL, &DHT11Task_attributes);

  /* creation of CANTask */
  CANTaskHandle = osThreadNew(CANStartTask, NULL, &CANTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_OLEDStartTask */
/**
* @brief Function implementing the OLEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLEDStartTask */
//void OLEDStartTask(void *argument)
//{
//  /* USER CODE BEGIN OLEDStartTask */
//  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }
//  /* USER CODE END OLEDStartTask */
//}

///* USER CODE BEGIN Header_DHT11StartTask */
///**
//* @brief Function implementing the DHT11Task thread.
//* @param argument: Not used
//* @retval None
//*/
///* USER CODE END Header_DHT11StartTask */
//void DHT11StartTask(void *argument)
//{
//  /* USER CODE BEGIN DHT11StartTask */
//  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }
//  /* USER CODE END DHT11StartTask */
//}

///* USER CODE BEGIN Header_CANStartTask */
///**
//* @brief Function implementing the CANTask thread.
//* @param argument: Not used
//* @retval None
//*/
///* USER CODE END Header_CANStartTask */
//void CANStartTask(void *argument)
//{
//  /* USER CODE BEGIN CANStartTask */
//  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }
//  /* USER CODE END CANStartTask */
//}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

