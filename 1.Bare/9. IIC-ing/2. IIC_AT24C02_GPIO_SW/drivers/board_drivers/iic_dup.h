/**
  * @file  iic_dup.h
  * @brief IIC�ϲ����
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       ���δ���
  *
  */ 

#ifndef _IIC_DUP_H
#define _IIC_DUP_H

/** ͷ�ļ������� ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** ���к�(���Ͷ���) -------------------------------------------- */
#define IIC_WRITE                   0
#define IIC_READ                    1

/** ö�١��ṹ�� ------------------------------------------------ */ 

/** ���к���ԭ�� ------------------------------------------------ */
void iic_start(void);
void iic_stop(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);

void iic_sendbyte(uint8_t byte);
uint8_t iic_readbyte(void);

uint8_t iic_check_device_status(uint8_t device_addr);

#endif

