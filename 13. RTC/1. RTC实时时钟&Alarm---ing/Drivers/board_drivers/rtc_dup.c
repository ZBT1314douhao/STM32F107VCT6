/**
  * @file	rtc_dup.c 
  * @brief	STM32F107VCT6内部RTC
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			初次创建
  */

/** -------------------- 头文件包含区 ------------------------------ */
#include "rtc_dup.h"
#include "led.h"
#include "gui.h"

/** -------------------- 文件私有变量 ------------------------------ */
static RTC_TimeTypeDef struct_time;
static RTC_DateTypeDef struct_date;

/** -------------------- 外部引用变量 ------------------------------ */
extern RTC_HandleTypeDef hrtc;

/** -------------------- 私有函数原型 ------------------------------ */

/** -------------------- 公有函数 --------------------------------- */

/**
  * @brief   RTC初始化配置
  * @param   None
  * @retval  None
  */
void rtc_init(void)
{
    RTC_TimeTypeDef  sTime;
    RTC_DateTypeDef  DateToUpdate;
    RTC_AlarmTypeDef sAlarm;
    
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
    HAL_RTC_Init(&hrtc);
    
    /** 检查数据是否存储在备份存储器1， 若不在则配置RTC，否则
        从备份存储器中读取RTC数据 
    */
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F1)
    {
    
        DateToUpdate.WeekDay = RTC_WEEKDAY_THURSDAY;
        DateToUpdate.Month = RTC_MONTH_AUGUST;
        DateToUpdate.Date = 0x15;
        DateToUpdate.Year = 0x16;
        HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD);
    
        sTime.Hours = 0x10;
        sTime.Minutes = 0x06;
        sTime.Seconds = 0x24;
        HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
        
        /** 向RTC备份寄存器1写入数据 */
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F1);
        /** 将日期写入备份寄存器 */
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, DateToUpdate.WeekDay);
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, DateToUpdate.Month);
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, DateToUpdate.Date);
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, DateToUpdate.Year);
    }
    else
    {
        /** POR/PDR复位标志置位 */
        if(__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
        {
            led_on(LED_0);
            HAL_Delay(1000);
            led_off(LED_0);
        }
        /** 引脚复位标志置位 */
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
        {
            led_on(LED_1);
            HAL_Delay(1000);
            led_off(LED_1);
        }
        __HAL_RCC_CLEAR_RESET_FLAGS();
        
        /** 从备份寄存器中读取掉电时的日期值并写入到RTC */
        DateToUpdate.WeekDay = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
        DateToUpdate.Month = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
        DateToUpdate.Date = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
        DateToUpdate.Year = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);
        HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD);
    }
    
    /** 配置RTC闹钟 */
    sAlarm.Alarm = RTC_ALARM_A;
    sAlarm.AlarmTime.Hours = 0x0;
    sAlarm.AlarmTime.Minutes = 0x00;
    sAlarm.AlarmTime.Seconds = 0x09;
    HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD);
}
 
/**
  * @brief  从RTC获取当前时间
  * @param  coor_x: 显示时间的x坐标值
            coor_y：显示时间的y坐标值
  * @retval None
  */
void RTC_display_current_time(const uint16_t coor_x, const uint16_t coor_y)
{ 
    HAL_RTC_GetTime(&hrtc, &struct_time, RTC_FORMAT_BIN);
    
    GUI_DispDecAt(struct_time.Hours, (coor_x), coor_y, 2);
    GUI_DispStringAt(":", (coor_x + 25), coor_y);
    GUI_DispDecAt(struct_time.Minutes, (coor_x + 30), coor_y, 2);
    GUI_DispStringAt(":", (coor_x + 55), coor_y);
    GUI_DispDecAt(struct_time.Seconds, (coor_x + 60), coor_y, 2);
}

/**
  * @brief   从RTC获取当前时间 
  * @param   None
  * @retval  返回获得的时间结构体
  */
RTC_TimeTypeDef RTC_get_time(void)
{   
    HAL_RTC_GetTime(&hrtc, &struct_time, RTC_FORMAT_BIN);
    
    return struct_time;
}

/**
  * @brief   设置RTC时间
  * @param   *sTime：存储时间的结构体指针
  * @retval  None
  */
void RTC_set_time(RTC_TimeTypeDef *sTime)
{
    HAL_RTC_SetTime(&hrtc, sTime, RTC_FORMAT_BIN);
}

/**
  * @brief   从RTC获取当前日期 
  * @param   coor_x: 显示日期的x坐标值
             coor_y：显示日期的y坐标值
  * @retval  None
  */
void RTC_display_current_date(const uint16_t coor_x, const uint16_t coor_y)
{
    HAL_RTC_GetDate(&hrtc, &struct_date, RTC_FORMAT_BIN);
    
    GUI_DispDecAt(20, (coor_x), coor_y, 2);
    GUI_DispDecAt(struct_date.Year, (coor_x + 25), coor_y, 2);
    GUI_DispStringAt("-", (coor_x + 50), coor_y);
    GUI_DispDecAt(struct_date.Month, (coor_x + 55), coor_y, 2);
    GUI_DispStringAt("-", (coor_x + 80), coor_y);
    GUI_DispDecAt(struct_date.Date, (coor_x + 85), coor_y, 2);
}

/**
  * @brief   从RTC获取当前日期 
  * @param   None
  * @retval  返回获得的日期结构体
  */
RTC_DateTypeDef RTC_get_date(void)
{ 
    HAL_RTC_GetDate(&hrtc, &struct_date, RTC_FORMAT_BIN);
    
    return struct_date;
}

/**
  * @brief   设置RTC日期
  * @param   *sDate：存储日期的结构体指针
  * @retval  None
  */
void RTC_set_date(RTC_DateTypeDef *sDate)
{
    HAL_RTC_SetDate(&hrtc, sDate, RTC_FORMAT_BIN);
}

/**
  * @brief   闹钟回调函数
  * @param   hrtc：RTC句柄
  * @retval  None
  * @note    此函数由RTC_Alarm_IRQHandler调用，用户无需调用，
             只需实现在中断函数里要实现的操作即可
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    led_on(LED_ALL);
    HAL_Delay(200);
    led_off(LED_ALL);
    HAL_Delay(500);
}

/** -------------------- 私有函数 --------------------------------- */

