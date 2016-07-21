/** 
 *  @file       key.c
 *  @brief      按键驱动程序
 *  @author     王晓荣   
 *  @version    
 *  @date       2014-08-29 
 */
 
#include "key.h"
#include "delay.h"

/** 配置key用到的端口、引脚 */
const struct GPIO_KEY key[] = 
{
    GPIOA, GPIO_PIN_0,
    GPIOC, GPIO_PIN_13,
};

#define NUM_KEYS    (sizeof(key) / sizeof(struct GPIO_KEY))
    
static enum KEY key_press_down(void);
    
/**
 *  @brief  读取按键值    
 *  @param  None
 *  @return enum KEY
 */
enum KEY key_read(void)
{
    enum KEY key_value = NO_KEY;
    
    key_value = key_press_down();
    
    if (key_value != NO_KEY)
    {
        delay_n_ms(100);
        while (key_press_down() != NO_KEY)
        {
            delay_n_ms(50);
        }
    }
    
    return key_value;
}

/**
 *  @brief  按键按下检测    
 *  @param  None
 *  @return enum KEY
 */
static enum KEY key_press_down(void)
{
    unsigned char i;
    enum KEY key_value = NO_KEY;
    
    for (i = 0; i < NUM_KEYS; i++)
    {
        /** 如果检测到按键按下 */
        if (!HAL_GPIO_ReadPin(key[i].port, key[i].pin))
        {
            key_value = i;
            break;
        }
    }
    
    return key_value;
}
