/**
  * @file       tim_dup.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 03
  * @brief      TIM6-7基本定时器
  *
  */  

#ifndef __TIM_DUP_H
#define __TIM_DUP_H

#include "stm32f1xx_hal.h"

enum BASE_TIMx
{   /**< STM32f107vct6的基本定时器 */
    TIMER_6, TIMER_7,  
};


void set_timer_flag(enum BASE_TIMx timer_x);
uint8_t read_timer_flag(enum BASE_TIMx timer_x);   

#endif
