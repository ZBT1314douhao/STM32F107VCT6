/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "gui.h"            /**< STemwin */
#include "adc_dup.h"
#include "ui_driver.h"
//#include "led.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId ad_taskHandle;
osThreadId emwin_taskHandle;
osMessageQId ad_queueHandle;

/* USER CODE BEGIN Variables */
static uint16_t voltage;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void start_ad_task(void const * argument);
void start_emwin_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

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

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityLow, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of ad_task */
  osThreadDef(ad_task, start_ad_task, osPriorityAboveNormal, 0, 128);
  ad_taskHandle = osThreadCreate(osThread(ad_task), NULL);

  /* definition and creation of emwin_task */
  osThreadDef(emwin_task, start_emwin_task, osPriorityNormal, 0, 128);
  emwin_taskHandle = osThreadCreate(osThread(emwin_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /** 添加LED线程(有句柄)  线程名 线程函数 优先级 线程实例个数 栈区大小 */
    
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of ad_queue */
  osMessageQDef(ad_queue, 1, uint8_t);
  ad_queueHandle = osMessageCreate(osMessageQ(ad_queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* start_ad_task function */
void start_ad_task(void const * argument)
{
  /* USER CODE BEGIN start_ad_task */
    osPriority uxPriority;
    
    uxPriority = osThreadGetPriority(ad_taskHandle);
    ADC_start(); 
    voltage = ADC_get_value(); 

  /* Infinite loop */
  for(;;) 
  {
    osDelay(1);
      /** 暂时提升emwin任务的优先级 */
    osThreadSetPriority(emwin_taskHandle, (osPriority)(uxPriority + 1));
  }
  /* USER CODE END start_ad_task */
}

/* start_emwin_task function */
void start_emwin_task(void const * argument)
{
  /* USER CODE BEGIN start_emwin_task */
    osPriority uxPriority;
    
    uxPriority = osThreadGetPriority(emwin_taskHandle);
    ui_init();
    
    GUI_DispStringAt("Volt: ", 40, 150);
    GUI_DispStringAt(".", 136, 150);              /**< 小数点 */
    GUI_DispStringAt("V", 184, 150);              /**< 单位 */
    
//    xQueueReceive(ad_queueHandle, (void*)&voltage, portMAX_DELAY);

  /* Infinite loop */
  for(;;)
  {
    GUI_DispDecAt(voltage / 1000,      120, 150, 1);  /**< 电压整数值 */      
    GUI_DispDecAt(voltage % 1000 / 10, 145, 150, 2);  /**< 2位小数 */
    osDelay(1);
      /** 撤销emwin优先级的提升 回到默认优先级 */
    osThreadSetPriority(NULL, (osPriority)(uxPriority - 1));
  }
  
  /* USER CODE END start_emwin_task */
}

/* USER CODE BEGIN Application */





/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
