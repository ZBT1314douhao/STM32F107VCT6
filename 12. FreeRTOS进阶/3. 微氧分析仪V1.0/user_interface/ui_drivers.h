/**
  *
  * @file	ui_drivers.h 
  * @brief	用户界面公用函数
  * @par	date        version     author      remarks
  *			2016-07-22	V1.0		zbt			初次创建			
  */

#ifndef __UI_DRIVERS_H
#define __UI_DRIVERS_H

/** ---------------------- 头文件包含区 ---------------------------- */
#include "stm32f1xx_hal.h"
/** emWin 相关头文件 */
#include "gui.h"
#include "WM.h"
#include "DIALOG.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "SCROLLBAR.h"
#include "MESSAGEBOX.h"

#define NO_HWIN               (-1)

/** 对话框信息结构体 */
typedef void (*_CALL_BACK)(WM_MESSAGE *pMsg);
typedef struct window
{
    GUI_WIDGET_CREATE_INFO *p;         /**< 界面数组指针 */
    uint8_t                 length;    /**< 数组长度 */
    _CALL_BACK              func;      /**< 界面回调函数 */
}WINDOW_INFO;

enum TIMER_STATE
{
    TIMER_ENABLE, TIMER_DISABLE
};
#define TIMER_PERIOD          (1000)

#define NO_HWIN               (-1)

/** ---------------------- 文件外部接口 ---------------------------- */
void user_interface_config(void);
WM_HWIN toggle_interface(WM_HWIN hWin, void (*pFunc)(WINDOW_INFO*),
                              enum TIMER_STATE timer, uint16_t period);

#endif
