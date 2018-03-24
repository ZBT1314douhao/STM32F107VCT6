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

static enum     GAME_STATE game_state = GAME_START;
static uint8_t  x_dir_flag = 0, y_dir_flag = 0;
static uint8_t  current_score = 0;

static void draw_ball(void);
static void clear_ball(uint16_t coor_x, uint16_t coor_y);
static void set_ball_speed(uint16_t relative_speed_value);

static void draw_baffle_board(void);
static void clear_baffle_board(uint16_t coor_x0, uint16_t coor_x1);
static void baffle_board_left_move(void);
static void baffle_board_right_move(void);

static void game_init(void);
static void game_over(void);
static void ball_move(void);
static void ball_move_dir(void);
static void ball_start_move(void);

void start_game(void)
{
    switch (game_state)
    {
    case GAME_START:
        game_start_screen();
        break;
    case GAME_RUN:
        ball_move();
        break;
    case GAME_END:
        game_over();
        break;
    default:
        break;
    }
}

/**
 *  @brief  起始画面
 *  @param  None
 *  @return None
 */
void game_start_screen(void)
{
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font32B_ASCII); 
    GUI_DispStringAt("Welcome", 80, 80);
    GUI_SetFont(&GUI_Font8x16); 
    GUI_DispStringAt("(Start game)? Press key1 : do nothing", 10, 120);
    GUI_SetFont(&GUI_Font8x16); 
    GUI_DispStringAt("By ZBT", 180, 160);
 
    if (key_read() == KEY_LEFT_MOVE)
    {
        GUI_Clear();
        game_init();
        game_state = GAME_RUN;
    }
} 

/**
 *  @brief  小球移动和挡板移动函数
 *  @param  None
 *  @return None
 */
static void ball_move(void)
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
 *  @brief  小球运动
 *  @param  None
 *  @return None
 */
static void ball_start_move(void)
{
    /**< 显示得分 */
    GUI_DispStringAt("Score:", 240, 20);
    GUI_DispDecAt(current_score, 290, 20, 2);
    draw_ball();
    set_ball_speed(400);
    clear_ball(ball_coor[0].x, ball_coor[0].y);
    
    if (ball_coor[0].x <= LEFT_BOUNDARY)
    {
        x_dir_flag = 0;
    }
    else if(ball_coor[0].x >= RIGHT_BOUNDARY)
    {
        x_dir_flag = 1;
    }

    if(ball_coor[0].y <= UP_BOUNDARY)
    {
        y_dir_flag = 0;
    }
    else if(ball_coor[0].y >= DOWN_BOUNDARY)
    {
        y_dir_flag = 1;
    }
    /**< 判断是否碰到挡板 */
    if (ball_coor[0].y < DOWN_BOUNDARY)
    {
        ball_move_dir();
    }
    else if (ball_coor[0].y >= DOWN_BOUNDARY)
    {
        if (baffle_board[0].x0 > ball_coor[0].x || ball_coor[0].x > baffle_board[0].x1)
        {
            game_state = GAME_END;           
        }
        else
        {
            current_score += 1;
            ball_move_dir();
        }
    }
}

/**
 *  @brief  小球移动方向选择
 *  @param  None
 *  @return None
 */
static void ball_move_dir(void)
{
	switch (x_dir_flag)
	{
	case 0:
		ball_coor[0].x += 1;
		break;
	case 1:
		ball_coor[0].x -= 1;
		break;
	default :
		break;
	}

	switch (y_dir_flag)
	{
	case 0:
		ball_coor[0].y += 1;
		break;
	case 1:
		ball_coor[0].y -= 1;
		break;
	default :
		break;
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
 *  @brief  清除挡板
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

/**
 *  @brief  游戏结束画面
 *  @param  None
 *  @return None
 */
static void game_over(void)
{
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font32B_ASCII); 
    GUI_DispStringAt("Game over", 80, 120);
    GUI_SetFont(&GUI_Font8x16); 
    GUI_DispStringAt("(Try again)? Press key1 : do nothing", 18, 160);
   
    if (key_read() == KEY_LEFT_MOVE)
    {
        GUI_Clear();
        game_state = GAME_START;
    }
}

/**
 *  @brief  设定小球运动的速度
 *  @param  None
 *  @return None
 */
static void game_init(void)
{
    /**< 小球复位 */
    ball_coor[0].x = 20;
    ball_coor[0].y = 40;
    /**< 挡板复位 */
    baffle_board[0].x0 = 0;
    baffle_board[0].x1 = 80;
    baffle_board[0].coor_y = BAFFLE_BOARD_DOWN_BOUNDARY;
    /**< 得分复位 */
    current_score = 0;
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
