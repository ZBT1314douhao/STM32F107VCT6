/**
  * @file  uart_dup.h
  * @brief �����ϲ����
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       ���δ���
  *
  */ 

#ifndef _UART_DUP_H
#define _UART_DUP_H

/** ͷ�ļ������� ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** ���к�(���Ͷ���) -------------------------------------------- */

/** ö�١��ṹ�� ------------------------------------------------ */ 

/** ���к���ԭ�� ------------------------------------------------ */
void    uart1_send_data(void); 
uint8_t uart1_get_recv_cmd(void);


#endif

