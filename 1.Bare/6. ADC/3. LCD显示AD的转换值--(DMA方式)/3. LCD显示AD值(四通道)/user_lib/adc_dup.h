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

struct ADC1_VOLTAGE
{
    uint16_t channel_8;       /**< 开发板外部通道 */    
    uint16_t channel_9;       /**< 开发板外部通道 */
    uint16_t channel_10;      /**< 电位器通道 */       
    uint16_t sensor_channel;  /**< 芯片内部温度传感器通道 */
};

void ADC_start_convert(void);
void display_channel_value(void);


#endif
