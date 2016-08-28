/**
  * @file	ad_app.h 
  * @brief	AD测量显示
  * @par	date        version     author      remarks
  *			2016-07-27	V1.0		zbt			初次创建			
  */

#ifndef __AD_APP_H
#define __AD_APP_H

/** -------------------- 头文件包含区 ------------------------------ */
#include "stm32f1xx_hal.h"

struct ADC1_VOLTAGE
{
    double channel_10;       
    double sensor_channel;  /**< 芯片内部温度传感器通道值 */
};

struct SENSOR_VALUE
{
    double temperature;   /**< 转换后的温度值 */
    double flow;        /**< 转换后的氧浓度值 */
};

/** ---------------------- 文件外部接口 ---------------------------- */
void ADC_start_convert(void);
void display_sensor_value(const uint16_t coor_x, const uint16_t coor_y);

#endif
