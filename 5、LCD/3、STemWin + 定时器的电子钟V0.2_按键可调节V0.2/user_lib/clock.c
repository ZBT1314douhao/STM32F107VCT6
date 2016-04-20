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
            if (min > 60)
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

static void GUI_DisplayClock(void)
{
     GUI_SetFont(&GUI_Font32B_ASCII);    
    //GUI_DispStringAt("T",136,150);              /**< ��ʾ�ַ� */
    
    GUI_DispDecAt(hour / 10, 0, 80, 1);          /**< ��ʾʱ �� ��*/     
    GUI_DispDecAt(hour % 10, 20, 80, 1);
    GUI_DispStringAt(":",40, 80);                  /**< ��ʾ�ַ� */
    GUI_DispDecAt(min / 10, 60, 80, 1);
    GUI_DispDecAt(min % 10, 80, 80, 1);
    GUI_DispStringAt(":",100, 80);                  /**< ��ʾ�ַ� */
    GUI_DispDecAt(sec / 10 , 120, 80, 1);
    GUI_DispDecAt(sec % 10 , 140, 80, 1);
}

