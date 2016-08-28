/**
  * @file	measure_window.c 
  * @brief	微氧分析仪测量主界面
  * @par	date        version     author      remarks
  *			2016-07-22	V1.0		zbt			初次创建
  */

/** -------------------- 头文件包含区 ------------------------------ */
#include "measure_window.h"
#include "adjust_time_window.h"
#include "sys_info_window.h"
#include "start_window.h"
#include "ui_drivers.h"
#include "rtc_dup.h"
#include "oxygen.h"
#include "adc_app.h"

/** -------------------- 文件私有变量 ------------------------------ */
static GUI_WIDGET_CREATE_INFO measure_window_interface[] = 
{   /** GUI_WIDGET_CREATE_FUNC* pfCreateIndirect,
        *pName Id x0, y0, xSize, ySize, Flags, Para, NumExtraBytes */
    {WINDOW_CreateIndirect, NULL, 0, 0, 0, 320, 240, 0, 0, 0},
    /** 绘制4个按钮 在屏幕下方均匀分布 */
    {BUTTON_CreateIndirect, "SetTime",  GUI_ID_BUTTON0, 9,   205, 60, 30, 0, 0},
    {BUTTON_CreateIndirect, "Range",    GUI_ID_BUTTON1, 83,  205, 60, 30, 0, 0},
    {BUTTON_CreateIndirect, "SysInfo",  GUI_ID_BUTTON2, 167, 205, 60, 30, 0, 0},
    {BUTTON_CreateIndirect, "Setting",  GUI_ID_BUTTON3, 250, 205, 60, 30, 0, 0},
};

static WM_HWIN hWin;
//static WM_HWIN hWinSrc;
static WM_HWIN hButton_n[4];

/** -------------------- 外部引用变量 ------------------------------ */
extern GUI_CONST_STORAGE GUI_FONT GUI_FontYaHei_Mono28;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontButton_YaHei_Mono18;

/** -------------------- 私有函数原型 ------------------------------ */
static void measure_window_info_cb(WM_MESSAGE *pMsg);
static void measure_window_paint(WM_MESSAGE *pMsg);
static void measure_window_init(WM_MESSAGE *pMsg);

/** -------------------- 公有函数 --------------------------------- */
/**
 * @brief  读取测量主界面的资源表、空间数量和回调函数地址
 * @param  info:存放上述内容的结构体指针
 * @retval None
 */
void measure_window_info(WINDOW_INFO *info)
{
    info->p      = measure_window_interface;
    info->length = GUI_COUNTOF(measure_window_interface);
    info->func   = &measure_window_info_cb;
}

/** -------------------- 私有函数 --------------------------------- */
/**
 * @brief  主界面回调函数
 * @param  *pMsg：消息指针
 * @retval None
 */
static void measure_window_info_cb(WM_MESSAGE *pMsg)
{
    int Id, NCode;
    
    hWin = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:    /**< 初始化对话框 */
        measure_window_init(pMsg);
        break;
    case WM_PAINT:      
        measure_window_paint(pMsg);
        break;
    case WM_TIMER:       /**< 定时触发窗口重绘 */
        WM_RestartTimer(pMsg->Data.v, 10);
        WM_InvalidateWindow(WM_GetClientWindow(hWin));
        break;
//    case WM_KEY:
//        break;
    case WM_NOTIFY_PARENT:    
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (NCode)
        {
        case WM_NOTIFICATION_RELEASED:
            switch (Id)
            {
            case GUI_ID_BUTTON0:
                toggle_interface(hWin, adj_time_window_info, 
                                    TIMER_ENABLE, TIMER_PERIOD);
            /** 将RTC时钟的当前值同步到spinbox的文本框 */
                rtc_write_to_spinbox();     
                break;
//            case GUI_ID_BUTTON1:
//                break;
            case GUI_ID_BUTTON2:
                toggle_interface(hWin, sys_info_window_info, 
                                    TIMER_DISABLE, 0);
                break;
//            case GUI_ID_BUTTON3:
//                toggle_interface(hWin, sys_info_window_info, 
//                                    TIMER_DISABLE, 0);
//                break;
            }
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

/**
 * @brief  绘制测量显示主界面
 * @param  *pMsg：消息指针
 * @retval None
 */
static void measure_window_paint(WM_MESSAGE *pMsg)
{
    uint8_t i;
    const uint8_t y_line_coor = 40;
    const uint8_t y_string_coor = 50;
    
    GUI_SetColor(GUI_BLACK);
    GUI_SetPenSize(2);                  /**< 设置画笔大小(单位：像素) */

    for (i = 0; i < 5; i++)             /**< 绘制主界面分割线 */
    {
        GUI_DrawLine(0, (y_line_coor + 40 * i), 320, (y_line_coor + 40 * i));
    }
    GUI_DrawLine(80, 40, 80, 200);
    
    GUI_SetFont(&GUI_FontYaHei_Mono28);
    GUI_DispStringHCenterAt("微氧分析仪 ", 160, 9); 
    GUI_DispStringAt("浓度", 9, y_string_coor);
    GUI_DispStringAt("温度", 9, (y_string_coor + 40));
    GUI_DispStringAt("流量", 9, (y_string_coor + 80));
    GUI_DispStringAt("时间", 9, (y_string_coor + 120));
    
    GUI_SetFont(&GUI_Font24B_1);
    /** 此处添加显示氧浓度值代码 */
    display_oxygen_concertration(100, y_string_coor);
    GUI_DispStringAt("%", 259, y_string_coor);
    
    /** 显示温度 */
    display_sensor_value(100, (y_string_coor + 40));
//    display_temperature(100, (y_string_coor + 40));
    GUI_DispStringAt("*C", 259, (y_string_coor + 40));

    /** 此处添加显示流量值代码 */
//    GUI_GotoXY(100, (y_string_coor + 80));
//    GUI_DispFloat(20.39, 6);
    GUI_DispStringAt("L/min", 259, (y_string_coor + 80));
    
    /** 显示时间 */
    RTC_display_current_date(100, (y_string_coor + 120));
    RTC_display_current_time(220, (y_string_coor + 120));
}

/**
 * @brief   初始化对话框
 * @param   *pMsg：消息指针
 * @retval  None
 */ 
static void measure_window_init(WM_MESSAGE *pMsg)
{
    uint8_t i;
    
    /** 设置窗口属性 */
    WINDOW_SetBkColor(WM_GetClientWindow(hWin), GUI_GRAY);
    
    for (i = 0; i <= 3; i++)
    {
        hButton_n[i] = WM_GetDialogItem(pMsg->hWin, (i + GUI_ID_BUTTON0));
        BUTTON_SetFont(hButton_n[i], &GUI_Font13B_1);
    }
}
