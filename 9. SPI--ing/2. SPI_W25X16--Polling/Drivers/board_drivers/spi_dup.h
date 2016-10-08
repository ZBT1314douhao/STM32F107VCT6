/**
* @file  spi_dup.h
* @brief SPI上层函数
* @par   date        version    author    remarks
*        2016-08-14  v1.0       zbt       初次创建
*
*/

#ifndef __SPI_DUP_H
#define __SPI_DUP_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */
#define DUMMY_BYTE          0xA5    /**< 伪（无效）字节 */    

/** 枚举、结构体 ------------------------------------------------ */ 


/** 公有函数原型 ------------------------------------------------ */
uint8_t spi_readwrite_onebyte(uint8_t send_byte);

#endif
