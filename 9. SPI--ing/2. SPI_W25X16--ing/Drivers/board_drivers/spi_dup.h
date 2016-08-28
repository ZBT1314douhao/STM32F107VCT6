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
/** SPI传送状态 */
enum 
{
	TRANSFER_WAIT,
	TRANSFER_COMPLETE,
	TRANSFER_ERROR
};

/** 公有函数原型 ------------------------------------------------ */
uint8_t spi_read_onebyte(void);
uint8_t spi_send_onebyte(uint8_t send_byte);
void set_transfer_state(void);

#endif
