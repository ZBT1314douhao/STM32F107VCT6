/**
  * @file  key.h
  * @brief 按键驱动程序
  * @par   date        version    author    remarks
  *        2016-03-19  v1.0       zbt       初次创建
  *
  */
 
#ifndef	__KEY_H
#define	__KEY_H 

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */

/** 枚举、结构体 ------------------------------------------------ */ 
enum KEY
{
    /** LEFT_SHIFT */ KEY_TAMPER, RIGHT_SHIFT, NO_KEY = 0xff,
}; 

struct GPIO_KEY
{
    GPIO_TypeDef    *port;
    uint16_t        pin;
};

/** 公有函数原型 ------------------------------------------------ */
enum KEY key_read(void);


#endif 
