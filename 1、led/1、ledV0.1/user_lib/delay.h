/**
  *
  * @file    delay.h 
  * @author  Z B T
  * @version V0.1
  * @date    2016-
  * @brief   延时函数 SysTick精确延时和粗略延时
  *
  */  

#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f1xx_hal.h"

void delay_n_ms(uint32_t n_ms);
void delay_n_us(uint32_t n_us);


#endif
