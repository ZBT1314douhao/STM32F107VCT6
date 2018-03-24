/**
  * @file    uart_printf.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016.05
  * @brief   uart1串口重定向 无对应头文件
  *
  */  

#include "stm32f1xx_hal.h"
#include "stdio.h"

#define TIME_OUT        0xFFFF
  
extern UART_HandleTypeDef huart1;


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, TIME_OUT);

  return ch;
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  * @note   可以使用getchar从串口1输入数据
  */
int fgetc(FILE *f)
{
    int ch;
    
    HAL_UART_Receive(&huart1, (uint8_t*)&ch, 1, TIME_OUT);
    
    return ch;
}
