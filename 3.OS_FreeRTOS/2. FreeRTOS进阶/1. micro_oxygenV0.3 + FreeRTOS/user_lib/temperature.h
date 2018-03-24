/**
  * @file	temperature.h 
  * @brief	利用STM32F107VCT6内部温度传感器获取环境温度
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			初次创建			
  */

#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

/** -------------------- 头文件包含区 ------------------------------ */
#include "stm32f1xx_hal.h"


/** ---------------------- 文件外部接口 ---------------------------- */

void display_temperature(const uint16_t coor_x, const uint16_t coor_y);

#endif
