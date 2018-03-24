/** 
 *  @file       key.c
 *  @brief      按键驱动程序
 *  @author     zbt 
 *  @version    V1.0
 *  @date       2016-08-21
 */
 
#include "key.h"
#include "led.h"
#include "delay.h"

#define NUM_KEYS    (sizeof(GPIO_Key) / sizeof(GPIO_Key[0]))

#define KEY_PRESSED                 0
#define KEY_RELEASED                1
#define KEY_HOLDING                 2

#define KEY_IS_PRESSED(id)         ((uint8_t)(3 * id + 1))
#define KEY_IS_HOLDING(id)         ((uint8_t)(3 * id + 2))
#define KEY_IS_RELEASED(id)        ((uint8_t)(3 * id + 3))

/** 配置key用到的端口、引脚 */
const struct GPIO_KEY GPIO_Key[] = 
{
    GPIOA, GPIO_PIN_0,
    GPIOC, GPIO_PIN_13,
};

static struct KEY sKey[NUM_KEYS];
static struct KEY_FIFO sKeyBuffer;

static void key_read(uint8_t id);
static GPIO_PinState key_get_state(uint8_t key_id);

/**
 * @brief  按键FIFO测试程序  
 * @param  None
 * @return None
 */
void key_fifo_test(void)
{   
    enum KEY_STATE eKeyState = NO_KEY;
    
    eKeyState = key_get_value_from_buffer();
    if (eKeyState != NO_KEY)
    {
        switch (eKeyState)
        {
        case LEFT_PRESSED:
            led_on(LED_1);
            printf("按键1按下，点亮led0\r\n");
            break;
        case LEFT_RELEASED:
            led_off(LED_1);
            printf("按键1抬起，熄灭led0\r\n");
            break;
        case LEFT_HOLDING:
            led_off(LED_3);
            printf("按键1保持，点亮led2\r\n");
            break;
        case RIGHT_PRESSED:
            led_on(LED_0);
            printf("按键0按下，点亮led1\r\n");
            break;
        case RIGHT_RELEASED:
            led_off(LED_0);
            printf("按键0抬起，熄灭led1\r\n");
            break;
        case RIGHT_HOLDING:
            led_on(LED_3);
            printf("按键0保持，点亮led3\r\n");
            break;
        default:
            break;
        }
       
    }
    
}

/**
 * @brief  按键初始化（不包含GPIO的配置)    
 * @param  None
 * @return None
 */
void key_init(void)
{
	uint8_t i;
    
    /** 初始化按键FIFO结构体 */
    sKeyBuffer.read = 0;
    sKeyBuffer.write = 0;
    /** 初始化KEY结构体的各成员 */
	for (i = 0; i < NUM_KEYS; i++)
	{
		sKey[i].state = (enum KEY_STATE)(NO_KEY);       /**< 0按下， 1 未按下 */                            
		printf("key init %d\r\n", sKey[i].state);
        sKey[i].pressed = KEY_RELEASED;
        sKey[i].debounce_count = 0;  
		sKey[i].holding_count = 0;
		sKey[i].holding_time = 0;
		sKey[i].repeat_speed = 0;        /**< 按键连发速度设置 0： 不连发*/
		sKey[i].repeat_count = 0;        /**< 连发计数器 */
	}
    
    /** 单独设置某个按键的参数 */
    sKey[0].holding_time = 50;
    sKey[1].holding_time = 100;
}

/**
 * @brief  从缓冲区获得键值    
 * @param  None
 * @return key_value： 获得的键值
 */
enum KEY_STATE key_get_value_from_buffer(void)
{
    enum KEY_STATE eKeyValue;
    
    if (sKeyBuffer.read == sKeyBuffer.write)
    {
        eKeyValue =  NO_KEY;      /**< 表示FIFO无可用按键值 */
    }
    else
    {
        if (sKeyBuffer.read >= KEY_FIFO_SIZE)
        {
            sKeyBuffer.read = 0;
        }
        eKeyValue = (enum KEY_STATE)sKeyBuffer.buffer[sKeyBuffer.read++];
    }
    
//    printf("读取到的缓冲区键值是 %d\r\n", eKeyValue);
    return eKeyValue;
}

