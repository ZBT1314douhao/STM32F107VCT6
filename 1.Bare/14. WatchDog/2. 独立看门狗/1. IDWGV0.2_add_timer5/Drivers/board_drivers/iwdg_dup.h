/**
  * @file  iwdg_dup.h
  * @brief 独立看门狗上层程序
  * @par   date        version    author    remarks
  *        2016-08-13  v1.0       zbt       初次创建
  *
  */

#ifndef __IWDG_DUP_H
#define __IWDG_DUP_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */

/** 枚举、结构体 ------------------------------------------------ */

/** 公有函数原型 ------------------------------------------------ */
void start_iwdg(void);
void feed_iwdg(void);
void check_system_resume_status(void);

#endif
