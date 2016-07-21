/**
  * @file       exti_dup..h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 03
  * @brief      外部中断应用程序 引脚配置见gpio.c
  *
  */  

#ifndef __EXTI_DUP_H
#define __EXTI_DUP_H

#include "stm32f1xx_hal.h"

enum EXTI_N
{
    EXTI_0, EXTI_13,
};

void set_exti_flag(enum EXTI_N exti_n);
uint8_t read_exti_flag(enum EXTI_N exti_n);


#endif
