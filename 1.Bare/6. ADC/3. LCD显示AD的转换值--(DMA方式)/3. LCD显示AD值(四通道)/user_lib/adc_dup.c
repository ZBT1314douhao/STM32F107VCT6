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

#define RANGE_OF_12BITS             (4095)
#define VDDA_VALUE                  (3300)
#define GET_VOLTAGE(ad_value)       (VDDA_VALUE * ad_value / RANGE_OF_12BITS)

#define NUM_OF_AD_CHANNEL   4       /**< AD通道数量 */
#define CHANNEL_BUFF_SIZE   8       /**< 通道缓冲区大小 */
#define ADC_BUFFER_SIZE     (NUM_OF_AD_CHANNEL * CHANNEL_BUFF_SIZE) 

static __IO uint16_t adc_buff[ADC_BUFFER_SIZE];

static struct ADC1_VOLTAGE ADC_get_value(void);

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
  *  @brief  显示各通道的值
  *  @param  None  		
  *  @return None
  *  @note   电压表测得读数和LCD显示读数相比，电压表读数小于LCD显示读数0.3V
             左右
             温度转换公式见内部温度传感器读取温度程序 or 参考芯片手册->电气
             特性部分
  */
void display_channel_value(void)
{
    struct ADC1_VOLTAGE Voltage;

    Voltage = ADC_get_value();

    GUI_SetColor(GUI_RED);
    GUI_SetFont(&GUI_Font24B_ASCII); 
    
    /** 显示依次为 channel8,channel9, channel_sensor, channel10 */
    GUI_DispStringAt("Volt: ", 40, 60);
    GUI_DispStringAt("mV", 150, 60);  
    GUI_GotoXY(90, 60);
    GUI_DispFloat(Voltage.channel_8, 4);
    
    GUI_DispStringAt("Volt: ", 40, 90);
    GUI_DispStringAt("mV", 150, 90);  
    GUI_GotoXY(90, 90);
    GUI_DispFloat(Voltage.channel_9, 4);
    
    GUI_DispStringAt("Volt: ", 40, 120);
    GUI_DispStringAt("mV", 150, 120);  
    GUI_GotoXY(90, 120);
    GUI_DispFloat(Voltage.sensor_channel, 4); 
    GUI_GotoXY(200, 120);
    /** 利用CPU内部的温度传感器 计算并显示CPU周围的温度 */
    GUI_DispFloat(((1430 - Voltage.sensor_channel) / 4.3f + 25.0), 2);
    GUI_DispStringAt("*C", 230, 120);
    
    GUI_DispStringAt("Volt: ", 40, 150);
    GUI_DispStringAt("mV", 150, 150);
    GUI_GotoXY(90, 150);
    GUI_DispFloat(Voltage.channel_10, 4);  
}

 /**
  *  @brief  将采样值转换为电压值 mV
  *  @param  None	
  *  @return None
  */
static struct ADC1_VOLTAGE ADC_get_value(void)
{
    uint8_t  i, j;
    uint32_t channel_sum[NUM_OF_AD_CHANNEL];
    uint16_t channel_value[NUM_OF_AD_CHANNEL];
    struct ADC1_VOLTAGE Voltage;
    
    for (i = 0; i < NUM_OF_AD_CHANNEL; i++)
    {
        channel_sum[i] = 0;
        for (j = i; j < CHANNEL_BUFF_SIZE; j ++)
        {
            /** 求出每个通道的AD值总和 */
            channel_sum[i] += adc_buff[i + j * NUM_OF_AD_CHANNEL];
        }
        /** 求出各通道平均值 */
        channel_value[i] = (uint16_t)channel_sum[i] / CHANNEL_BUFF_SIZE;
    }
    
    Voltage.sensor_channel = (uint16_t)GET_VOLTAGE(channel_value[0]);
    Voltage.channel_8 = (uint16_t)GET_VOLTAGE(channel_value[1]);
    Voltage.channel_9 = (uint16_t)GET_VOLTAGE(channel_value[2]);
    Voltage.channel_10 = (uint16_t)GET_VOLTAGE(channel_value[3]);    
    
    return Voltage;
}





