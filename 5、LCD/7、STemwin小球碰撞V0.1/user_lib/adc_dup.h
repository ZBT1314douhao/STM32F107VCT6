/**
  * @file       adc_dup.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 04
  * @brief      AD转换处理程序
  *
  */  

#ifndef __ADC_DUP_H
#define __ADC_DUP_H

#include "stm32f1xx_hal.h"


void ADC_start(void);
uint16_t ADC_get_value(void);

#endif
