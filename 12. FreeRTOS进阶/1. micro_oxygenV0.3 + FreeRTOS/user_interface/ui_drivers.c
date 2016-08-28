/**
  * @file	ui_drivers.c 
  * @brief	用户界面公用函数
  * @par	date        version     author      remarks
  *			2016-07-22	V1.0		zbt			初次创建
  */
  
/** -------------------- 头文件包含区 ------------------------------ */
#include "ui_drivers.h"
#include "start_window.h"

/** -------------------- 文件私有变量 ------------------------------ */

/** -------------------- 私有函数原型 ------------------------------ */


/** -------------------- 公有函数 ------------------------------- */
/**
  * @brief  切换对话框函数
  * @param  hWin：旧对话框的句柄
  *         pFunc：新对话框的信息获取函数
  *         timer：新对话框定时器使能或失能TIMER_ENABLE-TIMER_DISALBE
  *         period：定时器的周期，单位时间10ms，定时时间为period * 10ms
  * @retval hDlg：新窗口的句柄
  * @note   此函数只适用于创建一个定时器或者不创建定时器的对话框的切换，
  *         对于要创建多个定时器的窗口，请利用返回值在函数外创建，形式
  *         如下：
  *         hDlg = toggle_interface(hWin, pFunc, TIMER_DISABLE, 10);
  *         WM_CreateTimer(WM_GetClientWindow(hDlg), TIMER1_ID, period1, 0);
  *         WM_CreateTimer(WM_GetClientWindow(hDlg), TIMER2_ID, period2, 0);
  *         WM_CreateTimer(WM_GetClientWindow(hDlg), TIMER3_ID, period3, 0);
  */
WM_HWIN toggle_interface(WM_HWIN hWin, void (*pFunc)(WINDOW_INFO*),
                              enum TIMER_STATE timer, uint16_t period)
{
    WINDOW_INFO next_window;
    WM_HWIN     hDlg;

    /** 结束旧对话框 */
    if (hWin != NO_HWIN)
    {
        GUI_EndDialog(hWin, 0);
    }
    /** 获取新对话框的信息 */
    pFunc(&next_window);
    /** 创建新对话框 */
    hDlg = GUI_CreateDialogBox(next_window.p, next_window.length,
                               next_window.func, 0, 0, 0);
    /** 根据timer判断是否需要为对话框创建定时器 */
    if (timer == TIMER_ENABLE)
    {
        WM_CreateTimer(WM_GetClientWindow(hDlg), 0, period, 0);
    }    
    return hDlg;
}

/**
  * @brief  用户界面初始配置
  * @param  None
  * @retval None
  */
void user_interface_config(void)
{
    GUI_Init();
    /** 采用UTF8编码 */
    GUI_UC_SetEncodeUTF8();
    WM_SetCreateFlags(WM_CF_MEMDEV);    /**< 开启内存支持, 防刷屏闪烁 */
    
    /** 设置 小工具的皮肤 */
    PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
}

/** -------------------- 私有函数 --------------------------------- */
