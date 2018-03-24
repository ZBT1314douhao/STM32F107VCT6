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
#include "led.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
osThreadId led_0_BlinkTaskHandle, led_1_BlinkTaskHandle;          /**< led闪烁任务句柄 */
osThreadId STemwinDisplayTaskHandle;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void StartBlinkTask(void const* argument);
void StartBlinkTask1(void const *argument);
void STemwinDisplayTask1(void const *argument);

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /** 添加LED线程(有句柄)  线程名 线程函数 优先级 线程实例个数 栈区大小 */
    osThreadDef(ledBlinkTask, StartBlinkTask, osPriorityNormal, 0, 128);
    /** 创建线程,使其为就绪状态  NULL 传递到线程的启动参数 返回线程的ID(句柄？) */
    led_0_BlinkTaskHandle = osThreadCreate(osThread(ledBlinkTask), NULL);
    
    osThreadDef(ledBlinkTask1, StartBlinkTask1, osPriorityAboveNormal, 0, 128);
    led_1_BlinkTaskHandle = osThreadCreate(osThread(ledBlinkTask1), NULL);
    
    osThreadDef(emwinDisplayTask, STemwinDisplayTask1, osPriorityNormal, 0, 128);
    STemwinDisplayTaskHandle = osThreadCreate(osThread(emwinDisplayTask), NULL);
 
    
  /* USER CODE END RTOS_THREADS */

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

/* USER CODE BEGIN Application */

/**
 *  @brief  led0闪烁任务
 *  @param  void const* argument 
 *  @retval None
 */
void StartBlinkTask(void const *argument)
{  
//    osPriority uxPriority;
    
//    uxPriority = osThreadGetPriority(NULL);

    
    while (1)
    {
        led_on(LED_0);
        osDelay(500);
        led_off(LED_0);
        osDelay(500);
        /** 提升STemwin线程的优先级 使得STemwin线程立即得到执行*/
//        osThreadSetPriority(STemwinDisplayTaskHandle, (osPriority)(uxPriority + 1));
    }
}    

/**
 *  @param  void const* argument 
 *  @brief  led闪烁任务
 *  @retval None
 */
void StartBlinkTask1(void const *argument)
{  
//    osPriority uxPriority;
    uint32_t PreviousWakeTime;
    const uint32_t Frequency = 3000;            /**< 每固定3s调用StartBlinkTask1 */
//    uxPriority = osThreadGetPriority(NULL);
    /** 获取系统时间 */
    PreviousWakeTime = osKernelSysTick();
    
    while (1)
    {
        led_on(LED_1);
        osDelay(500);
        led_off(LED_1);
        osDelay(500);
        /** 提升STemwin线程的优先级 使得STemwin线程立即得到执行*/
//        osThreadSetPriority(STemwinDisplayTaskHandle, (osPriority)(uxPriority + 1));
        osDelayUntil(&PreviousWakeTime, Frequency / portTICK_RATE_MS);
    }
} 

/** 
 *  @brief  STemwin任务 利用vTaskDdelayUntil函数实现周期性显示
 *  @param  void const* argument    任务参数
 *  @retval None
 */
void STemwinDisplayTask1(void const *argument)
{
//    osPriority uxPriority;
    
//    uxPriority = osThreadGetPriority(NULL);
    
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_GREEN);
    GUI_SetFont(&GUI_Font32B_ASCII);
    
    while (1)
    { 
        GUI_DispStringAt("I am Z B T", 0, 50);	  
        /** 设置默认优先级(NULL代表改变自己线程的优先级) */
        /** 如果注释掉这句 LED会闪亮一下然后熄灭进入STemwin线程 LED线程应该被饿死了 */
//        osThreadSetPriority(NULL, (osPriority)(uxPriority - 2));
    }
}


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
