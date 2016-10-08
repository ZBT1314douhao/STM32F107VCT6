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
void AT24C02_write_data(uint8_t *write_data, uint16_t start_addr, uint16_t data_length);
void AT24C02_read_data(uint8_t *read_data, uint16_t start_addr, uint16_t data_length);

#endif

