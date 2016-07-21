/**
 * @file 	led.h
 * @brief  	led驱动程序
 * @author 	王晓荣
 * @version 
 * @date 2014-03-07
 */
 
#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

struct GPIO_LED
{
    GPIO_TypeDef      *port;
    uint16_t          pin;
};

#define	LED_0		0x01	
#define	LED_1		0x02	
#define	LED_2		0x04	
#define	LED_3		0x08	
#define	LED_4		0x10
#define	LED_5		0x20
#define	LED_6		0x40
#define	LED_7		0x80
#define	LED_ALL	    0xFFFF

void    led_on(uint16_t val);
void    led_off(uint16_t val) ;



#endif 
