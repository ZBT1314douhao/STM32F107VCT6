/** 
 *  @file       app_led.h
 *  @brief      按键控制led显示功能
 *  @author     zbt        
 *  @version    V0.1
 *  @date       2016
 */
 
#ifndef	__APP_LED_H
#define	__APP_LED_H 

#include "stm32f1xx_hal.h"

enum LED_FUNCTION
{
    LED_RESET,
    LED_BINARY_INC, LED_BINARY_DEC, LED_RIGHT_SHIFT, LED_LEFT_SHIFT,
};

void led_fun(void);
void led_reset(void);
void binary_increment(void);
void binary_decrement(void);
void right_shift(void);
void left_shift(void);

#endif
