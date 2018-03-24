/**
  * @file  bsp.c
  * @brief 存放驱动层头文件
  * @par   date        version    author    remarks
  *        2016-10-06  v1.0       zbt       初次创建
  *
  */

/** 头文件包含区 ------------------------------------------------ */
#include "bsp.h"

/** 私有宏(类型定义) -------------------------------------------- */ 

/** 私有变量 --------------------------------------------------- */ 

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */
static void bsp_printf_info(void);

/** 公有函数 --------------------------------------------------- */

/**
  * @brief  初始化所有的硬件底层驱动等
  * @param	None
  * @retval None
  */
void bsp_init(void)
{
    /** 串口打印信息 */
    bsp_printf_info();
    /** STemwin 初始化 */
//    GUI_Init();
//    GUI_UC_SetEncodeUTF8();
//    GUI_SetBkColor(GUI_BLACK);
//    GUI_SetColor(GUI_GREEN);
    
    /** board drivers 初始化 */
    
    /** user drivers 初始化 */
    w25x16_test();

}

/**
  * @brief  空闲时执行的函数
  * @param	None
  * @retval None
  * @note   用户可以在此函数体内执行喂狗，休眠CPU等功能（如果需要）
            模板文件未添加程序，用户可自行添加
  */
//void bsp_idle(void)
//{
//    
//}


/** 私有函数 --------------------------------------------------- */
/**
  * @brief  通过串口打印一些系统信息
  * @param	None
  * @retval None
  */
static void bsp_printf_info(void)
{
    printf("\n\r");
    printf("Hardware : STM32F107VCT6\n\r");
    printf("Software : MDK V5.20\n\r");
    printf("Eamil : zbt1314douhao@live.com\n\r");
}

