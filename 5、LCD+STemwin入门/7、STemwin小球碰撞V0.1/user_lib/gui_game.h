/**
  *
  * @file       gui_game.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016 - 04
  * @brief      lcdʵ�ּ򵥵���Ϸ
  *
  */  

#ifndef __GUI_GAME_H
#define __GUI_GAME_H

#include "stm32f1xx_hal.h"


enum   BALL_MOVE_DIRECTION
{   /**< ��ʱ�� & ˳ʱ�� ���������ƶ� */
    ANTICLOCKWISE_UP, ANTICLOCKWISE_DOWN, ANTICLOCKWISE_LEFT, ANTICLOCKWISE_RIGHT,
     CLOCKWISE_UP, CLOCKWISE_DOWN, CLOCKWISE_LEFT, CLOCKWISE_RIGHT,
};

struct BALL_COORDINATE
{
    uint16_t x;
    uint16_t y;
    uint8_t  raduis;
};

void ball_start_move(void);

#endif
