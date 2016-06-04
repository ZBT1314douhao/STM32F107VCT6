/**
  *
  * @file       uart.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016.05
  * @brief      串口驱动
  *
  */  

#ifndef _UART_DUP_H
#define _UART_DUP_H

#include "stm32f1xx_hal.h"

void    uart1_send_data(void); 
uint8_t uart1_get_recv_cmd(void);

#endif

/**< ---------------------- Log ------------------------------------ */
/**<
        采用Polling Mode(轮询模式)进行串口收发
        在需要串口收发数据的时候调用上述俩函数
        But 酱紫的轮询模式据说导致CPU效率低下
        So  学习下IT(中断模式)&DMA模式~循序渐进
        以上  2016.05.22
*/
