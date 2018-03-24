/**
  * @file  key.c
  * @brief 按键驱动程序
  * @par   date        version    author    remarks
  *        2016-03-19  v1.0       zbt       初次创建
  *
  */

/** 头文件包含区 ------------------------------------------------ */
#include "bsp.h"

/** 私有宏(类型定义) -------------------------------------------- */ 
#define NUM_KEYS    (sizeof(key) / sizeof(key[0]))
    
/** 私有变量 --------------------------------------------------- */ 
/** 配置key用到的端口、引脚 */
const struct GPIO_KEY key[] = 
{
    GPIOA, GPIO_PIN_0,
    GPIOC, GPIO_PIN_13,
};
  
/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */
static enum KEY key_press_down(void);

/** 公有函数 --------------------------------------------------- */

/**
  * @brief  读取按键值    
  * @param  None
  * @retval enum KEY
  */
enum KEY key_read(void)
{
    enum KEY key_value = NO_KEY;
    
    key_value = key_press_down();
    
    if (key_value != NO_KEY)
    {
//        delay_n_ms(100);
        HAL_Delay(20);
        while (key_press_down() != NO_KEY)
        {
//            delay_n_ms(50);
            HAL_Delay(50);
        }
    }
    
    return key_value;
}

/** 私有函数 --------------------------------------------------- */

/**
  * @brief  按键按下检测    
  * @param  None
  * @retval enum KEY
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
            key_value = (enum KEY)i;
            break;
        }
    }
    
    return key_value;
}
