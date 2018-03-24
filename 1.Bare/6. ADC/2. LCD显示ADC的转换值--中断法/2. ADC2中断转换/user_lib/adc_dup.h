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

enum ADCx_INx
{   /**< 开发板上的PC0 为引出AD */
    ADC2_IN10, 
};

void set_adc_flag(enum ADCx_INx adc2_n);
uint8_t read_adc_flag(enum ADCx_INx adc2_n);
uint32_t ADC_get_value(ADC_HandleTypeDef* adc2_x);

#endif
