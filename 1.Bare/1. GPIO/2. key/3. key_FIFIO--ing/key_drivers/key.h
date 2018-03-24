/** 
 *  @file       key.h
 *  @brief      按键驱动程序
 *  @author     zbt   
 *  @version    V1.0
 *  @date       2016-08-21 
 */
 
#ifndef	__KEY_H
#define	__KEY_H 

#include "stm32f1xx_hal.h"

/** 单位 10ms */
#define KEY_DEBOUNCE_TIME		2		/**< 去抖动时间 */
#define KEY_PRESSED_TIME        2
#define KEY_HOLDING_TIME		200	/**< 确认为长按的保持时间 */

#define KEY_FIFO_SIZE       10

struct GPIO_KEY
{
    GPIO_TypeDef    *port;
    uint16_t        pin;
};

enum KEY_ID
{
    TAMPER, WAKEUP,
};

enum KEY_STATE
{
    NO_KEY,
	/** 单独的按键 按下、抬起、保持(长按)状态 */
    RIGHT_PRESSED, RIGHT_HOLDING, RIGHT_RELEASED,
    LEFT_PRESSED, LEFT_HOLDING, LEFT_RELEASED,

	/** 组合键 */	
}; 
 
struct KEY
{
    uint8_t pressed;            /**< 按键按下 & 未按下状态 */
	uint8_t state;		        /**< 按键过程状态 按下 保持 释放 */
    
	uint8_t debounce_count;     /**< 按键等待计数 */
    uint8_t holding_count;
    uint16_t holding_time;		/**< 长按时间 */
    uint8_t repeat_count;       /**< 连发计数器 */
	uint8_t repeat_speed;		/**< 连发速度 */    
};

struct KEY_FIFO
{
    uint8_t buffer[KEY_FIFO_SIZE];
    uint8_t read;
    uint8_t write;
};

void key_fifo_test(void);

void key_init(void);
void key_scan(void);
void key_put_value_to_buffer(enum KEY_STATE eKeyValue);
enum KEY_STATE key_get_value_from_buffer(void);

#endif 
