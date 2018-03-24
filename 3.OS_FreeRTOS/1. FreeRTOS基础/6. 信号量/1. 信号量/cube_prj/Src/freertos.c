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
osSemaphoreId   osSemaphore;

osThreadId SemThread1Handle, SemThread2Handle;
//osThreadId   USART1_TestHandle;




/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
static void SemaphoreThread1(void const *argument);
static void SemaphoreThread2(void const *argument);
//void USART1_TestTask(void const *argument);

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
    osSemaphoreDef(SEM);            /**< ����һ���ź������� */
    /** ��������ʼ���ź������� �ڶ�������Ϊ�ɻ�õ���Դ�� */
    osSemaphore = osSemaphoreCreate(osSemaphore(SEM), 1);
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
  /** ��Ӷ��н�����Ϣ�߳�(�о��)  �߳��� �̺߳��� ���ȼ� �߳�ʵ������ ջ����С */
  osThreadDef(SEM_Thread1, SemaphoreThread1, osPriorityNormal, 0, 128);
  SemThread1Handle = osThreadCreate(osThread(SEM_Thread1), (void*)osSemaphore);
  
  osThreadDef(SEM_Thread2, SemaphoreThread2, osPriorityNormal, 0, 128);
  SemThread1Handle = osThreadCreate(osThread(SEM_Thread2), (void*)osSemaphore);
    
    
    /** �����߳� */
//    osThreadDef(usart1Task, USART1_TestTask, osPriorityNormal, 0, 128);
//    TransmitMsgTaskHandle = osThreadCreate(osThread(usart1Task), NULL);
  
    
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  
    /** ��������--��������--�����е������Ϣ��--������ϢԪ�ص��������� */
   
    
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
 *  @brief  LED_0��˸�߳�
 *  @param  argument: shared semphore
 *  @retval None
 */
void SemaphoreThread1(void const *argument)
{     
    uint32_t count = 0;
    osSemaphoreId semaphore = (osSemaphoreId) argument;
    
    while (1)
    {
        if (osSemaphore != NULL)
        {
            /** �ȴ��ź����ĵ��� ֱ����ʱ */
            if (osSemaphoreWait(osSemaphore, 100) == osOK)
            {
                count = osKernelSysTick() + 5000;
                while (count >= osKernelSysTick())
                {
                    led_toggle(LED_0);
                    osDelay(200);
                }
                
                led_off(LED_0);
                osSemaphoreRelease(semaphore);
                /** �����Լ���ִ�и������ȼ����� */
                osThreadSuspend(NULL);
            }
        }
    }
}    

/**
 *  @brief  LED_1��˸�߳�
 *  @param  argument: shared semphore
 *  @retval None
 */
void SemaphoreThread2(void const *argument)
{     
    uint32_t count = 0;
    osSemaphoreId semaphore = (osSemaphoreId) argument;
    
    while (1)
    {
        if (osSemaphore != NULL)
        {
            /** �ȴ��ź����ĵ��� ֱ����ʱ */
            if (osSemaphoreWait(osSemaphore, 0) == osOK)
            {
                osThreadResume(SemThread1Handle);
                
                count = osKernelSysTick() + 5000;
                /** ��5S��ʱ���� ÿ200ms��תһ��LED_1��״̬ */
                while (count >= osKernelSysTick())
                {
                    led_toggle(LED_1);
                    osDelay(200);
                }
                
                led_off(LED_1);
                /** �ͷ��ź��� �ָ��������ȼ����߳�1 */
                osSemaphoreRelease(semaphore);
            }
        }
    }
}    

/**
 *  @brief  ���ڴ�ӡ���Գ���
 *  @param  void const* argument  ��������ʱ���ݵ��β�
 *  @retval None
 */
//void USART1_TestTask(void const *argument)
//{
//    while (1)
//    {
////        taskDISABLE_INTERRUPTS();
////        taskENTER_CRITICAL();
//        osThreadSuspendAll();       /**< �����������������ݲ����ж� */
//        printf("FreeRTOS\r\n");
//        osThreadResumeAll();
////        taskEXIT_CRITICAL();
////        taskENABLE_INTERRUPTS();
//        osDelay(1000);
//    }
//}



/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
