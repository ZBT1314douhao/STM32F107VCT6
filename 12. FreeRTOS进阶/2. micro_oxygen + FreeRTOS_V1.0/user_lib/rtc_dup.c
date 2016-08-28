/**
  * @file	rtc_dup.c 
  * @brief	STM32F107VCT6内部RTC
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			初次创建
  */

/** -------------------- 头文件包含区 ------------------------------ */
#include "rtc_dup.h"
#include "gui.h"

/** -------------------- 文件私有变量 ------------------------------ */
static RTC_TimeTypeDef struct_time;
static RTC_DateTypeDef struct_date;

/** -------------------- 外部引用变量 ------------------------------ */
extern RTC_HandleTypeDef hrtc;

/** -------------------- 私有函数原型 ------------------------------ */

/** -------------------- 公有函数 --------------------------------- */
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
 * @brief   将spinbox中设定的时间值写入到RTC 
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
 * @brief   将spinbox中设定的日期值写入到RTC 
 * @param   *sDate：存储日期的结构体指针
 * @retval  None
 */
void RTC_set_date(RTC_DateTypeDef *sDate)
{
    HAL_RTC_SetDate(&hrtc, sDate, RTC_FORMAT_BIN);
}

 /** -------------------- 私有函数 --------------------------------- */

