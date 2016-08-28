/** 
 *  @file       app_led.c
 *  @brief      按键控制led显示功能
 *  @author     zbt  
 *  @version    V0.1
 *  @date       2016
 */
 
#include "app_led.h"
#include "key.h"
#include "led.h"
#include "delay.h"

static enum LED_FUNCTION led_function = LED_RESET;

/**
 *  @brief  led实现各种功能    
 *  @param  None
 *  @return None
 */
void led_fun(void)
{
    switch (led_function)
    {
    case LED_RESET:
        led_reset();
        break;
    case LED_BINARY_INC:
        binary_increment();
        break;
    case LED_BINARY_DEC:
        binary_decrement();
        break;
    case LED_RIGHT_SHIFT:
        right_shift();
        break;
    case LED_LEFT_SHIFT:
        left_shift();
        break;
    }
}

/**
 *  @brief  led默认状态    
 *  @param  None
 *  @return None
 */
void led_reset(void)
{
    led_off(LED_ALL);
    
    if (key_read() == LEFT_SHIFT)
    {
        led_function = LED_BINARY_INC;
    }
}

/**
 *  @brief  led右移    
 *  @param  None
 *  @return None
 */
void right_shift(void)
{
    static uint8_t i;
    
    i = (i == 0)? 3 : i - 1;
    led_on(LED_0 << i);
    delay_n_ms(100);
    led_off(LED_0 << i);
//    delay_n_ms(500);
    
    if (key_read() == LEFT_SHIFT)
    {
        led_function = LED_LEFT_SHIFT;
    }
}

/**
 *  @brief  led左移  
 *  @param  None
 *  @return None
 */
void left_shift(void)
{
    static uint8_t i;
    
    i = (i >= 3)? 0 : i + 1;
    led_on(LED_0 << i);
    delay_n_ms(100);
    led_off(LED_0 << i);
//    delay_n_ms(500);
    
    if (key_read() == LEFT_SHIFT)
    {
        led_function = LED_RESET;
    }
}

/**
 *  @brief  ledled实现二进制加法   
 *  @param  None
 *  @return None
 */
void binary_increment(void)
{
    static uint16_t led_n = 0x00;
    led_n = (led_n > 0x0f)? 0 : led_n + 1;
    led_on(led_n);
    delay_n_ms(1000);       /**< 延时约 1s */
    led_off(led_n);
//    delay_n_ms(500);
    
    if (key_read() == LEFT_SHIFT)
    {
        led_function = LED_BINARY_DEC;
    }
}

/**
 *  @brief  led实现二进制减法   
 *  @param  None
 *  @return None
 */
void binary_decrement(void)
{
    static uint16_t led_n = 0x0f;
    led_n = (led_n == 0)? 0x0f : led_n - 1;
    led_on(led_n);
    delay_n_ms(1000);       /**< 延时约 1s */
    led_off(led_n);
//    delay_n_ms(500);
    
    if (key_read() == LEFT_SHIFT)
    {
        led_function = LED_RIGHT_SHIFT;
    }
} 
