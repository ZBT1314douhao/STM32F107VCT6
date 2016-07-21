/** 
 *  @file       key.h
 *  @brief      按键驱动程序
 *  @author     王晓荣   
 *  @version    
 *  @date       2014-08-29 
 */
 
#ifndef	__KEY_H
#define	__KEY_H 

#include "stm32f1xx_hal.h"

enum KEY
{
    LEFT_SHIFT, RIGHT_SHIFT, NO_KEY = 0xff,
}; 

struct GPIO_KEY
{
    GPIO_TypeDef    *port;
    uint16_t        pin;
};
 
enum KEY key_read(void);


#endif 
