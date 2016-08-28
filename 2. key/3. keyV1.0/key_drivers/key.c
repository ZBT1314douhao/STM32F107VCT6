/** 
 *  @file       key.c
 *  @brief      按键驱动程序
 *  @author     zbt 
 *  @version    V1.0
 *  @date       2016-08-21
 */
 
#include "key.h"
#include "delay.h"

#define NUM_KEYS    (sizeof(GPIO_Key) / sizeof(GPIO_Key[0]))

#define KEY_PRESSED             GPIO_PIN_RESET
#define KEY_RELEASED            GPIO_PIN_SET

/** 单位 1ms */
#define KEY_DEBOUNCE_TIME		10		/**< 去抖动时间 */
#define KEY_HOLDING_TIME		1000	/**< 确认为长按的保持时间 */

/** 配置key用到的端口、引脚 */
const struct GPIO_KEY GPIO_Key[] = 
{
    GPIOA, GPIO_PIN_0,
    GPIOC, GPIO_PIN_13,
};

static struct KEY Key[NUM_KEYS];


/**
 *  @brief  按键初始化（不包含按键GPIO配置）
 *  @param  None
 *  @return None
 */
void key_init(void)
{
	uint8_t i;

    /** 初始化KEY结构体的各成员 */
	for (i = 0; i < NUM_KEYS; i++)
	{
		Key[i].state = (GPIO_PinState)(!KEY_PRESSED);       /**< 0按下， 1 未按下 */                            
		Key[i].debounce_count = KEY_DEBOUNCE_TIME / 2;  
		//Key[i].holding_count = ;
		Key[i].hold_time = KEY_HOLDING_TIME;
		Key[i].repeat_speed = 0;        /**< 按键连发速度设置 0： 不连发*/
		Key[i].repeat_count = 0;        /**< 连发计数器 */
	}
    
    /** 单独设置某个按键的参数 */
    Key[TAMPER].hold_time = 100;
}

/**
 *  @brief  读取按键值    
 *  @param  None
 *  @return enum KEY
 */
struct KEY key_read(void)
{
    uint8_t i;
    
    for (i = 0; i < NUM_KEYS; i++)
    {
        Key[i].state = get_key_state(i);    /**< 读取按键当前状态 */
        
        if (Key[i].state == KEY_PRESSED)
        {
            HAL_Delay(KEY_DEBOUNCE_TIME);   /**< 延时消抖 */
            /** 再次确认按键是否被按下 */
            if (get_key_state(i) == KEY_PRESSED)
            {
                HAL_Delay(KEY_HOLDING_TIME);
                
                if (get_key_state(i) == KEY_PRESSED)     /**< 长按 */
                {
                    Key[i].status = (enum KEY_STATE)(3 * i + 2);     
                }
                else    /**< 短按 */                    
                {
                    Key[i].status = (enum KEY_STATE)(3 * i + 0);
                }
            }
            else       /**< 按键误触处理程序 */
            {
                ;   
            }

            while (get_key_state(i) == KEY_RELEASED);       /**< 等待按键释放 */
            Key[i].status = (enum KEY_STATE)(3 * i + 1);   /**< 按键状态更改为为 抬起 */     
        }
        else
        {
            break;
        }
    }
    
    return *Key;
}

/**
 *  @brief  按键按下检测    
 *  @param  key_id： enum KEY_ID的除NO_KEY外的所有枚举值
 *  @return KeyId：被按下的键的KEY_ID
 */
GPIO_PinState get_key_state(uint8_t key_id)
{    
    /** 如果检测到按键按下 根据本开发板的电路设计可得
        HAL_GPIO_ReadPin返回值为0 
    */
    if (HAL_GPIO_ReadPin(GPIO_Key[key_id].port, GPIO_Key[key_id].pin) 
        == GPIO_PIN_RESET)
    {
        Key[key_id].state = KEY_PRESSED;
        return GPIO_PIN_RESET;
    }
    else
    {
        Key[key_id].state = KEY_RELEASED;
        return GPIO_PIN_SET;  
    }
    
//    return GPIO_PIN_SET;
}


