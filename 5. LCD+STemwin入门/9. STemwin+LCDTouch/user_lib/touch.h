/**
 * @file    touch.c
 * @brief   ����������
 * @author  ������    2014-05-01             ���δ���
 * @version V0.0.2    2016.07.10    zbt      �޸ĳ�����
 * @date    2014-05-01
 */
#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "stm32f1xx_hal.h"

										    
//#define PEN  	PCin(6)  	// INT
 
     
uint16_t touch_read_x(void);
uint16_t touch_read_y(void);


uint16_t read_lcd_id(void);																 
		  
#endif


