/**
  * @file	temperature.c
  * @brief	利用STM32F107VCT6内部温度传感器获取环境温度
  * @par	date        version     author      remarks
  *			2016-07-23	V1.0		zbt			初次创建			
  */

/** -------------------- 头文件包含区 ------------------------------ */
#include "temperature.h"
#include "adc.h"
#include "gui.h"

/** -------------------- 文件私有变量 ------------------------------ */

#define TEMPERATURE_BUFF_SIZE       8

//static __IO uint16_t ad_temp_buff[TEMPERATURE_BUFF_SIZE];


/** -------------------- 外部引用变量 ------------------------------ */

/** -------------------- 私有函数原型 ------------------------------ */



/** -------------------- 公有函数 --------------------------------- */




 /**
 *  @brief  显示采集到的温度值
 *  @param  coor_x：显示温度值的X坐标
            coor_y：显示温度值的Y坐标
 *  @retval None
 */
void display_temperature(const uint16_t coor_x, const uint16_t coor_y)
{
    struct S_AD s_ad;
    
    s_ad = AD_get_temperature();
    
    GUI_GotoXY(coor_x, coor_y);
    GUI_DispFloat(s_ad.value, 6);
    GUI_DispStringAt("V", (coor_x + 70), coor_y);
    GUI_GotoXY((coor_x + 110), coor_y);
    GUI_DispFloat(s_ad.temperature_value, 4);
}

 /** -------------------- 私有函数 --------------------------------- */



