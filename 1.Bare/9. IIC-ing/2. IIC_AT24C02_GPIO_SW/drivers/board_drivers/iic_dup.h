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
#define IIC_WRITE                   0
#define IIC_READ                    1

/** 枚举、结构体 ------------------------------------------------ */ 

/** 公有函数原型 ------------------------------------------------ */
void iic_start(void);
void iic_stop(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);

void iic_sendbyte(uint8_t byte);
uint8_t iic_readbyte(void);

uint8_t iic_check_device_status(uint8_t device_addr);

#endif

