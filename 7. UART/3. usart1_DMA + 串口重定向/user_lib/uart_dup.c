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
#include "delay.h"

#define UART_BUFF_SIZE		    1
#define PRINTF_BUFF_SIZE		10

static uint8_t uart_recv_cmd;
static uint8_t uart_tx_cmd = 0x01;
//static uint8_t uart_tx_cmd[] = "STM32";

extern UART_HandleTypeDef huart1;


/**
 *  @brief  串口发送uart_tx_buff[]的数据到PC 仅作演示
 *  @param  None
 *  @retval None
 */
void uart1_send_data(void) 
{
    /**< 可以选择同时发送多个或者一个数据 */
    HAL_UART_Transmit_DMA(&huart1, &uart_tx_cmd, UART_BUFF_SIZE);
//    HAL_UART_Transmit_DMA(&huart1, uart_tx_cmd, PRINTF_BUFF_SIZE);
    delay_n_ms(10);
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


//#if 1
//#pragma import(__use_no_semihosting)

//struct __FILE
//{
//    int handle;
//};

//FILE __stdout;

///**< 定义_sys_exit() 避免使用半主机模式 */
//void _sye_exti(int x)
//{
//    x = x;
//}



/**
 * @brief   重写fputc实现串口重定向 
 * @note    标准库函数默认输出设备是显示器 要实现在串口或者lcd输出 需要重定向
            选择使用微库Options-Target
 * @param   [in]ch  要发送的字符
 * @param   [in]FILE *f 
 * @retval  None
 */
int fputc(int ch, FILE *f)
{

     if (ch == '\r')    /**< 用于换行 */
    {
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)'\r', 1);
        delay_n_ms(5);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)'\n', 1);
        /**< 似乎不同的串口助手 需要的延时不太一样 不过这个长度够长了  可以减少*/
        delay_n_ms(100);        
        while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);
    }
    else
    {
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ch, PRINTF_BUFF_SIZE);
        delay_n_ms(100);
        while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);
    }
         
    return ch;
}



//#endif
