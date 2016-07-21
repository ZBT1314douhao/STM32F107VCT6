/**
  * @file       tim_dup.c 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 03
  * @brief      TIM6-7基本定时器
  *
  */  

#include "tim_dup.h"
//#include "led.h"
//#include "delay.h"

static uint8_t timer_flag[] = {0, 0, };

/**
 *  @brief  在中断函数中置位定时器标志 
 *  @param  enum GENERAL_TIM timer_x TIMER_2 -- TIMER_5
 *  @return None
 */
void set_timer_flag(enum BASE_TIMx timer_x)
{
    timer_flag[timer_x] = 1;
}    

/**
 *  @brief  读取timer的标志
 *  @param 	enum BASE_TIM6_7 timer_x -- TIMER_6  TIMER_7	
 *  @return 0 定时时间未到 ---- 1 定时时间到
 */
uint8_t read_timer_flag(enum BASE_TIMx timer_x)
{
    uint8_t tmp_flag;
    
    tmp_flag = timer_flag[timer_x];
    timer_flag[timer_x] = 0;           /**< 清除定时器标志位 */
    
    return tmp_flag;  
}



