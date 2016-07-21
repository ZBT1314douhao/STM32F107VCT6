/**
  *
  * @file       gui_game.h 
  * @author     Z B T
  * @version    V0.1
  * @date       2016 - 04
  * @brief      lcd实现简单的游戏
  *
  */  

#ifndef __GUI_GAME_H
#define __GUI_GAME_H

#include "stm32f1xx_hal.h"


enum   BALL_MOVE_DIRECTION
{   /**< 逆时针 & 顺时针 上下左右移动 */
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
