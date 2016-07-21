/**
  *
  * @file       uart.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016.05
  * @brief      ��������
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
        ����Polling Mode(��ѯģʽ)���д����շ�
        ����Ҫ�����շ����ݵ�ʱ���������������
        But ���ϵ���ѯģʽ��˵����CPUЧ�ʵ���
        So  ѧϰ��IT(�ж�ģʽ)&DMAģʽ~ѭ�򽥽�
        ����  2016.05.22
        
        Now ����ITģʽ���д����շ���cube������ж�
        ���̹��ܽ�Ϊ�������̣����ܽ���o(�s���t)o
        ��������ʾ���ʹ�ô���API
        ����  2016.05.22
        
        Now ����DMAģʽ���д����շ� cube���DMA����
        ����
        
        
*/
