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

/** 文件私有宏定义 ------------------------------------------------ */
#define VDDA_VALUE                  ((uint16_t)3300)        /**< 模拟供应电压值（VDDA） mV */
#define RANGE_12BITS                ((uint16_t)4095)        /**< 12位分辨率的ADC的最大数字量 */

#define GET_MASTRE_VALUE(VALUE)     ((VALUE) & 0x0000FFFF)  /**< 获取主ADC的各通道的值 */
#define GET_SLAVE_VALUE(VALUE)      ((VALUE) >> 16)         /**< 获取从ADC的各通道的值 */ 
/** 将获取的AD值转换为相应的电压值 */
#define GET_VLOTAGE_VALUE(VALUE)    ((VALUE) * VDDA_VALUE / RANGE_12BITS)

#define NUM_OF_AD_CHANNEL           1                       /**< AD通道数量 */
#define CHANNEL_BUFF_SIZE           16                       /**< 通道缓冲区大小 */
#define AD_BUFF_SIZE                (NUM_OF_AD_CHANNEL * CHANNEL_BUFF_SIZE) 
/** 文件私有变量 ------------------------------------------------ */
static __IO uint16_t adc_buff[AD_BUFF_SIZE];
static __IO uint16_t ad_master_value[AD_BUFF_SIZE];         /**< 将获取到的32位值分配到相应的AD缓冲区 */
static __IO uint16_t ad_slave_value[AD_BUFF_SIZE];

 /**
  *  @brief  启动ADC
  *  @param  None  		
  *  @return None
  */
void ADC_start_convert(void)
{
    /** 启动前执行自校准 */
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADCEx_Calibration_Start(&hadc2);
    /** 使能从机ADC */
    HAL_ADC_Start(&hadc2);
    HAL_Delay(200);
    /** 启动AD转换 */
    HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)adc_buff, AD_BUFF_SIZE); 
}

 /**
  *  @brief  显示各通道的电压值 mV
  *  @param  None	
  *  @return None
  */
void display_voltage(void)
{
    struct AD_VALUE Voltage;
    
    Voltage = ADC_get_value();
    
    GUI_SetColor(GUI_GREEN);
    GUI_SetFont(&GUI_Font32B_ASCII);    
    GUI_DispStringAt("Volt: ", 40, 100);
    GUI_DispStringAt(".", 136, 100);              
    GUI_DispStringAt("V", 184, 100);              
    GUI_DispStringAt("Volt: ", 40, 150);
    GUI_DispStringAt(".", 136, 150);              /**< 小数点 */
    GUI_DispStringAt("V", 184, 150);              /**< 单位 */
    
    GUI_DispDecAt(Voltage.adc1_value / 1000,      120, 150, 1);  /**< 电压整数值 */      
    GUI_DispDecAt(Voltage.adc1_value % 1000 / 10, 145, 150, 2);  /**< 2位小数 */   

    GUI_DispDecAt(Voltage.adc2_value / 1000,      120, 100, 1);
    GUI_DispDecAt(Voltage.adc2_value % 1000 / 10, 145, 100, 2);
    delay_ms(200); 
}

 /**
  *  @brief  将采样值转换为电压值 mV
  *  @param  None	
  *  @return None
  */
struct AD_VALUE ADC_get_value(void)
{
    uint8_t  i = 0;
    uint32_t adc_value = 0;
    struct AD_VALUE Voltage;
    
    for (i = 0; i < (AD_BUFF_SIZE); i++)
    {
        adc_value += adc_buff[i];
    }
    
    adc_value /= AD_BUFF_SIZE;    
    
    Voltage.adc1_value = (uint16_t)GET_MASTRE_VALUE(adc_value);
    Voltage.adc2_value = (uint16_t)GET_SLAVE_VALUE(adc_value);
    
    Voltage.adc1_value = GET_VLOTAGE_VALUE(Voltage.adc1_value);
    Voltage.adc2_value = GET_VLOTAGE_VALUE(Voltage.adc2_value);
    
    return Voltage;
}





