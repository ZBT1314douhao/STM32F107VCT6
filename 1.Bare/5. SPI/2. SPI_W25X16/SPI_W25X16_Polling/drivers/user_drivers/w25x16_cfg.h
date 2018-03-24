/**
* @file  w25x16_cfg.h
* @brief 模板
* @par   date        version    author    remarks
*        2016-08-14  v1.0       zbt       初次创建
*
*/

#ifndef __W25X16_CFG_H
#define __W25X16_CFG_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */

#define W25X16_PAGE_SIZE                    100
/** 块、扇区、芯片擦除指令 */
#define W25X16_BLOCK_ERASE					0XD8
#define W25X16_SECTOR_ERASE					0X20
#define W25X16_CHIP_ERASE					0XC7

/** 枚举、结构体 ------------------------------------------------ */
/** W25X16擦除区域 */
enum W25X16_ERASE
{
    BLOCK_ERASE, SECTOR_ERASE, CHIP_ERASE,
};

struct W25X16_ID
{
    uint8_t maunfacture;    /**< 制造商ID */
    uint8_t memory;         /**< 存储器ID */
    uint8_t compatibility;  /**< 兼容性ID */
    uint8_t device;         /**< 器件ID */
    uint32_t jedec_device;  /**< JEDEC设备ID */
};

/** 公有函数原型 ------------------------------------------------ */
void w25x16_test(void);

void w25x16_set_power(FunctionalState state);

uint8_t w25x16_read_status_register(void);
void w25x16_write_status_register(uint8_t data);
void w25x16_read(uint32_t addr, uint8_t *buffer, uint16_t byte_num);
void w25x16_write(uint32_t addr, uint8_t *buffer, uint16_t byte_num);
void w25x16_page_program(uint32_t addr, uint8_t *buffer, uint16_t byte_num); 
void w25x16_erase(enum W25X16_ERASE EraseArea, uint32_t addr);

#endif
