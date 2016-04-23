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

#define PI                  3.1415926
/**< 计算时钟刻度间隔 min和sec间隔相同 */
#define COS_HOUR_SPACE(x)   cos((double)x * PI / 6.00)      
#define SIN_HOUR_SPACE(x)   sin((double)x * PI / 6.00)
#define COS_MIN_SPACE(x)    cos((double)x * PI / 30.00)
#define SIN_MIN_SPACE(x)    sin((double)x * PI / 30.00)

const struct CIRCLE_INIT circle[] = 
{
    150, 120, 72,       /**< 内圆 */
    150, 120, 81,       /**< 外圆 */
};

static uint16_t coor_x_hour_pointer, coor_y_hour_pointer;
static uint16_t coor_x_min_pointer, coor_y_min_pointer;
static uint16_t coor_x_sec_pointer, coor_y_sec_pointer;

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
    uint16_t start_x_hour, start_y_hour;    /**< 时钟刻度的起始坐标和截止坐标 */
    uint16_t start_x_min, start_y_min;
    uint16_t end_x_hour, end_y_hour;;
    uint16_t end_x_min, end_y_min;
    
    GUI_SetPenSize(12);         /**< 绘制中心点 */
    GUI_DrawPoint(circle[0].center_x, circle[0].center_y);
    for (i = 0; i < 2; i++)
    {   /**< 绘制时钟内外圆 */
        GUI_DrawCircle(circle[0].center_x, circle[0].center_y, circle[0].raduis);
        GUI_DrawCircle(circle[1].center_x, circle[1].center_y, circle[1].raduis);
    }
  
    for (i = 0; i < 60; i++)       /**< 绘制分 秒 刻度 */
    {
        GUI_SetPenSize(2);
        GUI_SetBkColor(GUI_BLACK);
        GUI_SetColor(GUI_WHITE);
        start_x_min = circle[0].center_x + (circle[0].raduis - 2) * COS_MIN_SPACE(i);
        start_y_min = circle[0].center_y + (circle[0].raduis - 2) * SIN_MIN_SPACE(i);
        end_x_min   = circle[0].center_x + (circle[0].raduis) * COS_MIN_SPACE(i);
        end_y_min   = circle[0].center_y + (circle[0].raduis) * SIN_MIN_SPACE(i);
        GUI_DrawLine(start_x_min, start_y_min, end_x_min, end_y_min);
    }
    
    for (i = 0; i < 12; i++)        /**< 绘制时刻度 */
    {
        GUI_SetPenSize(2);
        GUI_SetBkColor(GUI_BLACK);
        GUI_SetColor(GUI_MAGENTA);
        start_x_hour = circle[0].center_x + (circle[0].raduis - 8) * COS_HOUR_SPACE(i);
        start_y_hour = circle[0].center_y + (circle[0].raduis - 8) * SIN_HOUR_SPACE(i);
        end_x_hour   = circle[0].center_x + (circle[0].raduis) * COS_HOUR_SPACE(i);
        end_y_hour   = circle[0].center_y + (circle[0].raduis) * SIN_HOUR_SPACE(i);
        GUI_DrawLine(start_x_hour, start_y_hour, end_x_hour, end_y_hour);
        
        end_x_hour   = circle[0].center_x + (circle[0].raduis + 8) * COS_HOUR_SPACE(i);
        end_y_hour   = circle[0].center_y + (circle[0].raduis + 8) * SIN_HOUR_SPACE(i);
        switch (i)
        {
        case 0: 
            GUI_DispStringHCenterAt("3", end_x_hour, end_y_hour);
        break;
        case 1: 
            GUI_DispStringHCenterAt("4", end_x_hour, end_y_hour);
        break;
        case 2: 
            GUI_DispStringHCenterAt("5", end_x_hour, end_y_hour);
        break;
        case 3: 
            GUI_DispStringHCenterAt("6", end_x_hour, end_y_hour);
        break;
        case 4: 
            GUI_DispStringHCenterAt("7", end_x_hour, end_y_hour);
        break;
        case 5: 
            GUI_DispStringHCenterAt("8", end_x_hour, end_y_hour);
        break;
        case 6: 
            GUI_DispStringHCenterAt("9", end_x_hour, end_y_hour);
        break;
        case 7: 
            GUI_DispStringHCenterAt("10", end_x_hour, end_y_hour);
        break;
        case 8: 
            GUI_DispStringHCenterAt("11", end_x_hour, end_y_hour);
        break;
        case 9: 
            GUI_DispStringHCenterAt("12", end_x_hour, end_y_hour);
        break;
        case 10: 
            GUI_DispStringHCenterAt("1", end_x_hour, end_y_hour);
        break;
        case 11: 
            GUI_DispStringHCenterAt("2", end_x_hour, end_y_hour);
        break;
        default:
            break;          
        }
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
//    uint16_t coor_x_hour, coor_y_hour;
//    uint16_t coor_x_min, coor_y_min;
    
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

//    coor_x_min  = circle[0].center_x + circle[0].raduis * COS_MIN_SPACE(min);
//    coor_y_min  = circle[0].center_y + circle[0].raduis * SIN_MIN_SPACE(min);
//    coor_x_hour = circle[0].center_x + circle[0].raduis * COS_HOUR_SPACE(hour);
//    coor_y_hour = circle[0].center_y + circle[0].raduis * SIN_HOUR_SPACE(hour);
    
    coor_x_hour_pointer = circle[0].center_x + (circle[0].raduis - 20) * COS_HOUR_SPACE(hour);
    coor_y_hour_pointer = circle[0].center_y + (circle[0].raduis - 20) * SIN_HOUR_SPACE(hour);
    coor_x_min_pointer =  circle[0].center_x + (circle[0].raduis - 6)  * COS_MIN_SPACE(min);
    coor_y_min_pointer =  circle[0].center_y + (circle[0].raduis - 6)  * SIN_MIN_SPACE(min);
    coor_x_sec_pointer  = circle[0].center_x + (circle[0].raduis - 0)  * COS_MIN_SPACE(sec);
    coor_y_sec_pointer  = circle[0].center_y + (circle[0].raduis - 0)  * SIN_MIN_SPACE(sec);
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
    GUI_SetPenSize(4);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_GREEN);
//    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_hour, coor_y_hour);  
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_hour_pointer, coor_y_hour_pointer);  
    /**< 分钟 指针 */
    GUI_SetPenSize(2);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_CYAN);
//    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_min, coor_y_min);    
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_min_pointer, coor_y_min_pointer);  
    /**< 秒 指针 */
    GUI_SetPenSize(1);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_WHITE);
//    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_sec, coor_y_sec); 
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_sec_pointer, coor_y_sec_pointer); 
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
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_hour_pointer, coor_y_hour_pointer);  
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
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_min_pointer, coor_y_min_pointer);    

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
//    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_sec, coor_y_sec); 
    GUI_DrawLine(circle[0].center_x, circle[0].center_y, coor_x_sec_pointer, coor_y_sec_pointer); 
}





