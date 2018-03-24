/**
  * @file    uart_drv.h
  * @author  Z B T
  * @version V0.1
  * @date    2016/05
  * @brief   uart1上层驱动
  *
  */

#ifndef __UART_DRV_H
#define __UART_DRV_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */

/** 公有函数原型 ------------------------------------------------ */
void uart_test(void);

HAL_StatusTypeDef uart_transmit(uint8_t *data, uint16_t size);
HAL_StatusTypeDef uart_receive(uint8_t *data, uint16_t size);

#endif
