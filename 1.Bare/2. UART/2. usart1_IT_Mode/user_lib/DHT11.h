/**
* DHT11处理头文件
*/

#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f1xx_hal.h"

 #define DHT11_ON()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
 #define DHT11_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)
 #define DHT11_READ  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)
 
void DHT11_READ_DATA(void);




#endif
