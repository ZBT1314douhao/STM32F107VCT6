/**
  * @file	ad_app.h 
  * @brief	AD������ʾ
  * @par	date        version     author      remarks
  *			2016-07-27	V1.0		zbt			���δ���			
  */

#ifndef __AD_APP_H
#define __AD_APP_H

/** -------------------- ͷ�ļ������� ------------------------------ */
#include "stm32f1xx_hal.h"

struct ADC1_VOLTAGE
{
    double channel_10;       
    double sensor_channel;  /**< оƬ�ڲ��¶ȴ�����ͨ��ֵ */
};

struct SENSOR_VALUE
{
    double temperature;   /**< ת������¶�ֵ */
    double flow;        /**< ת�������Ũ��ֵ */
};

/** ---------------------- �ļ��ⲿ�ӿ� ---------------------------- */
void ADC_start_convert(void);
void display_sensor_value(const uint16_t coor_x, const uint16_t coor_y);

#endif
