/**
  * @file	oxygen.h 
  * @brief	获取氧气浓度
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			初次创建			
  */

#ifndef __OXYGEN_H
#define __OXYGEN_H

/** -------------------- 头文件包含区 ------------------------------ */
#include "stm32f1xx_hal.h"

struct S_AD_OXYGEN
{
    double oxygen_value; /**< 根据电压值转换后的氧浓度值 */
    double value;        /**< 获取的电压值 */
};

enum ADC2_INx
{   
    ADC2_IN8, 
};

/** ---------------------- 文件外部接口 ---------------------------- */
void    ADC2_start_IT(void);
uint8_t read_adc2_it_flag(enum ADC2_INx adc2_x);
void    set_adc2_it_flag(enum ADC2_INx adc2_x);
void    display_oxygen_concertration(const uint16_t coor_x, const uint16_t coor_y);

#endif
