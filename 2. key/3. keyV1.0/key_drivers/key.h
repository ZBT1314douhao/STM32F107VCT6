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


enum KEY_ID
{
    TAMPER, WAKEUP,

	NO_KEY = 0xff,	    /**< 无按键按下 */
};

enum KEY_STATE
{
	/** 单独的按键 按下、抬起、保持(长按)状态 */
    LEFT_PRESSED,  LEFT_RELEASED,  LEFT_HOLD,
	RIGHT_PRESSED, RIGHT_RELEASED, RIGHT_HOLD,

	/** 组合键 */

	
}; 

struct GPIO_KEY
{
    GPIO_TypeDef    *port;
    uint16_t        pin;
};
 
struct KEY
{
	GPIO_PinState  state;		/**< 按键状态 按下 or 未按下 */
	
	uint8_t  debounce_count;

	uint16_t hold_count;		/**< 长按计数器 */
	uint16_t hold_time;		    /**< 长按时间 */

	uint8_t repeat_speed;		/**< 连发速度 */
    uint8_t repeat_count;       /**< 连发计数器 */
    
    enum KEY_ID id;             /**< 按键ID号 */
    enum KEY_STATE status;       /**< 按键状态 按下、保持(长按)、释放 */
};

void key_init(void);
struct KEY key_read(void);

GPIO_PinState get_key_state(uint8_t key_id);

#endif 
