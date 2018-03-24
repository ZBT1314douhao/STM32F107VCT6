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
#include "key.h"
#include "gui.h"
#include "delay.h"

#define LEFT_BOUNDARY	             4
#define RIGHT_BOUNDARY	             320
#define UP_BOUNDARY		             4
#define DOWN_BOUNDARY	             192   
#define DEAD_BOUNDARY                240    /**< 原 DOWN_BOUNDARY */
/**< 设定小球移动的相对速度  注意RELATIVE_SPEED_VALUE取值范围 */
#define RELATIVE_SPEED_VALUE         5
/**< 挡板y坐标 */
#define BAFFLE_BOARD_DOWN_BOUNDARY   200

struct BALL_COORDINATE static ball_coor[] = 
{   /**< 球的坐标 和 半径 */
    20, 40, 4,
};
struct BAFFLE_BOARD_COORDINATE static baffle_board[] = 
{   /**< 挡板长度为80 */
    0, 80, BAFFLE_BOARD_DOWN_BOUNDARY,
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
static void set_ball_speed(uint16_t relative_speed_value);

static void draw_baffle_board(void);
static void clear_baffle_board(uint16_t coor_x0, uint16_t coor_x1);
static void baffle_board_left_move(void);
static void baffle_board_right_move(void);
static void ball_start_move(void);

/**
 *  @brief  小球移动和挡板移动函数
 *  @param  None
 *  @return None
 */
void ball_move(void)
{
    switch (key_read())
    {
        case KEY_LEFT_MOVE:
            baffle_board_left_move();
            break;
        case KEY_RIGHT_MOVE:
            baffle_board_right_move();
            break;
        default:
            ball_start_move();
                draw_baffle_board();
        break;
    }
}

/**
 *  @brief  小球运动状态转移函数
 *  @param  None
 *  @return None
 */
static void ball_start_move(void)
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
    set_ball_speed(RELATIVE_SPEED_VALUE);
    ball_coor[0].x -= 1;
    ball_coor[0].y += 1;
    
    clear_ball(tmp_x, tmp_y);
    
    /**< 如果碰到挡板 */
    if (ball_coor[0].y >= DOWN_BOUNDARY && (baffle_board[0].x0 <= ball_coor[0].x && ball_coor[0].x <= baffle_board[0].x1))   
    {
        ball_mov_dir = CLOCKWISE_LEFT;
    }
    else if (ball_coor[0].y >= DOWN_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_DOWN;
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
    set_ball_speed(RELATIVE_SPEED_VALUE);
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
    set_ball_speed(RELATIVE_SPEED_VALUE);
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
    set_ball_speed(RELATIVE_SPEED_VALUE);
    ball_coor[0].x += 1;
    ball_coor[0].y += 1;
    
    clear_ball(tmp_x, tmp_y);
    
    if (ball_coor[0].x >= RIGHT_BOUNDARY)
    {
        ball_mov_dir = CLOCKWISE_DOWN;
    }

    if (ball_coor[0].y >= DOWN_BOUNDARY && (baffle_board[0].x0 <= ball_coor[0].x && ball_coor[0].x <= baffle_board[0].x1))
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
    set_ball_speed(RELATIVE_SPEED_VALUE);
    ball_coor[0].x += 1;
    ball_coor[0].y += 1;
    
    clear_ball(tmp_x, tmp_y);
    
    /**< 触碰到底部挡板后 向右反弹 */
    if (ball_coor[0].y >= DOWN_BOUNDARY && (baffle_board[0].x0 <= ball_coor[0].x && ball_coor[0].x <= baffle_board[0].x1))
    {
        ball_mov_dir = ANTICLOCKWISE_RIGHT;
    }
    else if (ball_coor[0].y >= DOWN_BOUNDARY)   /**< 否则 继续向下走向死亡边界 */
    {
        ball_mov_dir = ANTICLOCKWISE_DOWN;
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
    set_ball_speed(RELATIVE_SPEED_VALUE);
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
    set_ball_speed(RELATIVE_SPEED_VALUE);
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
    set_ball_speed(RELATIVE_SPEED_VALUE);
    ball_coor[0].x -= 1;
    ball_coor[0].y += 1;    
    clear_ball(tmp_x, tmp_y); 
    
    if (ball_coor[0].x <= LEFT_BOUNDARY)
    {
        ball_mov_dir = ANTICLOCKWISE_DOWN;
    }

    if (ball_coor[0].y >= DOWN_BOUNDARY && (baffle_board[0].x0 <= ball_coor[0].x && ball_coor[0].x <= baffle_board[0].x1))
    {
        ball_mov_dir = CLOCKWISE_LEFT;
    } 
}


/**
 *  @brief  底部挡板左移
 *  @param  None
 *  @return None
 */
static void baffle_board_left_move(void)
{
    uint16_t tmp_x0, tmp_x1;
    
    draw_baffle_board();
    tmp_x0 = baffle_board[0].x0;
    tmp_x1 = baffle_board[0].x1;  
    if (baffle_board[0].x0 <= LEFT_BOUNDARY)
    {
        baffle_board[0].x0 -= 0;
        baffle_board[0].x1 -= 0;
    }
    else
    {
        baffle_board[0].x0 -= 10;
        baffle_board[0].x1 -= 10;
    }   
    clear_baffle_board(tmp_x0, tmp_x1);
    
}

/**
 *  @brief  底部挡板右移
 *  @param  None
 *  @return None
 */
static void baffle_board_right_move(void)
{
    uint16_t tmp_x0, tmp_x1;
    
    draw_baffle_board();
    tmp_x0 = baffle_board[0].x0;
    tmp_x1 = baffle_board[0].x1;   
    if (baffle_board[0].x1 >= RIGHT_BOUNDARY)
    {
        baffle_board[0].x0 -= 0;
        baffle_board[0].x1 -= 0;
    }
    else
    {
        baffle_board[0].x0 += 10;
        baffle_board[0].x1 += 10;
    } 
    clear_baffle_board(tmp_x0, tmp_x1);
}


/**
 *  @brief  设定小球运动的速度
 *  @param  None
 *  @return None
 */
static void set_ball_speed(uint16_t relative_speed_value)
{
    if (1 <= relative_speed_value && relative_speed_value <= 10)
    {
        delay_n_ms(relative_speed_value);
    }
    else if(11 <= relative_speed_value && relative_speed_value <= 999)
    {
        delay_n_us(relative_speed_value);
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

/**
 *  @brief  绘制挡板
 *  @param  None
 *  @return None
 */
static void draw_baffle_board(void)
{
    GUI_SetPenSize(8);
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(GUI_BLACK);
    GUI_DrawLine(baffle_board[0].x0, baffle_board[0].coor_y, baffle_board[0].x1, baffle_board[0].coor_y); 
}

/**
 *  @brief  清楚挡板
 *  @param  None
 *  @return None
 */
static void clear_baffle_board(uint16_t coor_x0, uint16_t coor_x1)
{
    GUI_SetPenSize(8);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_BLACK);
    GUI_DrawLine(coor_x0, baffle_board[0].coor_y, coor_x1, baffle_board[0].coor_y); 
}
