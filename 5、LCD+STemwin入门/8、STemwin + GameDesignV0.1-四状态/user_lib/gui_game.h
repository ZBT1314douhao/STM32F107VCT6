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

enum GAME_STATE
{
    GAME_START, GAME_RUN, GAME_END,
};

struct BALL_COORDINATE
{
    uint16_t x;
    uint16_t y;
    uint8_t  raduis;
};
/**< 挡板位置坐标 */
struct BAFFLE_BOARD_COORDINATE
{
    uint16_t x0;
    uint16_t x1;
    uint8_t  coor_y;
};


void start_game(void);
void game_start_screen(void);

#endif
