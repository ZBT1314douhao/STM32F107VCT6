/**
  *
  * @file    gui_game.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016- 04
  * @brief   lcd实现简单的游戏
  *
  */  

#include "gui_game.h"
#include "gui.h"
#include "delay.h"

#define LEFT_BOUNDARY	    4
#define RIGHT_BOUNDARY	    320
#define UP_BOUNDARY		    4
#define DOWN_BOUNDARY	    240

struct BALL_COORDINATE static ball_coor[] = 
{   /**< 球的坐标 和 半径 */
    20, 40, 4,
};

static enum   BALL_MOVE_DIRECTION ball_mov_dir = ANTICLOCKWISE_UP;

static void draw_ball(void);
static void clockwise_ball_up_move(void);
static void clockwise_ball_down_move(void);
static void clockwise_ball_right_move(void);
static void clockwise_ball_left_move(void);
static void anticlockwise_ball_up_move(void);
static void anticlockwise_ball_down_move(void);
static void anticlockwise_ball_right_move(void);
static void anticlockwise_ball_left_move(void);
static void clear_ball(uint16_t coor_x, uint16_t coor_y);

/**
 *  @brief  小球运动状态转移函数
 *  @param  None
 *  @return None
 */
void ball_start_move(void)
{
    switch (ball_mov_dir)
    {
    case ANTICLOCKWISE_UP:
        anticlockwise_ball_up_move();
        break;
    case ANTICLOCKWISE_DOWN:
        anticlockwise_ball_down_move();
        break;
    case ANTICLOCKWISE_LEFT:
        anticlockwise_ball_left_move();
        break;
    case ANTICLOCKWISE_RIGHT:
        anticlockwise_ball_right_move();
        break;
    case CLOCKWISE_UP:
        clockwise_ball_up_move();
        break;
    case CLOCKWISE_DOWN:
        clockwise_ball_down_move();
        break;
    case CLOCKWISE_LEFT:
        clockwise_ball_left_move();
        break;
    case CLOCKWISE_RIGHT:
        clockwise_ball_right_move();
        break;
    default: 
        break;
    }
}

/**< ------------------------------ 私有函数 ---------------------------------*/ 

/**
 *  @brief  小球顺时针方向下移
 *  @param  None
 *  @return None
 */
static void clockwise_ball_down_move(void)
{
    uint16_t tmp_x, tmp_y;
    
    draw_ball();
    tmp_x = ball_coor[0].x;
    tmp_y = ball_coor[0].y;
    delay_n_us(500);
    ball_coor[0].x -= 1;
    ball_coor[0].y += 1;
    
    clear_ball(tmp_x, tmp_y);
    
    if (ball_coor[0].y >= DOWN_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_LEFT;
    }

    if (ball_coor[0].x <= LEFT_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_DOWN;
    }

}

/**
 *  @brief  小球顺时针方向上移
 *  @param  None
 *  @return None
 */
static void clockwise_ball_up_move(void)
{
    uint16_t tmp_x, tmp_y;
    
    draw_ball();
    tmp_x = ball_coor[0].x;
    tmp_y = ball_coor[0].y;
    delay_n_us(500);
    ball_coor[0].x += 1;
    ball_coor[0].y -= 1;
    
    clear_ball(tmp_x, tmp_y);
    
    if (ball_coor[0].y <= UP_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_RIGHT;
    }

    if (ball_coor[0].x >= RIGHT_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_UP;
    }

}

/**
 *  @brief  小球顺时针方向左移
 *  @param  None
 *  @return None
 */
static void clockwise_ball_left_move(void)
{
    uint16_t tmp_x, tmp_y;
    
    draw_ball();
    tmp_x = ball_coor[0].x;
    tmp_y = ball_coor[0].y;
    delay_n_us(500);
    ball_coor[0].x -= 1;
    ball_coor[0].y -= 1;
    
    clear_ball(tmp_x, tmp_y);
    
    if (ball_coor[0].x <= LEFT_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_UP;
    }

    if (ball_coor[0].y <= UP_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_LEFT;
    }

}

