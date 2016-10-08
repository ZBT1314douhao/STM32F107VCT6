/**
  * @file  uart_dup.h
  * @brief 串口上层程序
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       初次创建
  *
  */ 

#ifndef _UART_DUP_H
#define _UART_DUP_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */

/** 枚举、结构体 ------------------------------------------------ */ 

/** 公有函数原型 ------------------------------------------------ */
void    uart1_send_data(void); 
uint8_t uart1_get_recv_cmd(void);


#endif

