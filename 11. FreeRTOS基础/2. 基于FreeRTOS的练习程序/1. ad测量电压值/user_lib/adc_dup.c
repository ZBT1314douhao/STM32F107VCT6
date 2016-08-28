/**
  * @file       adc_dup.c 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 04
  * @brief      AD转换处理程序 DMA方式
  *
  */  

#include "cmsis_os.h"
#include "adc_dup.h"
#include "adc.h"
#include "delay.h"

#define ADC_BUFFER_SIZE     16

static __IO uint16_t adc_buff[ADC_BUFFER_SIZE];

extern osMessageQId ad_queueHandle;

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
  *  @param  None	
  *  @return None
  */
uint16_t ADC_get_value(void)
{
    uint8_t  i = 0;
    uint16_t value = 0;
    uint32_t adc_value = 0;
    
    for (i = 0; i < ADC_BUFFER_SIZE; i++)
    {
        adc_value += adc_buff[i];
    }
    
    adc_value >>= 4;        
    value = (uint16_t)(3300 * adc_value / 4095);
    
//    xQueueSend(ad_queueHandle, (void*)&value, portMAX_DELAY);
    
    return value;
}





