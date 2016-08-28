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

struct ADC_Value
{
    double channel_10;       
    double temperature_channel;  /**< 芯片内部温度传感器通道值 */
};

void AWD_interrupt_handler(void);
void ADC_start_convert(void);
void ADC_stop_convert(void);
struct ADC_Value ADC_get_value(void);

#endif
