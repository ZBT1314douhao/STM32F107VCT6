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
#define SPI_FLASH_DUMMY_BYTE          0xA5    /**< 伪（无效）字节 */    

/** 枚举、结构体 ------------------------------------------------ */ 


/** 公有函数原型 ------------------------------------------------ */
uint8_t spi_flash_read_onebyte(void);
uint8_t spi_flash_write_onebyte(uint8_t send_byte);

HAL_StatusTypeDef spi_flash_transmit(uint8_t *tx_data, uint16_t size);
HAL_StatusTypeDef spi_flash_transmit_receive(uint8_t *tx_data, uint8_t *rx_data, uint16_t size);

#endif
