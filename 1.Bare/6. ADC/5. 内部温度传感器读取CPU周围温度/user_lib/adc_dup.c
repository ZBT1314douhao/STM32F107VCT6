/**
  * @file       adc_dup.c 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 04
  * @brief      AD转换处理程序 DMA方式
  *
  */  

#include "adc_dup.h"
#include "adc.h"
#include "gui.h"
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
    HAL_ADCEx_Calibration_Start(&hadc1);    /**< 启动前进行校准 */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_buff, ADC_BUFFER_SIZE);
}

 /**
  *  @brief  显示CPU周围的温度
  *  @param  None  		
  *  @return None
  * @note   温度计算公式 Temperature = (V25 - VSensor) / AvgSlope + 25
            V25 & AVGSlope见芯片手册， 对于STM32F107VCT6来讲，典型值
            V25 = 1.43V， AVGSlope = 4.3 mV/^C
  */
void display_temperature(void)
{
    uint16_t voltage;
    float    temperature;
    
    GUI_SetColor(GUI_RED);
    GUI_SetFont(&GUI_Font32B_ASCII); 
    GUI_DispStringAt("Temp: ", 40, 100);
    GUI_DispStringAt("*C", 200, 100);     
    
    GUI_DispStringAt("Volt: ", 40, 150);
    GUI_DispStringAt(".", 136, 150);              /**< 小数点 */
    GUI_DispStringAt("V", 184, 150);              /**< 单位 */
    
    voltage = ADC_get_value();
    temperature = (1430 - voltage) / 4.3f + 25.0;
    
    GUI_DispDecAt(voltage / 1000,      120, 150, 1);  /**< 电压整数值 */      
    GUI_DispDecAt(voltage % 1000 / 10, 145, 150, 2);  /**< 2位小数 */  
    
    GUI_GotoXY(145, 100);
    GUI_DispFloat(temperature, 4);
    
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
    
    adc_value /= ADC_BUFFER_SIZE;        
    value = (uint16_t)(3300 * adc_value / 4095);
    
    return value;
}





