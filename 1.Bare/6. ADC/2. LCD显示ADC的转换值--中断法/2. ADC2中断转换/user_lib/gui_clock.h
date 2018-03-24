/**
  *
  * @file       gui_clock.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016 - 04
  * @brief      ����STemwin��ʱ��
  *
  */  

#ifndef __GUI_CLOCK_H
#define __GUI_CLOCK_H

#include "stm32f1xx_hal.h"

void draw_clock(void);
void display_clock(void);

struct CIRCLE_INIT
{
    uint16_t center_x;
    uint16_t center_y;
    uint8_t  raduis;
};

#endif
