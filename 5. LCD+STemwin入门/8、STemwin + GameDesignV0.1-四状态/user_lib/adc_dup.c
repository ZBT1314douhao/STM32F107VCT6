/**
  * @file       adc_dup.c 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 04
  * @brief      AD转换处理程序
  *
  */  

#include "adc_dup.h"
#include "adc.h"
#include "delay.h"

#define ADC_BUFFER_SIZE     16

static __IO uint16_t adc_buff[ADC_BUFFER_SIZE];

 /**
  *  @brief  启动ADC
  *  @param  None  		
  *  @return None
  */
void ADC_start(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_buff, ADC_BUFFER_SIZE);
}

 /**
  *  @brief  将采样值转换为电压值 mV
  *  @param    		
  *  @return None
  */
uint16_t ADC_get_value(void)
{
    uint8_t  i = 0;
    uint32_t adc_value;
    uint16_t value;
    
    for (i = 0; i < ADC_BUFFER_SIZE; i++)
    {
        adc_value += adc_buff[i];
    }
    
    adc_value = adc_value / ADC_BUFFER_SIZE;
    value = (uint16_t)(((3300 * adc_value)) / 4096);
    
    return value;
}





