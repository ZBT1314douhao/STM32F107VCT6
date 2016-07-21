/**
  *
  * @file    delay.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016-
  * @brief   
  *
  */  

#include "delay.h"

/**
 *  @brief   延时n_us
 *  @param  [in]n_us	粗略延时
 *  @return None
 */
void delay_n_us(uint32_t n_us)
{
    uint8_t i = 0;

    while (n_us--)
    {
        i = 10;
        while (i--);
    }
}

/**
 *  @brief  延时n_ms
 *  @param  [in] n_ms 粗略延时
 *  @return None
 */
void delay_n_ms(uint32_t n_ms)
{
    uint16_t i = 0;

    while (n_ms--)
    {
        i = 12000;
        while (i--);
    }
}

/**
*  @brief  粗略的延时ms
 *  @param [in] ms 粗略的延时
 *  @return None
 */
void delay_ms(uint16_t ms)
{
	unsigned int i;  
    
	while(ms--) 
	{
		for(i = 0; i < 10000; i++);
	}	
}


