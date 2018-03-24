/**
* @file  iwdg_dup.c
* @brief 独立看门狗上层程序
* @par   date        version    author    remarks
*        2016-08-13  v1.0       zbt       初次创建
*
*/

/** 头文件包含区 ------------------------------------------------ */
#include "iwdg_dup.h"
#include "led.h"

/** 私有宏(类型定义) -------------------------------------------- */ 

/** 私有变量 --------------------------------------------------- */ 

/** 外部变量 --------------------------------------------------- */
extern IWDG_HandleTypeDef hiwdg;

/** 私有函数原型 ----------------------------------------------- */
//static void pravite_function(void);

/** 公有函数 --------------------------------------------------- */

/**
  * @brief  检查系统是否从独立看门狗复位中恢复
  * @param	None
  * @retval None
  */
void check_system_resume_status(void)
{
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
    {
        led_on(LED_0);
        /** 清除复位标志 */
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    else
    {
        led_off(LED_0);
    }
}

/**
  * @brief  启动独立看门狗
  * @param	None
  * @retval None
  * @note   启动失败则LED_1被点亮
  */
void start_iwdg(void)
{
    if (HAL_IWDG_Start(&hiwdg) != HAL_OK)
    {
        led_on(LED_1);
    }
    else
    {
        led_off(LED_1);
    }
}

/**
  * @brief  喂独立看门狗
  * @param	None
  * @retval None
  * @note   喂狗失败则LED_2被点亮
  */
void feed_iwdg(void)
{
    if (HAL_IWDG_Refresh(&hiwdg) != HAL_OK)
    {
        led_on(LED_2);
    }
    else
    {
        led_off(LED_2);
    }
}

/** 私有函数 --------------------------------------------------- */

/**
  * @brief 
  * @param	None
  * @retval None
  */
//static void pravite_function(void)
//{

//}
