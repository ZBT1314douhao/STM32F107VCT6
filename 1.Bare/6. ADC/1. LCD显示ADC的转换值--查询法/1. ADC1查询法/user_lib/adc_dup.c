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

#define ADC_BUFF_SIZE       16

static uint8_t adc1_flag[] = {0, 0,};


 /**
  *  @brief  读取ADC的值
  *  @param  ADC 外设编号   	
  *  @return 12位的ADC的值
  */
uint32_t ADC_get_value(ADC_HandleTypeDef* adc1_x)
{
    uint8_t  i = 0;
    uint32_t adc_value;
    uint32_t value;
    
	
    /**< 等待转换完成  暂时不明加上这句就不可以的原因 */
//    while (HAL_ADC_PollForConversion(&hadc1, 60) == HAL_OK); 
    
    for (i = 0; i < ADC_BUFF_SIZE; i++)
    {
        adc_value += HAL_ADC_GetValue(&hadc1);
    }
    /**< 多次测量去平均值 亦可根据需要采用其他数字滤波算法 */
    adc_value /= ADC_BUFF_SIZE;
    value = (((3300 * adc_value)) >> 12);
    
    return value;
}

/**< 中断法  本例程未采用中断法 */

/**
 *  @brief  
 *  @param 	
 *  @return None
 */
void set_adc_flag(enum ADCx_INx adc1_n)
{
    adc1_flag[adc1_n] = 1;
}

 /**
  *  @brief  
  *  @param 	
  *  @return None
  */
uint8_t read_adc_flag(enum ADCx_INx adc1_n)
{
    uint8_t tmp_flag;

    tmp_flag = adc1_flag[adc1_n];
    adc1_flag[adc1_n] = 0;          /**< 清除中断标志位 */

    return tmp_flag;
}