/**
 *  @brief  小球顺时针方向右移
 *  @param  None
 *  @return None
 */
static void clockwise_ball_right_move(void)
{
    uint16_t tmp_x, tmp_y;
    
    draw_ball();
    tmp_x = ball_coor[0].x;
    tmp_y = ball_coor[0].y;
    delay_n_us(500);
    ball_coor[0].x += 1;
    ball_coor[0].y += 1;
    
    clear_ball(tmp_x, tmp_y);
    
    if (ball_coor[0].x >= RIGHT_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_DOWN;
    }

    if (ball_coor[0].y >= DOWN_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_RIGHT;
    }

}

/**
 *  @brief  小球逆时针方向下移
 *  @param  None
 *  @return None
 */
static void anticlockwise_ball_down_move(void)
{
    uint16_t tmp_x, tmp_y;
    
    draw_ball();
    tmp_x = ball_coor[0].x;
    tmp_y = ball_coor[0].y;
    delay_n_us(500);
    ball_coor[0].x += 1;
    ball_coor[0].y += 1;
    
    clear_ball(tmp_x, tmp_y);
    
    /**< 触碰到底部边界后 向右反弹 */
    if (ball_coor[0].y >= DOWN_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_RIGHT;
    }
    if (ball_coor[0].x >= RIGHT_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_DOWN;
    }


}

/**
 *  @brief  小球逆时针方向右移
 *  @param  None
 *  @return None
 */
static void anticlockwise_ball_right_move(void)
{
    uint16_t tmp_x, tmp_y;
    
    draw_ball();
    tmp_x = ball_coor[0].x;
    tmp_y = ball_coor[0].y;
    delay_n_us(500);
    ball_coor[0].x += 1;
    ball_coor[0].y -= 1;
    clear_ball(tmp_x, tmp_y);
    
    if (ball_coor[0].x >= RIGHT_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_UP;
    }

    if (ball_coor[0].y <= UP_BOUNDARY )
    {
        ball_mov_dir = CLOCKWISE_RIGHT;
    }

}

/**
 *  @brief  小球逆时针方向上移
 *  @param  None
 *  @return None
 */
static void anticlockwise_ball_up_move(void)
{
    uint16_t tmp_x, tmp_y;
    
    draw_ball();
    tmp_x = ball_coor[0].x;
    tmp_y = ball_coor[0].y;
    delay_n_us(800);
    ball_coor[0].x -= 1;
    ball_coor[0].y -= 1;    
    clear_ball(tmp_x, tmp_y);
    
    if (ball_coor[0].y <= UP_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_LEFT;
    }
    if (ball_coor[0].x <= LEFT_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_UP;
    }

}

/**
 *  @brief  小球逆时针方向左移
 *  @param  None
 *  @return None
 */
static void anticlockwise_ball_left_move(void)
{
    uint16_t tmp_x, tmp_y;
    
    draw_ball();
    tmp_x = ball_coor[0].x;
    tmp_y = ball_coor[0].y;
    delay_n_us(800);
    ball_coor[0].x -= 1;
    ball_coor[0].y += 1;    
    clear_ball(tmp_x, tmp_y); 
    
    if (ball_coor[0].x <= LEFT_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_DOWN;
    }

    if (ball_coor[0].y >= DOWN_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_LEFT;
    }
   
}

/**
 *  @brief  绘制小球大小
 *  @param  None
 *  @return None
 */
static void draw_ball(void)
{
    GUI_SetPenSize(12);
    GUI_SetColor(GUI_RED);
    GUI_SetBkColor(GUI_BLACK);
    GUI_FillCircle(ball_coor[0].x, ball_coor[0].y, ball_coor[0].raduis);
}

/**
 *  @brief  清除小球轨迹
 *  @param  None
 *  @return None
 */
static void clear_ball(uint16_t coor_x, uint16_t coor_y)
{
    GUI_SetPenSize(12);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_BLACK);
    GUI_FillCircle(coor_x, coor_y, ball_coor[0].raduis);
}
