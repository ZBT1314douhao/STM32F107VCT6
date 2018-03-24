/**
  * @file  bsp.h
  * @brief 存放驱动层头文件
  * @par   date        version    author    remarks
  *        2016-08-12  v1.0       zbt       初次创建
  *
  */

#ifndef __BSP_H
#define __BSP_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/** 以下未用到的外设可以将其头文件注释掉 ------------------------- */
/** 片上外设驱动头文件 */
#include "spi_dup.h"

/** 用户增加的外设头文件 */
#include "gui.h"            /**< STemwin */
#include "lcd_ssd1289.h"
#include "key.h"
#include "led.h"
#include "w25x16_cfg.h"

/** 公有宏(类型定义) -------------------------------------------- */

/** 枚举、结构体 ------------------------------------------------ */

/** 公有函数原型 ------------------------------------------------ */
void bsp_init(void);
//void bsp_idle(void);

#endif
