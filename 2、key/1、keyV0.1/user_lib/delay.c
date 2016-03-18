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
 *  @brief  ´ÖÂÔÑÓÊ±n us
 *  @param  None
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
 *  @brief  ´ÖÂÔÑÓÊ±n ms
 *  @param  None
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


