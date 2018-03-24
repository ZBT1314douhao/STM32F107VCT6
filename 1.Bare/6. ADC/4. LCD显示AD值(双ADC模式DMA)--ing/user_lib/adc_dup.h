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

struct AD_VALUE
{
    uint16_t adc1_value;
    uint16_t adc2_value;
};

void ADC_start_convert(void);
void display_voltage(void);
struct AD_VALUE ADC_get_value(void);

#endif
