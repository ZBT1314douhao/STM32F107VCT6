/**
 * @file    led.c
 * @brief   led驱动程序
 * @author  zbt
 * @version V0.1 
 * @date    2016-03
 */

#include "led.h"

/** 配置led用到的端口、引脚 */
const struct GPIO_LED led[] =
{
    GPIOD, GPIO_PIN_2,
    GPIOD, GPIO_PIN_3,
    GPIOD, GPIO_PIN_4,
    GPIOD, GPIO_PIN_7,
};

#define NUM_LEDS (sizeof(led) / sizeof(struct GPIO_LED))


/**
 * @brief   点亮led驱动程序 可同时点亮多个 
 * @param   val 多个LED组合
 * @return  None
 * @note    例如： led_on(LED_0 | LED_1)
 */
void led_on(uint16_t val) 
{
    uint8_t n;
    
    for(n = 0; n < NUM_LEDS; n++)
    {
        if (val & (1 << n)) 
        {
            HAL_GPIO_WritePin(led[n].port, led[n].pin, GPIO_PIN_SET);        
        }        
    }    
}

/**
 * @brief   熄灭led驱动程序 可同时熄灭多个 
 * @param   val 多个LED组合
 * @return  None
 * @note    例如： led_off(LED_0 | LED_1)
 */
void led_off(uint16_t val) 
{
    uint8_t n;
    
    for(n = 0; n < NUM_LEDS; n++)
    {
        if (val & (1 << n)) 
        {
            HAL_GPIO_WritePin(led[n].port, led[n].pin, GPIO_PIN_RESET);        
        }        
    }     
}



