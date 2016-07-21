/**
  *
  * @file       gui_clock.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016 - 04
  * @brief      基于STemwin的时钟
  *
  */  

#ifndef __GUI_CLOCK_H
#define __GUI_CLOCK_H

#include "stm32f1xx_hal.h"

void draw_clock(void);
void display_clock(void);
void clear_clock_sec_pointer(void);
void clear_clock_hour_pointer(void);
void clear_clock_min_pointer(void);


struct CIRCLE_INIT
{
    uint16_t center_x;
    uint16_t center_y;
    uint8_t  raduis;
};

#endif
