/**
  * @file	temperature.h 
  * @brief	����STM32F107VCT6�ڲ��¶ȴ�������ȡ�����¶�
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			���δ���			
  */

#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

/** -------------------- ͷ�ļ������� ------------------------------ */
#include "stm32f1xx_hal.h"


/** ---------------------- �ļ��ⲿ�ӿ� ---------------------------- */

void display_temperature(const uint16_t coor_x, const uint16_t coor_y);

#endif
