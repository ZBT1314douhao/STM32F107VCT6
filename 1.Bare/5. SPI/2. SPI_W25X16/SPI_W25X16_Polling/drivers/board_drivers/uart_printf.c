/**
  * @file    uart_printf.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016.05
  * @brief   uart1串口重定向 无对应头文件
  *
  */  

#include "stm32f1xx_hal.h"

#define TIME_OUT        0XFFFF
  
extern UART_HandleTypeDef huart1;


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  ch： 
  * @param  f： 
  * @retval None
  * @note   用于printf输出数据到串口
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, TIME_OUT);

  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  f： 
  * @retval ch： 
  * @note   可以从串口1输入数据
  */
int fgetc(FILE *f)
{
    int ch;
    
    HAL_UART_Receive(&huart1, (uint8_t*)&ch, 1, TIME_OUT);
//    putchar(ch);            /**< 回显 */
    
    return ch;
}
