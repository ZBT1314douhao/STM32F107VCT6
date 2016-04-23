/**
  *
  * @file    gui_clock.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016 - 04
  * @brief   
  *
  */  

#include "gui_clock.h"
#include "clock.h"
#include "gui.h"
#include "delay.h"
#include <math.h>

#define PI  3.1415926

const struct CIRCLE_INIT circle[] = 
{
    150, 120, 72,       /**< 内圆 */
    150, 120, 81,       /**< 外圆 */
};

static uint16_t coor_x_hour, coor_y_hour;
static uint16_t coor_x_min, coor_y_min; 
static uint16_t coor_x_sec, coor_y_sec;

static void set_clock_pointer(void);
static void clear_clock_sec_pointer(void);
static void clear_clock_min_pointer(void);
static void clear_clock_hour_pointer(void);

/**
 *  @brief  绘制时钟外形
 *  @param  None
 *  @return None
 */
void draw_clock(void)
{    
    uint8_t  i = 0;
    uint16_t coor_x_hour_point, coor_y_hour_point;
    uint16_t coor_x_min_point, coor_y_min_point;
    
    GUI_SetPenSize(12);         /**< 绘制中心点 */
    GUI_DrawPoint(circle[0].center_x, circle[0].center_y);
    for (i = 0; i < 2; i++)
    {
        GUI_DrawCircle(circle[0].center_x, circle[0].center_y, circle[0].raduis);
        GUI_DrawCircle(circle[1].center_x, circle[1].center_y, circle[1].raduis);
    }
  
    for (i = 0; i < 12; i++)        /**< 绘制时刻度 */
    {
        GUI_SetPenSize(8);
        coor_x_hour_point = circle[0].center_x + circle[0].raduis * cos((double)i * PI / 6.00);
        coor_y_hour_point = circle[0].center_y + circle[0].raduis * sin((double)i * PI / 6.00);
        GUI_DrawPoint(coor_x_hour_point, coor_y_hour_point);       
    }
    
    for (i = 0; i < 60; i++)       /**< 绘制分 秒 刻度 */
    {
        GUI_SetPenSize(4);
        GUI_SetBkColor(GUI_BLACK);
        GUI_SetColor(GUI_WHITE);
        coor_x_min_point = circle[0].center_x + circle[0].raduis * cos((double)i * PI / 30.00);
        coor_y_min_point = circle[0].center_y + circle[0].raduis * sin((double)i * PI / 30.00);
        GUI_DrawPoint(coor_x_min_point, coor_y_min_point);
    }
}

/**
 *  @brief  绘制时钟外形
 *  @param  None
 *  @return None
 */
void display_clock()
{
    int8_t sec, min, hour;
    
    delay_n_ms(800);
    clear_clock_sec_pointer();
    clear_clock_min_pointer();
    clear_clock_hour_pointer();
    sec  = get_clock_sec();
    min  = get_clock_min();
    hour = get_clock_hour();
    /**< 坐标补偿 因时钟坐标和所建立的坐标有90度的相位差 */
    sec -= 14;
    min -= 15;
    hour -= 3;
    coor_x_sec  = circle[0].center_x + circle[0].raduis * cos((double)sec  * PI / 30.00);
    coor_y_sec  = circle[0].center_y + circle[0].raduis * sin((double)sec  * PI / 30.00);
    coor_x_min  = circle[0].center_x + circle[0].raduis * cos((double)min  * PI / 30.00);
    coor_y_min  = circle[0].center_y + circle[0].raduis * sin((double)min  * PI / 30.00);
    coor_x_hour = circle[0].center_x + circle[0].raduis * cos((double)hour * PI / 6.00);
    coor_y_hour = circle[0].center_y + circle[0].raduis * sin((double)hour * PI / 6.00);
    
    set_clock_pointer();
}

/**< ------------------------ 私有函数 ---------------------------------------- */
/**
 *  @brief  绘制时钟指针
 *  @param  None
 *  @return None
 */
static void set_clock_pointer(void)
{
    /**< 小时 指针 */
    GUI_SetPenSize(2);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_GREEN);
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_hour, coor_y_hour);  
    /**< 分钟 指针 */
    GUI_SetPenSize(2);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_CYAN);
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_min, coor_y_min);    
    /**< 秒 指针 */
    GUI_SetPenSize(1);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_WHITE);
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_sec, coor_y_sec); 
}

/**
 *  @brief  清除时钟走过的轨迹 时
 *  @param  None
 *  @return None
 */
static void clear_clock_hour_pointer(void)
{
    /**< 小时 指针 */
    GUI_SetPenSize(4);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_hour, coor_y_hour);  
}

/**
 *  @brief  清除时钟走过的轨迹 分
 *  @param  None
 *  @return None
 */
static void clear_clock_min_pointer(void)
{
    /**< 分钟 指针 */
    GUI_SetPenSize(2);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_min, coor_y_min);    

}

/**
 *  @brief  清除时钟走过的轨迹 秒
 *  @param  None
 *  @return None
 */
static void clear_clock_sec_pointer(void)
{   
    /**< 秒 指针 */
    GUI_SetPenSize(1);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_sec, coor_y_sec); 
}





