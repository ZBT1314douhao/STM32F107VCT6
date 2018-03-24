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

/** -------------------- 文件私有宏定义 ---------------------------- */
#define NUM_OF_AD_CHANNEL           3       /**< AD通道数量 */
#define CHANNEL_BUFF_SIZE           4       /**< 通道缓冲区大小 */
#define AD_BUFF_SIZE                (NUM_OF_AD_CHANNEL * CHANNEL_BUFF_SIZE) 

/** -------------------- 文件私有变量 ------------------------------ */
/** AD数据总缓冲区大小 */
static __IO uint32_t ad_value_buff[AD_BUFF_SIZE];

/** -------------------- 外部引用变量 ------------------------------ */

/** -------------------- 私有函数原型 ------------------------------ */
static struct ADC1_VOLTAGE  ADC1_get_value(void);
static struct SENSOR_VALUE  get_sensor_value(void);

/** -------------------- 公有函数 --------------------------------- */
/**
 * @brief   启动AD转换
 * @param   None
 * @retval  None
 */
void ADC_start_convert(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ad_value_buff, AD_BUFF_SIZE);
}

/**
 * @brief  显示采集传感器的值
 * @param  coor_x：X坐标
           coor_y：Y坐标
 * @retval None
 */
void display_sensor_value(const uint16_t coor_x, const uint16_t coor_y)
{
    struct SENSOR_VALUE Sensor;
    
    Sensor = get_sensor_value();
    
    GUI_SetFont(&GUI_Font24B_1); 
    /** 显示氧传感器的电压值和转换后的氧浓度值 */
    GUI_GotoXY(coor_x, coor_y);
    GUI_DispFloat(Sensor.v_adc1.channel_9, 5);
    GUI_DispStringAt("V", (coor_x + 60), coor_y);
    GUI_GotoXY((coor_x + 100), coor_y);
    GUI_DispFloat(Sensor.oxygen, 4);
    GUI_DispStringAt("%", 259, coor_y);
    
    /** 显示温度传感器的电压值和转换后的温度值 */
    GUI_GotoXY(coor_x, (coor_y + 40));
    GUI_DispFloat(Sensor.v_adc1.sensor_channel, 5);
    GUI_DispStringAt("V", (coor_x + 60), (coor_y + 40));
    GUI_GotoXY((coor_x + 100), (coor_y + 40));
    GUI_DispFloat(Sensor.temperature, 4);
    GUI_DispStringAt("*C", 259, (coor_y + 40));
    
    /** 显示流量传感器的电压值和转换后的流量值 */
    GUI_GotoXY(coor_x, (coor_y + 80));
    GUI_DispFloat(Sensor.v_adc1.channel_10, 5);
    GUI_DispStringAt("V", (coor_x + 60), (coor_y + 80));
    GUI_GotoXY((coor_x + 100), (coor_y + 80));
    GUI_DispFloat(Sensor.flow, 4);
    GUI_DispStringAt("L/min", 259, (coor_y + 80));    
}

/** -------------------- 私有函数 --------------------------------- */

/**
 * @brief  将AD转换后的电压值转换为传感器的值(如温度、浓度、流量等)
 * @param  None
 * @retval 标度变换后的值
 */
static struct SENSOR_VALUE get_sensor_value(void)
{   
    struct SENSOR_VALUE Sensor;
    
    Sensor.v_adc1 = ADC1_get_value();   /**< 获取各通道的电压值 */
    
    /** 将电压值转换为传感器的对应值 */
    Sensor.oxygen = Sensor.v_adc1.channel_9 * 2.0;
    Sensor.temperature = (1.43 - Sensor.v_adc1.sensor_channel) / 4.3 + 25.0;
    Sensor.flow = Sensor.v_adc1.channel_10 * 0.8;
    
    return Sensor;
}

/**
 * @brief   获取AD各通道的值
 * @param   None
 * @retval  s_adc1_value：存放AD各通道的值的结构体的电压值
 */
static struct ADC1_VOLTAGE ADC1_get_value(void)
{
    uint8_t i, j; 
    uint16_t channel_9 = 0;
    uint16_t channel_10 = 0;
    uint16_t sensor_channel = 0; 
    struct ADC1_VOLTAGE Vlotage;
    
    for (i = 0; i < NUM_OF_AD_CHANNEL; i++)
    {
        for (j = i; j < AD_BUFF_SIZE; j += NUM_OF_AD_CHANNEL)
        {
            if (i == 0)
            {
                sensor_channel += ad_value_buff[j];    
            }
            else if (i == 1)
            {
                channel_10 += ad_value_buff[j];
            }
            else if (i == 2)
            {
                channel_9 += ad_value_buff[i];
            }

        }
    }
    /** 转换为相应的电压值 */
    Vlotage.channel_9 = (double)channel_9 / CHANNEL_BUFF_SIZE;
    Vlotage.channel_9 = (3.300 * Vlotage.channel_9 / 4095);
    
    Vlotage.channel_10 = (double)channel_10 / CHANNEL_BUFF_SIZE;
    Vlotage.channel_10 = (3.300 * Vlotage.channel_10 / 4095);
    
    Vlotage.sensor_channel = (double)sensor_channel / CHANNEL_BUFF_SIZE;
    Vlotage.sensor_channel = (3.300 * Vlotage.sensor_channel / 4095);
      
    return Vlotage;
}
