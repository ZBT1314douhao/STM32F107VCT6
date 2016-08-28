/**
  * @file	oxygen.h 
  * @brief	��ȡ����Ũ��
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			���δ���			
  */

#ifndef __OXYGEN_H
#define __OXYGEN_H

/** -------------------- ͷ�ļ������� ------------------------------ */
#include "stm32f1xx_hal.h"

struct S_AD_OXYGEN
{
    double oxygen_value; /**< ���ݵ�ѹֵת�������Ũ��ֵ */
    double value;        /**< ��ȡ�ĵ�ѹֵ */
};

enum ADC2_INx
{   
    ADC2_IN8, 
};

/** ---------------------- �ļ��ⲿ�ӿ� ---------------------------- */
void    ADC2_start_IT(void);
uint8_t read_adc2_it_flag(enum ADC2_INx adc2_x);
void    set_adc2_it_flag(enum ADC2_INx adc2_x);
void    display_oxygen_concertration(const uint16_t coor_x, const uint16_t coor_y);

#endif
