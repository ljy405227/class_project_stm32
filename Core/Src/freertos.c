/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "my_main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for myTask_key */
osThreadId_t myTask_keyHandle;
const osThreadAttr_t myTask_key_attributes = {
  .name = "myTask_key",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask_control */
osThreadId_t myTask_controlHandle;
const osThreadAttr_t myTask_control_attributes = {
  .name = "myTask_control",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for myTask_shell */
osThreadId_t myTask_shellHandle;
const osThreadAttr_t myTask_shell_attributes = {
  .name = "myTask_shell",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask_timer */
osThreadId_t myTask_timerHandle;
const osThreadAttr_t myTask_timer_attributes = {
  .name = "myTask_timer",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask_seg */
osThreadId_t myTask_segHandle;
const osThreadAttr_t myTask_seg_attributes = {
  .name = "myTask_seg",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTask_key(void *argument);
void StartTask_control(void *argument);
void StartTask_shell(void *argument);
void StartTask_timer(void *argument);
void StartTask_seg(void *argument);

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of myTask_key */
  myTask_keyHandle = osThreadNew(StartTask_key, NULL, &myTask_key_attributes);

  /* creation of myTask_control */
  myTask_controlHandle = osThreadNew(StartTask_control, NULL, &myTask_control_attributes);

  /* creation of myTask_shell */
  myTask_shellHandle = osThreadNew(StartTask_shell, NULL, &myTask_shell_attributes);

  /* creation of myTask_timer */
  myTask_timerHandle = osThreadNew(StartTask_timer, NULL, &myTask_timer_attributes);

  /* creation of myTask_seg */
  myTask_segHandle = osThreadNew(StartTask_seg, NULL, &myTask_seg_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTask_key */
/**
  * @brief  Function implementing the myTask_key thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask_key */
void StartTask_key(void *argument)
{
  /* USER CODE BEGIN StartTask_key */
  app_task_key(argument);
  /* USER CODE END StartTask_key */
}

/* USER CODE BEGIN Header_StartTask_control */
/**
* @brief Function implementing the myTask_control thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_control */
void StartTask_control(void *argument)
{
  /* USER CODE BEGIN StartTask_control */
  app_task_control(argument);
  /* USER CODE END StartTask_control */
}

/* USER CODE BEGIN Header_StartTask_shell */
/**
* @brief Function implementing the myTask_shell thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_shell */
void StartTask_shell(void *argument)
{
  /* USER CODE BEGIN StartTask_shell */
  app_task_shell(argument);
  /* USER CODE END StartTask_shell */
}

/* USER CODE BEGIN Header_StartTask_timer */
/**
* @brief Function implementing the myTask_timer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_timer */
void StartTask_timer(void *argument)
{
  /* USER CODE BEGIN StartTask_timer */
  app_task_timer(argument);
  /* USER CODE END StartTask_timer */
}

/* USER CODE BEGIN Header_StartTask_seg */
/**
* @brief Function implementing the myTask_seg thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_seg */
void StartTask_seg(void *argument)
{
  /* USER CODE BEGIN StartTask_seg */
  app_task_seg(argument);
  /* USER CODE END StartTask_seg */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

