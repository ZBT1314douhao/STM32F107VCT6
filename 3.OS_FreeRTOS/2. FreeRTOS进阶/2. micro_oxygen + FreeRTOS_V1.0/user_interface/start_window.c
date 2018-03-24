/**
  * @file	start_window.c 
  * @brief	微氧分析仪开机进度条加载界面
  * @par	date        version     author      remarks
  *			2016-07-22	V1.0		zbt			初次创建
  */

/** -------------------- 头文件包含区 ------------------------------ */
#include "start_window.h"
#include "measure_window.h"
#include "sys_info_window.h"
#include "adjust_time_window.h"

/** -------------------- 文件私有变量 ------------------------------ */
#define TIMER_TICK          1
#define START_TIMER_COUNT   50         /**< TIMER_TICK计数 */

/** 创建起始画面要显示的小工具 */
static GUI_WIDGET_CREATE_INFO start_window_interface[] = 
{
    {WINDOW_CreateIndirect, NULL, 0, 0, 0, 320, 240, 0, 0, 0,},
    {PROGBAR_CreateIndirect, NULL, GUI_ID_PROGBAR0, 30, 160, 249, 30, 0, 0, 0,},
};

static WM_HWIN hWin;
static uint16_t s_start_count;

/** -------------------- 外部引用变量 ------------------------------ */
extern GUI_CONST_STORAGE GUI_FONT GUI_FontMicrosoftYaHeiMono28;

/** -------------------- 私有函数原型 ------------------------------ */
static void start_window_info_cb(WM_MESSAGE *pMsg);
static void start_window_init(WM_MESSAGE *pMsg);
static void start_window_paint(WM_MESSAGE *pMsg);

/** -------------------- 公有函数 --------------------------------- */
/**
 * @brief  读取开机界面的资源表、控件数量和回调函数地址
 * @param  info:存放上述内容的结构体指针
 * @retval None
 */
void start_window_info(WINDOW_INFO *info)
{
    info->p      = start_window_interface;
    info->length = GUI_COUNTOF(start_window_interface);
    info->func   = &start_window_info_cb;   
}
/** -------------------- 私有函数 --------------------------------- */
/**
 * @brief  开始界面回调函数
 * @param  *pMsg：消息指针
 * @retval None
 */
static void start_window_info_cb(WM_MESSAGE *pMsg)
{
    hWin = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
       start_window_init(pMsg);
        break;
    case WM_PAINT:
        start_window_paint(pMsg);
        break;
    case WM_TIMER:
        s_start_count = (s_start_count > 300)? 0 : s_start_count + 1;
        PROGBAR_SetValue(WM_GetDialogItem(hWin, GUI_ID_PROGBAR0), s_start_count);
        if (s_start_count > START_TIMER_COUNT)
        {
            toggle_interface(hWin, measure_window_info, TIMER_ENABLE, TIMER_PERIOD);
        }
        WM_RestartTimer(pMsg->Data.v, TIMER_TICK);      /**< 重启定时器 */
    default:
        WM_DefaultProc(pMsg);      
    }
}

/**
 * @brief  绘制开始主界面
 * @param  *pMsg：消息指针
 * @retval None
 */
static void start_window_paint(WM_MESSAGE *pMsg)
{
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_FontMicrosoftYaHeiMono28);
    GUI_DispStringAt("南京工业大学", 80, 100); 
}

/**
 * @brief   初始化对话框
 * @param   *pMsg：消息指针
 * @retval  None
 */ 
static void start_window_init(WM_MESSAGE *pMsg)
{   
    /** 设置窗口属性 */
    WINDOW_SetBkColor(WM_GetClientWindow(hWin), GUI_GRAY);
    /** 设置进度条属性 */
    PROGBAR_SetFont(WM_GetDialogItem(hWin, GUI_ID_PROGBAR0), &GUI_Font16B_1);
    PROGBAR_SetMinMax(WM_GetDialogItem(hWin, GUI_ID_PROGBAR0), 0, START_TIMER_COUNT);
}
