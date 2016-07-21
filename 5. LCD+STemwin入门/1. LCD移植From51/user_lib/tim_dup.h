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

enum GENERAL_TIM
{   /**< STM32f107vct6的通用定时器 */
    TIMER_2, TIMER_3, TIMER_4, TIMER_5, 
};


void set_timer_flag(enum GENERAL_TIM timer_x);
uint8_t read_timer_flag(enum GENERAL_TIM timer_x);   

#endif
