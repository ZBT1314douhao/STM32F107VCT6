/**
  * @file  led.h
  * @brief led驱动程序
  * @par   date        version    author    remarks
  *        2016-03-20  v1.0       zbt       初次创建
  *
  */ 
 
#ifndef __LED_H
#define __LED_H

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */
/** 注：本开发板上仅有4个led */
#define	LED_0		0x01	
#define	LED_1		0x02	
#define	LED_2		0x04	
#define	LED_3		0x08	
#define	LED_4		0x10
#define	LED_5		0x20
#define	LED_6		0x40
#define	LED_7		0x80
#define	LED_ALL	    0xFF

/** 枚举、结构体 ------------------------------------------------ */ 
struct GPIO_LED
{
    GPIO_TypeDef      *port;
    uint16_t          pin;
};

/** 公有函数原型 ------------------------------------------------ */
void led_on(uint16_t val);
void led_off(uint16_t val) ;
void led_toggle(uint16_t val);

#endif 
