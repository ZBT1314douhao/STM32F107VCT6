/**
  * @file  tim_dup.c
  * @brief 定时器上层函数
  * @par   date        version    author    remarks
  *        2016-03-18  v1.0       zbt       初次创建
  *
  */ 

#ifndef __TIM_DUP_H
#define __TIM_DUP_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */

/** 枚举、结构体 ------------------------------------------------ */ 
enum BASE_TIMx
{   /**< STM32f107vct6的基本定时器 */
    TIMER_6, TIMER_7,  
};

/** 公有函数原型 ------------------------------------------------ */
void set_timer_flag(enum BASE_TIMx timer_x);
uint8_t read_timer_flag(enum BASE_TIMx timer_x);   

#endif
