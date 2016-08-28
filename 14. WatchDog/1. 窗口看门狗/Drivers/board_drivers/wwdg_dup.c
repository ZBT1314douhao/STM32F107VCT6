/**
* @file  wwdg_dup.c
* @brief 窗口看门狗上层程序
* @par   date        version    author    remarks
*        2016-08-13  v1.0       zbt       初次创建
*
*/

/** 头文件包含区 ------------------------------------------------ */
#include "wwdg_dup.h"
#include "led.h"

/** 私有宏(类型定义) -------------------------------------------- */ 

/** 私有变量 --------------------------------------------------- */ 

/** 外部变量 --------------------------------------------------- */
extern WWDG_HandleTypeDef hwwdg;

/** 私有函数原型 ----------------------------------------------- */
//static void pravite_function(void);

/** 公有函数 --------------------------------------------------- */

/**
  * @brief  检查系统是否从窗口看门狗复位中恢复
  * @param	None
  * @retval None
  * @note   若恢复过来 则闪烁LED_0一次
  */
void check_system_resume_status(void)
{
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
    {
        led_on(LED_0);
        HAL_Delay(1000);
        led_off(LED_0);
        /** 清除复位标志 */
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    else
    {
        led_off(LED_0);
    }
}

/**
  * @brief  启动窗口看门狗且早期中断使能
  * @param	None
  * @retval None
  * @note   启动失败则LED_1被点亮
  */
void start_wwdg(void)
{
    if (HAL_WWDG_Start_IT(&hwwdg) != HAL_OK)
    {
        led_on(LED_1);
    }
    else
    {
        led_off(LED_1);
    }
}

/**
  * @brief  刷新窗口看门狗
  * @param	None
  * @retval None
  * @note   喂狗失败则LED_2被点亮，将计数值更新到127，
            可得刷新窗口时间为43ms(~910 * (127-80)) and 58 ms(~910 * 64)
  */
void feed_wwdg(void)
{
    if (HAL_WWDG_Refresh(&hwwdg, 127) != HAL_OK)
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
