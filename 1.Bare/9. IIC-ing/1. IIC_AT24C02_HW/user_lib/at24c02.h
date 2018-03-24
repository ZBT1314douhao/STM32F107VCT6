/**
  * @file  at24c02.h
  * @brief at24c02驱动程序
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       初次创建
  *
  */ 

#ifndef _AT24C02__H
#define _AT24C02__H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */

/** 枚举、结构体 ------------------------------------------------ */ 

/** 公有函数原型 ------------------------------------------------ */
void AT24C02_iic_test(void);

void at24c02_write(uint8_t addr, uint8_t *p_data, uint8_t size);
void at24c02_read(uint8_t addr, uint8_t *p_data, uint8_t size);

#endif

