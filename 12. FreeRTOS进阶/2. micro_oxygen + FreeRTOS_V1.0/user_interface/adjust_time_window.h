/**
  * @file	adjust_time_window.h 
  * @brief	RTC时钟时间设置界面
  * @par	date        version     author      remarks
  *			2016-07-24	V1.0		zbt			初次创建			
  */

#ifndef __ADJUST_TIME_WINDOW_H
#define __ADJUST_TIME_WINDOW_H

/** -------------------- 头文件包含区 ------------------------------ */
#include "stm32f1xx_hal.h"
#include "ui_drivers.h"

/** ---------------------- 文件外部接口 ---------------------------- */
void adj_time_window_info(WINDOW_INFO *info);
void rtc_write_to_spinbox(void);
#endif
