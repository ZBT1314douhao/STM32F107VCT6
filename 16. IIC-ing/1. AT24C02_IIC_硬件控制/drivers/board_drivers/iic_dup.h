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

/** ö�١��ṹ�� ------------------------------------------------ */ 

/** ���к���ԭ�� ------------------------------------------------ */
void AT24C02_iic_test(void);
HAL_StatusTypeDef AT24C02_write(uint8_t *buffer, uint8_t mem_addr, uint16_t num_to_write);
HAL_StatusTypeDef AT24C02_read(uint8_t *buffer, uint8_t mem_addr, uint16_t num_to_read);

#endif

