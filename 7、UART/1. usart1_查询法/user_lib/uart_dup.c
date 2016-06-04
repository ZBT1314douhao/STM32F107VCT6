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

#define TX_BUFF_SIZE    (sizeof(uart_tx_buff) / sizeof(uart_tx_buff[0]))
#define TIME_OUT        0x02

static uint8_t uart_recv_cmd;
static uint8_t uart_tx_buff[] = {0x01, 0x02, 0x03, 0x04, };
//static uint8_t uart_rx_buff[8];
    
extern UART_HandleTypeDef huart1;


/**
 *  @brief  串口发送uart_tx_buff[]的数据到PC 仅作演示
 *  @param  None
 *  @retval None
 */
void uart1_send_data(void) 
{
    HAL_UART_Transmit(&huart1, uart_tx_buff, TX_BUFF_SIZE, TIME_OUT);
}

/**
 * @brief   获取串口从PC接收到的数据
 * @param   None
 * @retval  None
 */
uint8_t uart1_get_recv_cmd(void)
{
    uint8_t   tmp_cmd;  

    HAL_UART_Receive(&huart1, (uint8_t*)&uart_recv_cmd, 1, TIME_OUT);
    tmp_cmd = uart_recv_cmd; 
    uart_recv_cmd = 0;   

    return tmp_cmd; 	
}	

