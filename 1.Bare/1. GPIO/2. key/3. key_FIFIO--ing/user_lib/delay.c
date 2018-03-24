/**
  *
  * @file    delay.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016-
  * @brief   粗略延时函数
  *
  */  

#include "delay.h"
#include "key.h"

static void idle_task(void);


/**
 *  @brief  用户程序中重新实现HAL_Delay();
 *  @param  Delay： 延时时间 单位： ms
 *  @return None
 */
void HAL_Delay(__IO uint32_t Delay)
{
    uint32_t tick_start = 0;
    
    tick_start = HAL_GetTick();
    while((HAL_GetTick() - tick_start) < Delay)
    {
        idle_task();
    }
}

/**
 *  @brief   延时n_us
 *  @param  [in]n_us	粗略延时
 *  @return None
 */
void delay_n_us(uint32_t n_us)
{
  uint8_t i = 0;

  while (n_us--)
  {
    i = 10;
    while (i--);
  }
}

/**
 *  @brief  延时n_ms
 *  @param  [in] n_ms 粗略延时
 *  @return None
 */
void delay_n_ms(uint32_t n_ms)
{
  uint16_t i = 0;

  while (n_ms--)
  {
    i = 12000;
    while (i--);
  }
}

/**
 *  @brief  周期性运行的程序
 *  @param  None
 *  @return None
 *  @note   比如喂狗、CPU休眠或者其他优先级较低的任务
            在SysTick_Handler();中被调用
 */
void task_run_per10ms(void)
{
    key_scan();
}

/**
 *  @brief  空闲任务
 *  @param  None
 *  @return None
 *  @note   比如喂狗、CPU休眠或者其他优先级较低的任务
            在HAL_Delay();中被调用
 */
static void idle_task(void)
{
    
}
