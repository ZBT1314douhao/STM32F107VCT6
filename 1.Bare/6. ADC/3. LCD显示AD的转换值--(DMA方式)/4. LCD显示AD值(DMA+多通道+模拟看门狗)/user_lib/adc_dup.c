/**
  * @file       adc_dup.c 
  * @author     Z B T
  * @version    V0.2
  * @date       2016- 04
  * @brief      AD转换处理程序 DMA方式 双通道
  *
  */  

#include "adc_dup.h"
#include "adc.h"
#include "delay.h"
#include "led.h"

#define NUM_OF_AD_CHANNEL   2       /**< AD通道数量 */
#define CHANNEL_BUFF_SIZE   8       /**< 通道缓冲区大小 */
#define ADC_BUFFER_SIZE     (NUM_OF_AD_CHANNEL * CHANNEL_BUFF_SIZE) 

static __IO uint16_t s_adc_buff[ADC_BUFFER_SIZE];

/**
 *  @brief  启动ADC
 *  @param  None  		
 *  @return None
*/
void ADC_start_convert(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1);        /**< 转换前启动自校准提高转换精度 */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&s_adc_buff, ADC_BUFFER_SIZE);
}

/**
 *  @brief  停止ADC
 *  @param  None  		
 *  @return None
*/
void ADC_stop_convert(void)
{
    HAL_ADC_Stop_DMA(&hadc1);
}

/**
 *  @brief  将采样值转换为电压值 mV
 *  @param  None	
 *  @return ADC1各通道的AD值
 */
struct ADC_Value ADC_get_value(void)
{
    uint8_t  i, j;
    uint32_t channel_10 = 0;
    uint32_t temperature = 0;
    struct ADC_Value ad_value;
    
    for (i = 0; i < NUM_OF_AD_CHANNEL; i++)
    {
        for (j = i; j < ADC_BUFFER_SIZE; j += NUM_OF_AD_CHANNEL)
        {
            if (i == 1)
            {
                channel_10 += s_adc_buff[j];
            }
            else if (i == 0)
            {
                temperature += s_adc_buff[j];
            }
            
        }
    }
    
    /** 将从各通道获取的AD值转换为相应的传感器值 */
    ad_value.channel_10 = (double)channel_10 / (ADC_BUFFER_SIZE / 2);
    ad_value.channel_10 = (double)(3.30 * ad_value.channel_10 / 4095);
    
    ad_value.temperature_channel = (double)temperature / (ADC_BUFFER_SIZE / 2);
    ad_value.temperature_channel = (double)(3.30 * ad_value.temperature_channel / 4095);
    
    return ad_value;
}

/**
 *  @brief  看门狗中断处理
 *  @param  None	
 *  @return None
 */
void AWD_interrupt_handler(void)
{
    while (1)
    {
        led_on(LED_ALL);
    }
}


