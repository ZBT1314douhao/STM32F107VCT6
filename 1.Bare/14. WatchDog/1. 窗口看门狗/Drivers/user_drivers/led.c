/**
  * @file  led.c
  * @brief led驱动程序
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       初次创建
  *
  */ 

/** 头文件包含区 ------------------------------------------------ */
#include "led.h"

/** 私有宏(类型定义) -------------------------------------------- */ 
#define NUM_LEDS (sizeof(led) / sizeof(led[0]))
    
/** 私有变量 --------------------------------------------------- */
/** 配置led用到的端口、引脚 */
const struct GPIO_LED led[] =
{
    GPIOD, GPIO_PIN_2,
    GPIOD, GPIO_PIN_3,
    GPIOD, GPIO_PIN_4,
    GPIOD, GPIO_PIN_7,
};

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */

/** 公有函数 --------------------------------------------------- */

/**
  * @brief  点亮led驱动程序 可同时点亮多个 
  * @param  val 多个LED组合
  * @retval None
  * @note   例如： led_on(LED_0 | LED_1)
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
  * @brief  熄灭led驱动程序 可同时熄灭多个 
  * @param  val 多个LED组合
  * @retval None
  * @note   例如： led_on(LED_0 | LED_1)
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

/**
  * @brief  反转led
  * @param  val 多个LED组合
  * @retval None
  * @note   例如： led_toggle(LED_0 | LED_1)
  */
void led_toggle(uint16_t val) 
{
    uint8_t n;
    
    for(n = 0; n < NUM_LEDS; n++)
    {
        if (val & (1 << n)) 
        {
            HAL_GPIO_TogglePin(led[n].port, led[n].pin);        
        }        
    }     
}

/** 私有函数 --------------------------------------------------- */
