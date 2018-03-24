/**
  * @file  exti_dup.c
  * @brief 外部中断上层函数
  * @par   date        version    author    remarks
  *        2016-03-04  v1.0       zbt       初次创建
  *
  */  

/** 头文件包含区 ------------------------------------------------ */
#include "bsp.h"

/** 私有宏(类型定义) -------------------------------------------- */ 

/** 私有变量 --------------------------------------------------- */
static uint8_t exti_flag[] = {0, 0,};

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */

/** 公有函数 --------------------------------------------------- */

/**
  *  @brief  置位外部中断
  *  @param  enum EXTI_N exti_n	外部中断编号
  *  @retval None
  */
void set_exti_flag(enum EXTI_N exti_n)
{
    exti_flag[exti_n] = 1;
}

/**
  * @brief  读取外部中断标志
  * @param 	enum EXTI_N exti_n	外部中断编号	
  * @retval None
  */
uint8_t read_exti_flag(enum EXTI_N exti_n)
{
    uint8_t flag;
    
    flag = exti_flag[exti_n];
    exti_flag[exti_n] = 0;          /**< 清除中断标志位 */
    
    return flag;
}

/**     参考用法
    if (read_exti_flag(EXTI_0))
    {
       code
    }
*/ 

/** 私有函数 --------------------------------------------------- */

