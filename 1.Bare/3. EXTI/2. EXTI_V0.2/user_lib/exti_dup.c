/**
  * @file       exti_dup.c 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 03
  * @brief      外部中断应用程序 还可利用外部中断的回调函数实现
  *
  */  

#include "exti_dup.h"
#include "led.h"
#include "delay.h"

static uint8_t exti_flag[] = {0, 0,};

/**
 *  @brief  置位外部中断
 *  @param 	enum EXTI_N exti_n	外部中断编号
 *  @return None
 */
void set_exti_flag(enum EXTI_N exti_n)
{
    exti_flag[exti_n] = 1;
}

/**
 *  @brief  读取外部中断标志
 *  @param 	enum EXTI_N exti_n	外部中断编号	
 *  @return None
 */
uint8_t read_exti_flag(enum EXTI_N exti_n)
{
    uint8_t flag;
    
    flag = exti_flag[exti_n];
    exti_flag[exti_n] = 0;          /**< 清除中断标志位 */
    
    return flag;
}

/**     参考用法
    if (read_exti_flag(EXTI_0))
    {
       code
    }
*/ 




