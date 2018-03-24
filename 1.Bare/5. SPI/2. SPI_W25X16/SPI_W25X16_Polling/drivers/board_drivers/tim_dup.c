/**
  * @file  tim_dup.c
  * @brief 定时器上层函数
  * @par   date        version    author    remarks
  *        2016-03-18  v1.0       zbt       初次创建
  *
  */

/** 头文件包含区 ------------------------------------------------ */
#include "bsp.h"

/** 私有宏(类型定义) -------------------------------------------- */ 

/** 私有变量 --------------------------------------------------- */ 
static uint8_t timer_flag[] = {0, 0, };

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */

/** 公有函数 --------------------------------------------------- */
/**
  * @brief  在中断函数中置位定时器标志 
  * @param  enum GENERAL_TIM timer_x TIMER_2 -- TIMER_5
  * @retval None
  */
void set_timer_flag(enum BASE_TIMx timer_x)
{
    timer_flag[timer_x] = 1;
}    

/**
  * @brief  读取timer的标志
  * @param 	enum BASE_TIM6_7 timer_x -- TIMER_6  TIMER_7	
  * @retval 0 定时时间未到 ---- 1 定时时间到
  */
uint8_t read_timer_flag(enum BASE_TIMx timer_x)
{
    uint8_t tmp_flag;
    
    tmp_flag = timer_flag[timer_x];
    timer_flag[timer_x] = 0;           /**< 清除定时器标志位 */
    
    return tmp_flag;  
}

/** 私有函数 --------------------------------------------------- */

