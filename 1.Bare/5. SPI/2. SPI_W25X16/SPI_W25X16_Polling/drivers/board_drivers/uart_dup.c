/**
  * @file  uart_dup.c
  * @brief 串口上层程序
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       初次创建
  *
  */ 

/** 头文件包含区 ------------------------------------------------ */
#include "bsp.h"

/** 私有宏(类型定义) -------------------------------------------- */ 
#define UART_BUFF_SIZE		    1
#define PRINTF_BUFF_SIZE		10

/** 私有变量 --------------------------------------------------- */
static uint8_t uart_recv_cmd;
static uint8_t uart_tx_cmd = 0x01;
//static uint8_t uart_tx_cmd[] = "STM32";

/** 外部变量 --------------------------------------------------- */
extern UART_HandleTypeDef huart1;

/** 私有函数原型 ----------------------------------------------- */

/** 公有函数 --------------------------------------------------- */
/**
  * @brief  串口发送uart_tx_buff[]的数据到PC 仅作演示
  * @param  None
  * @retval None
  */
void uart1_send_data(void) 
{
    /**< 可以选择同时发送多个或者一个数据 */
    HAL_UART_Transmit_DMA(&huart1, &uart_tx_cmd, UART_BUFF_SIZE);
//    HAL_UART_Transmit_DMA(&huart1, uart_tx_cmd, PRINTF_BUFF_SIZE);
//    delay_n_ms(10);
    HAL_Delay(10);
}

/**
  * @brief  获取串口从PC接收到的数据
  * @param  None
  * @retval None
  */
uint8_t uart1_get_recv_cmd(void)
{
    uint8_t   tmp_cmd;  

	HAL_UART_Receive_DMA(&huart1, &uart_recv_cmd, UART_BUFF_SIZE);
    tmp_cmd = uart_recv_cmd; 
    uart_recv_cmd = 0;   

    return tmp_cmd; 	
}	

/** 串口重定向 ------------------------------------------------- */
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
  * @brief  重写fputc实现串口重定向 
  * @note   标准库函数默认输出设备是显示器 要实现在串口或者lcd输出 需要重定向
            选择使用微库Options-Target
  * @param  ch  要发送的字符
  * @param  FILE *f 
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    /** 循环发送字符串直到传输完毕 */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);     
    
    return ch;
}

//#endif

/** 串口重定向 ------------------------------------------------- */

/** 私有函数 --------------------------------------------------- */
