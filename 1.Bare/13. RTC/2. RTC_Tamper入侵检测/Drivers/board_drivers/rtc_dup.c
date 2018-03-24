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
static __IO FlagStatus TamperStatus = RESET;    /**< 入侵检测标志 */

static RTC_TimeTypeDef struct_time;
static RTC_DateTypeDef struct_date;
static const uint32_t aBKPDataReg[RTC_BKP_NUMBER] =
{
  RTC_BKP_DR1,  RTC_BKP_DR2,
  RTC_BKP_DR3,  RTC_BKP_DR4,  RTC_BKP_DR5,
  RTC_BKP_DR6,  RTC_BKP_DR7,  RTC_BKP_DR8,
  RTC_BKP_DR9,  RTC_BKP_DR10, RTC_BKP_DR11,
  RTC_BKP_DR12, RTC_BKP_DR13, RTC_BKP_DR14,
  RTC_BKP_DR15, RTC_BKP_DR16, RTC_BKP_DR17,
  RTC_BKP_DR18, RTC_BKP_DR19, RTC_BKP_DR20,
  RTC_BKP_DR21, RTC_BKP_DR22, RTC_BKP_DR23,
  RTC_BKP_DR24, RTC_BKP_DR25, RTC_BKP_DR26,
  RTC_BKP_DR27, RTC_BKP_DR28, RTC_BKP_DR29,
  RTC_BKP_DR30, RTC_BKP_DR31, RTC_BKP_DR32,
  RTC_BKP_DR33, RTC_BKP_DR34, RTC_BKP_DR35,
  RTC_BKP_DR36, RTC_BKP_DR37, RTC_BKP_DR38,
  RTC_BKP_DR39, RTC_BKP_DR40, RTC_BKP_DR41,
  RTC_BKP_DR42
};

/** -------------------- 外部引用变量 ------------------------------ */
extern RTC_HandleTypeDef hrtc;

/** -------------------- 私有函数原型 ------------------------------ */

/** -------------------- 公有函数 --------------------------------- */

/**
  * @brief   RTC断电后的初始化 
  * @param   None
  * @retval  None
  */
void rtc_init(void)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef DateToUpdate;
    
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
    HAL_RTC_Init(&hrtc);
    
    /** 检查数据是否存储在备份存储器1， 若不在则配置RTC，否则
        从备份存储器中读取RTC数据 
    */
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
    {
        sTime.Hours = 0x10;
        sTime.Minutes = 0x06;
        sTime.Seconds = 0x24;
        HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        
        DateToUpdate.WeekDay = RTC_WEEKDAY_THURSDAY;
        DateToUpdate.Month = RTC_MONTH_AUGUST;
        DateToUpdate.Date = 0x15;
        DateToUpdate.Year = 0x16;
        HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
  
        /** 向RTC备份寄存器1写入数据 */
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
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
    }
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
  * @brief   将设定的时间值写入到RTC 
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
  * @brief   将设定的日期值写入到RTC 
  * @param   *sDate：存储日期的结构体指针
  * @retval  None
  */
void RTC_set_date(RTC_DateTypeDef *sDate)
{
    HAL_RTC_SetDate(&hrtc, sDate, RTC_FORMAT_BIN);
}

/**
  * @brief   向备份寄存器中写入数据 
  * @param   bkp_data_reg[]：寄存器表（数组）
  * @retval  None
  */
void write_data_to_bkp_reg(void)
{
    uint8_t i;
    
    for (i = 0; i < RTC_BKP_NUMBER; i++)
    {
        HAL_RTCEx_BKUPWrite(&hrtc, aBKPDataReg[i], 0xdf59 + (i * 0x5a ));
    }
    
    for (i = 0; i < RTC_BKP_NUMBER; i++)
    {
        if (HAL_RTCEx_BKUPRead(&hrtc, aBKPDataReg[i]) != (0xDF59 + (i * 0x5A)))
        {
            /** 写入/读取数据出错处理 */
            while (1);  /**< 读取的数据和写入的数据不一致则陷入死循环 */
        }
    }
}

/**
  * @brief   用于检测在入侵检测产生后备份寄存器中的值是否被清除
  * @param   None
  * @retval  None
  */
void check_bkp_data(void)
{
    uint8_t i;
    
    for (i = 0; i < RTC_BKP_NUMBER; i++)
    {
        if (HAL_RTCEx_BKUPRead(&hrtc, aBKPDataReg[i]) != 0x00)
        {
            /** 写入/读取数据出错处理 */
            while (1);  /**< 读取的数据和写入的数据不一致则陷入死循环 */
        }
    }
}

/**
  * @brief   去激活入侵检测
  * @param   None
  * @retval  None
  */
void deactive_tamper(void)
{
    HAL_RTCEx_DeactivateTamper(&hrtc, RTC_TAMPER_1);
}

/**
  * @brief   设置入侵检测引脚标志
  * @param   flg_value：要给TamperStatus赋的值
             @arg：SET
                   RESET
  * @retval  None
  */
void set_tamper_flag(__IO FlagStatus flg_value)
{
    if (flg_value == 0)
    {
        TamperStatus = RESET;
    }
    else
    {
        TamperStatus = SET;
    }
}

/**
  * @brief   获取入侵检测标志
  * @param   None
  * @retval  FlagStatus：获取到的标志值
  */
FlagStatus get_tamper_flag(void)
{
    return TamperStatus;
}

/** -------------------- 私有函数 --------------------------------- */

