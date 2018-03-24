/**
  * @file  at24c02.h
  * @brief at24c02��������
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       ���δ���
  *
  */ 

#ifndef _AT24C02__H
#define _AT24C02__H

/** ͷ�ļ������� ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** ���к�(���Ͷ���) -------------------------------------------- */

/** ö�١��ṹ�� ------------------------------------------------ */ 

/** ���к���ԭ�� ------------------------------------------------ */
void AT24C02_iic_test(void);
void AT24C02_write_data(uint8_t *write_data, uint16_t start_addr, uint16_t data_length);
void AT24C02_read_data(uint8_t *read_data, uint16_t start_addr, uint16_t data_length);

#endif

