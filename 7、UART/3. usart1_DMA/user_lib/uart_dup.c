/**
  *
  * @file    uart.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016.05
  * @brief   串口驱动
  *
  */  

#include "uart_dup.h"


#define UART_BUFF_SIZE		1

static uint8_t uart_recv_cmd;
static uint8_t uart_tx_cmd = 0x01;

extern UART_HandleTypeDef huart1;


/**
 *  @brief  串口发送uart_tx_buff[]的数据到PC 仅作演示
 *  @param  None
 *  @retval None
 */
void uart1_send_data(void) 
{
    HAL_UART_Transmit_DMA(&huart1, &uart_tx_cmd, UART_BUFF_SIZE);
}

/**
 * @brief   获取串口从PC接收到的数据
 * @param   None
 * @retval  None
 */
uint8_t uart1_get_recv_cmd(void)
{
    uint8_t   tmp_cmd;  

	HAL_UART_Receive_DMA(&huart1, &uart_recv_cmd, UART_BUFF_SIZE);
    tmp_cmd = uart_recv_cmd; 
    uart_recv_cmd = 0;   

    return tmp_cmd; 	
}	



