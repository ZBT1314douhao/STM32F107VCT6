/**
  *
  * @file	rtc_dup.h
  * @brief	STM32F107VCT6�ڲ�RTC
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			���δ���
  */

#ifndef __RTC_DUP_H
#define __RTC_DUP_H

/** -------------------- ͷ�ļ������� ------------------------------ */
#include "stm32f1xx_hal.h"

/** ---------------------- �ļ��ⲿ�ӿ� ---------------------------- */
void RTC_display_current_time(const uint16_t coor_x, const uint16_t coor_y);
void RTC_display_current_date(const uint16_t coor_x, const uint16_t coor_y);

void rtc_init(void);
RTC_TimeTypeDef RTC_get_time(void);
RTC_DateTypeDef RTC_get_date(void);
void RTC_set_time(RTC_TimeTypeDef *sTime);
void RTC_set_date(RTC_DateTypeDef *sDate);

#endif
