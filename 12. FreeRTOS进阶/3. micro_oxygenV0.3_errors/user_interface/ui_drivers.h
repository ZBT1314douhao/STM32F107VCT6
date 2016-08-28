/**
  *
  * @file	ui_drivers.h 
  * @brief	�û����湫�ú���
  * @par	date        version     author      remarks
  *			2016-07-22	V1.0		zbt			���δ���			
  */

#ifndef __UI_DRIVERS_H
#define __UI_DRIVERS_H

/** ---------------------- ͷ�ļ������� ---------------------------- */
#include "stm32f1xx_hal.h"
/** emWin ���ͷ�ļ� */
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

/** �Ի�����Ϣ�ṹ�� */
typedef void (*_CALL_BACK)(WM_MESSAGE *pMsg);
typedef struct window
{
    GUI_WIDGET_CREATE_INFO *p;         /**< ��������ָ�� */
    uint8_t                 length;    /**< ���鳤�� */
    _CALL_BACK              func;      /**< ����ص����� */
}WINDOW_INFO;

enum TIMER_STATE
{
    TIMER_ENABLE, TIMER_DISABLE
};
#define TIMER_PERIOD          (1000)

#define NO_HWIN               (-1)

/** ---------------------- �ļ��ⲿ�ӿ� ---------------------------- */
void user_interface_config(void);
WM_HWIN toggle_interface(WM_HWIN hWin, void (*pFunc)(WINDOW_INFO*),
                              enum TIMER_STATE timer, uint16_t period);

#endif
