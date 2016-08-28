/**
 * @file    ad7705.h
 * @author  zbt
 * @version v0.01
 * @date    2016-07-11
 * @brief   ad7705驱动程序
 */
 
#ifndef __AD7705_H
#define __AD7705_H

#include "stm32f1xx_hal.h"

void AD7705_init(void);
uint16_t AD7705_get_adc_value(uint8_t channel);

void AD7705_self_calibration(uint8_t channel);
void AD7705_system_calibration_zero(uint8_t channel);
void AD7705_system_calibration_full(uint8_t channel);

#endif 
