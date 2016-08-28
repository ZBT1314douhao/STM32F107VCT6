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
osThreadId led_0_BlinkTaskHandle, led_1_BlinkTaskHandle;          /**< led��˸������ */
osThreadId STemwinDisplayTaskHandle, ResumeStartBlinkTaskHandle;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void StartBlinkTask(void const* argument);
void StartBlinkTask1(void const *argument);
void ResumeStartBlinkTask(void const *argument);
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
  /** ���LED�߳�(�о��)  �߳��� �̺߳��� ���ȼ� �߳�ʵ������ ջ����С */
    osThreadDef(ledBlinkTask, StartBlinkTask, osPriorityNormal, 0, 128);
    /** �����߳�,ʹ��Ϊ����״̬  NULL ���ݵ��̵߳��������� �����̵߳�ID(�����) */
    led_0_BlinkTaskHandle = osThreadCreate(osThread(ledBlinkTask), NULL);
    
    osThreadDef(ledBlinkTask1, StartBlinkTask1, osPriorityAboveNormal, 0, 128);
    led_1_BlinkTaskHandle = osThreadCreate(osThread(ledBlinkTask1), NULL);
    
    osThreadDef(ResumeledBlinkTask, ResumeStartBlinkTask, osPriorityNormal, 0, 128);
    ResumeStartBlinkTaskHandle = osThreadCreate(osThread(ResumeledBlinkTask), NULL);
    
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
 *  @brief  led0��˸����
 *  @param  void const* argument 
 *  @retval None
 */
void StartBlinkTask(void const *argument)
{  
//    osPriority uxPriority;
    
//    uxPriority = osThreadGetPriority(NULL);
  
    while (1)
    {
        osThreadSuspend(led_0_BlinkTaskHandle);      /**< �����Լ� ������NULL������������ */
        led_on(LED_0);
        osDelay(500);
        led_off(LED_0);
        osDelay(500);
        /** ����STemwin�̵߳����ȼ� ʹ��STemwin�߳������õ�ִ��*/
//        osThreadSetPriority(STemwinDisplayTaskHandle, (osPriority)(uxPriority + 1));
    }
}    

/**
 *  @brief  �ָ�led0��˸���� ÿ5S�ָ�һ��
 *  @param  void const* argument 
 *  @retval None
 */
void ResumeStartBlinkTask(void const *argument)
{
    uint32_t PreviousWakeTime;
    const uint32_t Frequency = 5000;
    
    PreviousWakeTime = osKernelSysTick();
    
    while (1)
    {
        osThreadResume(led_0_BlinkTaskHandle);
        osDelayUntil(&PreviousWakeTime, Frequency);
    }
}

/**
 *  @brief  led��˸���� ÿ3S����һ��
 *  @param  void const* argument 
 *  @retval None
 */
void StartBlinkTask1(void const *argument)
{  
//    osPriority uxPriority;
    uint32_t PreviousWakeTime;
    const uint32_t Frequency = 3000;            /**< ÿ�̶�3s����StartBlinkTask1 */
//    uxPriority = osThreadGetPriority(NULL);
    /** ��ȡϵͳ��ǰʱ�� */
    PreviousWakeTime = osKernelSysTick();
    
    while (1)
    {
        led_on(LED_1);
        osDelay(500);
        led_off(LED_1);
        osDelay(500);
        /** ����STemwin�̵߳����ȼ� ʹ��STemwin�߳������õ�ִ��*/
//        osThreadSetPriority(STemwinDisplayTaskHandle, (osPriority)(uxPriority + 1));
        osDelayUntil(&PreviousWakeTime, Frequency / portTICK_RATE_MS);
    }
} 

/** 
 *  @brief  STemwin���� ����vTaskDdelayUntil����ʵ����������ʾ
 *  @param  void const* argument    �������
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
        /** ����Ĭ�����ȼ�(NULL����ı��Լ��̵߳����ȼ�) */
        /** ���ע�͵���� LED������һ��Ȼ��Ϩ�����STemwin�߳� LED�߳�Ӧ�ñ������� */
//        osThreadSetPriority(NULL, (osPriority)(uxPriority - 2));
    }
}


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
