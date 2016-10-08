/**
  * @file  iic_dup.h
  * @brief IIC上层程序
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       初次创建
  *
  */ 

#ifndef _IIC_DUP_H
#define _IIC_DUP_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */

/** 枚举、结构体 ------------------------------------------------ */ 

/** 公有函数原型 ------------------------------------------------ */
void AT24C02_iic_test(void);
HAL_StatusTypeDef AT24C02_write(uint8_t *buffer, uint8_t mem_addr, uint16_t num_to_write);
HAL_StatusTypeDef AT24C02_read(uint8_t *buffer, uint8_t mem_addr, uint16_t num_to_read);

#endif

