/** 
 *  @file       key.h
 *  @brief      按键驱动程序
 *  @author     王晓荣   
 *  @version    
 *  @date       2016- 
 *  @modified   1、修改NULL = 0xff 改为 NO_KEY = 0xff
 */
 
#ifndef	__KEY_H
#define	__KEY_H 

#include "stm32f1xx_hal.h"

enum KEY
{
    KEY_CURSOR, KEY_ADJ, NO_KEY = 0xff,
}; 

struct GPIO_KEY
{
    GPIO_TypeDef    *port;
    uint16_t        pin;
};
 
enum KEY key_read(void);


#endif 