/**
 * @brief  向缓冲区写入一个键值    
 * @param  key_value： 要写入的键值
 * @return None
 */
void key_put_value_to_buffer(enum KEY_STATE eKeyValue)
{
    if (sKeyBuffer.write >= KEY_FIFO_SIZE)
    {
        sKeyBuffer.write = 0;
    }
    sKeyBuffer.buffer[sKeyBuffer.write++] = eKeyValue;
    printf("写入缓冲区键值是 %d\r\n", eKeyValue);
}

/**
 * @brief  按键扫描函数    
 * @param  None
 * @return None
 * @note   在中断函数中被周期性的调用
 */
void key_scan(void)
{
    uint8_t i;
    
    for (i = 0; i < NUM_KEYS; i++)
    {
        key_read(i);
    }
}

/**
 *  @brief  读取按键值    
 *  @param  None
 *  @return enum KEY
 */
static void key_read(uint8_t id)
{     
    
    if (key_get_state(id) == KEY_PRESSED)
    {
        printf("key %d pressed\r\n", id);
        /** 按键消抖 */
        if (sKey[id].debounce_count < KEY_DEBOUNCE_TIME)
        {
            ++sKey[id].debounce_count;
            printf("debounce %d\r\n", sKey[id].debounce_count);
        }
        else
        {   /** 在检测到按下期间如果按键释放则发送按键释放 */
            if (sKey[id].holding_time > 0)
            {

                printf("key state %d\r\n",sKey[id].state);
                if (sKey[id].holding_count < sKey[id].holding_time)
                {
                    sKey[id].holding_count++;
                    printf("保持计数值 %d\r\n", sKey[id].holding_count);
                    if(sKey[id].holding_count == KEY_PRESSED_TIME)
                    {
                        sKey[id].state = KEY_PRESSED;       /**< 表示短按 */
                        key_put_value_to_buffer((enum KEY_STATE)KEY_IS_PRESSED(id));
                        printf("按键 %d 短按\r\n", id);
                    }
//                    else 
                        if((sKey[id].holding_count >= sKey[id].holding_time))
                    {
                        sKey[id].state = KEY_HOLDING;       /**< 表示长按 */
                        key_put_value_to_buffer((enum KEY_STATE)KEY_IS_HOLDING(id));
                        printf("按键 %d 长按\r\n", id);
                    }
                }
            }
            else 
            {
                sKey[id].state = KEY_PRESSED;
                key_put_value_to_buffer((enum KEY_STATE)KEY_IS_PRESSED(id));
                printf("按键 %d 被按下等待释放\r\n", id);
            }
        }     
    }
    else    /**< 按键释放 */
    {
        if (sKey[id].debounce_count != 0)
        {
            if (sKey[id].debounce_count > KEY_DEBOUNCE_TIME)
            {
                sKey[id].debounce_count = KEY_DEBOUNCE_TIME;
            }
            sKey[id].debounce_count--;
            printf("释放消抖时间 %d\r\n", sKey[id].debounce_count);
            if (sKey[id].debounce_count == 0)
            {
                sKey[id].state = KEY_RELEASED;
            }
        }
        if (sKey[id].state == KEY_RELEASED)
        {
            sKey[id].state = NO_KEY;
            key_put_value_to_buffer((enum KEY_STATE)KEY_IS_RELEASED(id));
            printf("按键 %d 释放\r\n", id);
        }
        sKey[id].holding_count = 0;
        sKey[id].repeat_count = 0;
    }
}

/**
 *  @brief  检测按键是否被按下    
 *  @param  key_id： 要检测按键序列号
 *  @return 检测到的按键的状态
 */
static GPIO_PinState key_get_state(uint8_t id)
{    
    sKey[id].pressed = HAL_GPIO_ReadPin(GPIO_Key[id].port, GPIO_Key[id].pin);
    
    return (GPIO_PinState)sKey[id].pressed;
}
