/** 
 *  @file       clock.c
 *  @brief      电子钟
 *  @author     ZBT   
 *  @version    
 *  @date       2016 
 */
  
#include "clock.h"
#include "key.h"
#include "tim_dup.h"
#include "delay.h"
#include "gui.h"            /**< STemwin */
#include "gui_clock.h"


static uint8_t sec  = 0;
static uint8_t min  = 0;
static uint8_t hour = 0;
static enum  CLOCK_STATE  clock_state = CLOCK_RUN;

static void GUI_DisplayClock(void);


/**
 *  @Brief  电子钟主程序
 *  @param  None
 *  @return None
 */
void  clock(void)
{
	switch(clock_state)
    {
    case CLOCK_RUN:
        clock_run();
        break;
    case CLOCK_SEC_ADJ:
        clock_sec_adj();
        break;
    case CLOCK_MIN_ADJ:
        clock_min_adj();
        break;
    case CLOCK_HOUR_ADJ:
        clock_hour_adj();
        break;
    default:
        break;
    }	
}


/**
 *  @Brief  电子钟运行显示界面
 *  @param  None
 *  @return None
 */
void clock_run(void)
{ 
    
    if (read_timer_flag(TIMER_7))
    {
        sec++;
        clear_clock_sec_pointer();
        if (sec > 59)
        {
            clear_clock_min_pointer();
            sec = 0;
            min++;
            if (min > 59)
            {
                min = 0;
                hour++;
                clear_clock_hour_pointer();
                if (hour > 23)
                {
                    hour = 0;
                }
            }
        }
    }
    
    if (key_read() == KEY_CURSOR)
    {
        clock_state = CLOCK_SEC_ADJ;
    }
    else
    {
        
    }
    
    GUI_DisplayClock();
}
/**
 *  @Brief  电子钟秒针调整界面
 *  @param  None
 *  @return None
 */
void clock_sec_adj(void)
{
   GUI_DisplayClock();
    
	switch (key_read())
    {
    case KEY_CURSOR:
        clock_state = CLOCK_MIN_ADJ;
    break;
    case KEY_ADJ:
        sec = (sec > 58)? 0 : sec + 1;
    break;
    default:
        break;
    }
    

}

/**
 *  @Brief  电子钟分针调整界面
 *  @param  None
 *  @return None
 */
void clock_min_adj(void)
{
    GUI_DisplayClock();
    
    switch (key_read())
    {
    case KEY_CURSOR:
        clock_state = CLOCK_HOUR_ADJ;
    break;
    case KEY_ADJ:
        min = (min > 58)? 0 : min + 1;
    break;
    default:
        break;
    }
}

/**
 *  @Brief  电子钟小时调整界面
 *  @param  None
 *  @return None
 */
void clock_hour_adj(void)
{
    GUI_DisplayClock();
	
    switch (key_read())
    {
    case KEY_CURSOR:
        clock_state = CLOCK_RUN;
    break;
    case KEY_ADJ:
        hour = (hour > 22)? 0 : hour + 1;
    break;
    default:
        break;
    }
}

/**
 *  @Brief  得到当前时钟值
 *  @param  None
 *  @return 时钟结构体的值
 */
struct CLOCK_VALUE get_clock_value(void)
{
    struct CLOCK_VALUE clock_value;
    
    clock_value.sec  = sec;
    clock_value.min  = min;
    clock_value.hour = hour;
    
    return clock_value;
}


/**< --------------------------------- 私有函数 --------------------------------- */
/**
 *  @Brief  时钟显示界面
 *  @param  None
 *  @return None
 */
static void GUI_DisplayClock(void)
{
    uint16_t coor_x = 40;
    uint16_t coor_y = 220;
    
    GUI_SetBkColor(GUI_BLACK);      /**< 设定显示时间的前景色和背景色 */
    GUI_SetColor(GUI_GREEN);
//     GUI_SetFont(&GUI_Font32B_ASCII);   
    GUI_SetFont(&GUI_Font8x16);    
    GUI_DispStringAt("Time:", 0, coor_y);              /**< 显示字符 */
    
    GUI_DispDecAt(hour / 10, coor_x, coor_y, 1);          /**< 显示时 分 秒*/     
    GUI_DispDecAt(hour % 10, coor_x + 10, coor_y, 1);
    GUI_DispStringAt(":",coor_x + 20, coor_y);                  /**< 显示字符 */
    GUI_DispDecAt(min / 10, coor_x + 30, coor_y, 1);
    GUI_DispDecAt(min % 10, coor_x + 40, coor_y, 1);
    GUI_DispStringAt(":",coor_x + 50, coor_y);                  /**< 显示字符 */
    GUI_DispDecAt(sec / 10 , coor_x + 60, coor_y, 1);
    GUI_DispDecAt(sec % 10 , coor_x + 70, coor_y, 1);
}

