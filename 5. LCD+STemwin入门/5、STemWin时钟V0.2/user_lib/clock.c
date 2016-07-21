/** 
 *  @file       clock.c
 *  @brief      ������
 *  @author     ZBT   
 *  @version    
 *  @date       2016 
 */
  
#include "clock.h"
#include "key.h"
#include "tim_dup.h"
#include "delay.h"
#include "gui.h"            /**< STemwin */


static uint8_t sec  = 0;
static uint8_t min  = 0;
static uint8_t hour = 0;
static enum  CLOCK_STATE  clock_state = CLOCK_RUN;

static void GUI_DisplayClock(void);


/**
 *  @Brief  ������������
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
 *  @Brief  ������������ʾ����
 *  @param  None
 *  @return None
 */
void clock_run(void)
{ 
    
    if (read_timer_flag(TIMER_7))
    {
        sec++;
        if (sec > 59)
        {
            sec = 0;
            min++;
            if (min > 59)
            {
                min = 0;
                hour++;
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
 *  @Brief  �����������������
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
 *  @Brief  �����ӷ����������
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
 *  @Brief  ������Сʱ��������
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
 *  @Brief  �õ���ǰʱ��ֵ
 *  @param  None
 *  @return ʱ�ӽṹ���ֵ
 */
struct CLOCK_VALUE get_clock_value(void)
{
    struct CLOCK_VALUE clock_value;
    
    clock_value.sec  = sec;
    clock_value.min  = min;
    clock_value.hour = hour;
    
    return clock_value;
}


/**< --------------------------------- ˽�к��� --------------------------------- */
/**
 *  @Brief  ʱ����ʾ����
 *  @param  None
 *  @return None
 */
static void GUI_DisplayClock(void)
{
    uint16_t coor_x = 40;
    uint16_t coor_y = 220;
    
    GUI_SetBkColor(GUI_BLACK);      /**< �趨��ʾʱ���ǰ��ɫ�ͱ���ɫ */
    GUI_SetColor(GUI_GREEN);
//     GUI_SetFont(&GUI_Font32B_ASCII);   
    GUI_SetFont(&GUI_Font8x16);    
    GUI_DispStringAt("Time:", 0, coor_y);              /**< ��ʾ�ַ� */
    
    GUI_DispDecAt(hour / 10, coor_x, coor_y, 1);          /**< ��ʾʱ �� ��*/     
    GUI_DispDecAt(hour % 10, coor_x + 10, coor_y, 1);
    GUI_DispStringAt(":",coor_x + 20, coor_y);                  /**< ��ʾ�ַ� */
    GUI_DispDecAt(min / 10, coor_x + 30, coor_y, 1);
    GUI_DispDecAt(min % 10, coor_x + 40, coor_y, 1);
    GUI_DispStringAt(":",coor_x + 50, coor_y);                  /**< ��ʾ�ַ� */
    GUI_DispDecAt(sec / 10 , coor_x + 60, coor_y, 1);
    GUI_DispDecAt(sec % 10 , coor_x + 70, coor_y, 1);
}

