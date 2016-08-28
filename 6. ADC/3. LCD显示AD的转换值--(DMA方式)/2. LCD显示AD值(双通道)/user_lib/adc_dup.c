/**
  * @file       adc_dup.c 
  * @author     Z B T
  * @version    V0.2
  * @date       2016- 04
  * @brief      AD转换处理程序 DMA方式 双通道
                温度传感器通道 & 接电阻的channel_10
  *
  */  

#include "adc_dup.h"
#include "gui.h"
#include "adc.h"
#include "delay.h"

#define NUM_OF_AD_CHANNEL   2       /**< AD通道数量 */
#define CHANNEL_BUFF_SIZE   8       /**< 通道缓冲区大小 */
#define ADC_BUFFER_SIZE     (NUM_OF_AD_CHANNEL * CHANNEL_BUFF_SIZE) 

static __IO uint16_t adc_buff[ADC_BUFFER_SIZE];

 /**
  *  @brief  启动ADC
  *  @param  None  		
  *  @return None
  */
void ADC_start_convert(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_Delay(50);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_buff, ADC_BUFFER_SIZE);
}

 /**
  *  @brief  启动ADC
  *  @param  None  		
  *  @return None
  *  @note   电压表测得读数和LCD显示读数相比，电压表读数小于LCD显示读数0.3V
             左右
             温度转换公式见内部温度传感器读取温度程序 or 参考芯片手册->电气
             特性部分
  */
void display_channel_value(void)
{
    struct ADC_Value Voltage;

    Voltage = ADC_get_value();

    GUI_SetColor(GUI_RED);
    GUI_SetFont(&GUI_Font32B_ASCII); 
    
    GUI_DispStringAt("Volt: ", 40, 150);
    GUI_DispStringAt("V", 184, 150);
    GUI_GotoXY(120, 150);
    GUI_DispFloat(Voltage.channel_10, 4);  
                  
    GUI_DispStringAt("T: ", 40, 100);
    GUI_DispStringAt("mV", 184, 100);  
    GUI_GotoXY(120, 100);
    GUI_DispFloat(Voltage.temperature_channel, 4);   
    GUI_DispStringAt("*C", 184, 50);    
    GUI_GotoXY(120, 50);
    GUI_DispFloat(((1430 - Voltage.temperature_channel) / 4.3f + 25.0), 2);
}

 /**
  *  @brief  将采样值转换为电压值 mV
  *  @param  None	
  *  @return None
  */
struct ADC_Value ADC_get_value(void)
{
    uint8_t  i, j;
    uint32_t channel_10 = 0;
    uint32_t temperature = 0;
    struct ADC_Value adc_value;
    
    for (i = 0; i < NUM_OF_AD_CHANNEL; i++)
    {
        for (j = i; j < ADC_BUFFER_SIZE; j += NUM_OF_AD_CHANNEL)
        {
            if (i == 1)
            {
                temperature += adc_buff[j];
            }
            else if (i == 0)
            {
                channel_10 += adc_buff[j];
            }
            
        }
    }
    
    adc_value.channel_10 = channel_10 / CHANNEL_BUFF_SIZE;
    adc_value.channel_10 = 3.30 * adc_value.channel_10 / 4095;
    
    adc_value.temperature_channel = temperature / CHANNEL_BUFF_SIZE;
    adc_value.temperature_channel = 3300 * adc_value.temperature_channel / 4095;
    
    return adc_value;
}





