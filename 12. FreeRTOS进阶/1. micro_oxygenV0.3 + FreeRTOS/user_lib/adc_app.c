/**
  * @file	ad_app.c 
  * @brief	AD测量显示
  * @par	date        version     author      remarks
  *			2016-07-27	V1.0		zbt			初次创建
  */

/** -------------------- 头文件包含区 ------------------------------ */
#include "adc_app.h"
#include "adc.h"
#include "gui.h"

/** -------------------- 文件私有变量 ------------------------------ */
#define NUM_OF_AD_CHANNEL           2       /**< AD通道数量 */
#define CHANNEL_BUFF_SIZE           8       /**< 通道缓冲区大小 */
#define AD_BUFF_SIZE                (NUM_OF_AD_CHANNEL * CHANNEL_BUFF_SIZE) 

/** AD数据总缓冲区大小 */
static __IO uint16_t ad_value_buff[AD_BUFF_SIZE];
//static struct ADC1_VALUE    s_adc1_value;
static struct SENSOR_VALUE  s_sensor_value;
static struct ADC1_VOLTAGE Vlotage;

/** -------------------- 外部引用变量 ------------------------------ */

/** -------------------- 私有函数原型 ------------------------------ */
static struct ADC1_VOLTAGE    ADC1_get_value(void);
static struct SENSOR_VALUE  get_sensor_value(void);

/** -------------------- 公有函数 --------------------------------- */
/**
 * @brief  启动AD转换
 * @param  None
 * @retval None
 */
void ADC_start_convert(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ad_value_buff, AD_BUFF_SIZE);
}

/**
 *  @brief  显示采集传感器的值
 *  @param  coor_x：X坐标
            coor_y：Y坐标
 *  @retval None
 */
void display_sensor_value(const uint16_t coor_x, const uint16_t coor_y)
{
    s_sensor_value = get_sensor_value();
    
    GUI_GotoXY(coor_x, coor_y);
    GUI_DispFloat(Vlotage.sensor_channel, 4);
    GUI_DispStringAt("V", (coor_x + 70), coor_y);
    GUI_GotoXY((coor_x + 110), coor_y);
    GUI_DispFloat(s_sensor_value.temperature, 4);
    
    GUI_GotoXY(coor_x, (coor_y + 40));
    GUI_DispFloat(Vlotage.channel_10, 4);
    GUI_DispStringAt("V", (coor_x + 70), (coor_y + 40));
    GUI_GotoXY((coor_x + 115), (coor_y + 40));
    GUI_DispFloat(s_sensor_value.flow, 4);
}

/** -------------------- 私有函数 --------------------------------- */

/**
 *  @brief  获取AD转换后各传感器的值(如温度、浓度、流量等)
 *  @param  None
 *  @retval 标度变换后的值
 */
static struct SENSOR_VALUE get_sensor_value(void)
{   
    Vlotage = ADC1_get_value();   
    
    s_sensor_value.temperature = (1.43 - Vlotage.sensor_channel) / 4.3 + 25.0;
    s_sensor_value.flow = Vlotage.channel_10 * 0.5 ;

    return s_sensor_value;
}


/**
 * @brief   获取AD各通道的值
 * @param   None
 * @retval  s_adc1_value：存放AD各通道的值的结构体的电压值
 */
static struct ADC1_VOLTAGE ADC1_get_value(void)
{
    uint8_t i, j;
    uint32_t channel_10 = 0;
    uint32_t sensor_channel = 0; 
//    struct ADC1_VOLTAGE Vlotage;
    
    for (i = 0; i < NUM_OF_AD_CHANNEL; i++)
    {
        for (j = i; j < AD_BUFF_SIZE; j += NUM_OF_AD_CHANNEL)
        {
            if (i == 0)
            {
                /** 获取温度传感器通道的值 */
                sensor_channel += ad_value_buff[j];    
            }
            else if (i == 1)
            {
                /** 获取channel 10的值 */
                channel_10 += ad_value_buff[j];
            }

        }
    }
    /** 取平均值 */
    Vlotage.channel_10 = (double) channel_10 / (AD_BUFF_SIZE / 2);
    Vlotage.channel_10 = (double)(3.300 * Vlotage.channel_10 / 4095);
    
    Vlotage.sensor_channel = (double)sensor_channel / (AD_BUFF_SIZE / 2);
    Vlotage.sensor_channel = (double)(3.300 * Vlotage.sensor_channel / 4095);
      
    return Vlotage;
}
