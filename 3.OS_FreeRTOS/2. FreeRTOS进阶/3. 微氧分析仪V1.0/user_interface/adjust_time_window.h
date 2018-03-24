/**
  * @file	adjust_time_window.h 
  * @brief	RTCʱ��ʱ�����ý���
  * @par	date        version     author      remarks
  *			2016-07-24	V1.0		zbt			���δ���			
  */

#ifndef __ADJUST_TIME_WINDOW_H
#define __ADJUST_TIME_WINDOW_H

/** -------------------- ͷ�ļ������� ------------------------------ */
#include "stm32f1xx_hal.h"
#include "ui_drivers.h"

/** ---------------------- �ļ��ⲿ�ӿ� ---------------------------- */
void adj_time_window_info(WINDOW_INFO *info);
void rtc_write_to_spinbox(void);
#endif
