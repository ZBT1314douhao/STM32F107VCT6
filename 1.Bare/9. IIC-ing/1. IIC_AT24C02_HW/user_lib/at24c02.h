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

void at24c02_write(uint8_t addr, uint8_t *p_data, uint8_t size);
void at24c02_read(uint8_t addr, uint8_t *p_data, uint8_t size);

#endif

