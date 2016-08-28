/**
 * @file	adjust_time_window.c
 * @brief	RTC时钟时间设置界面
 * @par	date        version     author      remarks
 *		2016-07-24	V1.0		zbt			初次创建
 */

/** -------------------- 头文件包含区 ------------------------------ */
#include "adjust_time_window.h"
#include "sys_info_window.h"
#include "measure_window.h"
#include "rtc_dup.h"

/** -------------------- 文件私有变量 ------------------------------ */
static GUI_WIDGET_CREATE_INFO adj_time_window_interface[] = 
{   /** GUI_WIDGET_CREATE_FUNC* pfCreateIndirect,    
        *pName Id x0, y0, xSize, ySize, Flags, Para, NumExtraBytes */
    { WINDOW_CreateIndirect,  NULL, 0, 0, 0, 320, 240, 0, 0, 0},
    
    { TEXT_CreateIndirect,    "Year", GUI_ID_TEXT0, 0,   15,  40,  30, 0, 0, 0 },
    { TEXT_CreateIndirect,    "Mon",  GUI_ID_TEXT1, 0,   85,  40,  30, 0, 0, 0 },
    { TEXT_CreateIndirect,    "Date", GUI_ID_TEXT2, 0,   155, 40,  30, 0, 0, 0 },
    { TEXT_CreateIndirect,    "Hour", GUI_ID_TEXT3, 160, 15,  40,  30, 0, 0, 0 },
    { TEXT_CreateIndirect,    "Min",  GUI_ID_TEXT4, 160, 85,  40,  30, 0, 0, 0 },
    { TEXT_CreateIndirect,    "Sec",  GUI_ID_TEXT5, 160, 155, 40,  30, 0, 0, 0 },
    
    { SPINBOX_CreateIndirect, NULL, GUI_ID_SPINBOX0, 50,  10,   100, 40, 0, 0, 0 },
    { SPINBOX_CreateIndirect, NULL, GUI_ID_SPINBOX1, 50,  80,   100, 40, 0, 0, 0 },
    { SPINBOX_CreateIndirect, NULL, GUI_ID_SPINBOX2, 50,  150,  100, 40, 0, 0, 0 },
    { SPINBOX_CreateIndirect, NULL, GUI_ID_SPINBOX3, 210, 10,   100, 40, 0, 0, 0 },
    { SPINBOX_CreateIndirect, NULL, GUI_ID_SPINBOX4, 210, 80,   100, 40, 0, 0, 0 },
    { SPINBOX_CreateIndirect, NULL, GUI_ID_SPINBOX5, 210, 150,  100, 40, 0, 0, 0 },
    /** 绘制4个按钮 在屏幕下方均匀分布 */
    { BUTTON_CreateIndirect, "Measure",  GUI_ID_BUTTON0, 9,   205, 60, 30, 0, 0 },
    { BUTTON_CreateIndirect, "Setting",  GUI_ID_BUTTON1, 90,  205, 60, 30, 0, 0 },
    { BUTTON_CreateIndirect, "Ok",       GUI_ID_BUTTON2, 167, 205, 60, 30, 0, 0 },
    { BUTTON_CreateIndirect, "Cancel",   GUI_ID_BUTTON3, 250, 205, 60, 30, 0, 0 },
};

static RTC_TimeTypeDef struct_time;
static RTC_DateTypeDef struct_date;
static WM_HWIN hWin, hWinSrc;
static WM_HWIN hSpinbox_n[6], hText_n[6], hButton_n[4];

/** -------------------- 外部引用变量 ------------------------------ */

/** -------------------- 私有函数原型 ------------------------------ */
static void adj_time_window_info_cb(WM_MESSAGE *pMsg);
static void adj_time_window_paint(WM_MESSAGE *pMsg);
static void adj_time_window_init(WM_MESSAGE *pMsg);
static void spinbox_write_to_rtc(void);
/** -------------------- 公有函数 --------------------------------- */
/**
 * @brief  读取设置时间界面的资源表、空间数量和回调函数地址
 * @param  info:存放上述内容的结构体指针
 * @retval None
 */
void adj_time_window_info(WINDOW_INFO *info)
{
    info->p      = adj_time_window_interface;
    info->length = GUI_COUNTOF(adj_time_window_interface);
    info->func   = &adj_time_window_info_cb;
}

/**
 * @brief  用于修改RTC时钟前将时钟值同步到spinbox
 * @param  None
 * @retval None
 */
void rtc_write_to_spinbox(void)
{
    uint8_t i;
    /** 获取当前日期和时间 */
    struct_date = RTC_get_date();
    struct_time = RTC_get_time();
    
    /** 初始化spinbox */
    for (i = 0; i <= (GUI_ID_SPINBOX5 - GUI_ID_SPINBOX0); i++)
    {   /** 获取spinbox的句柄 设置皮肤和字体*/
        hSpinbox_n[i] = WM_GetDialogItem(hWin, (i + GUI_ID_SPINBOX0));
    }
    
    /** 设置 Spinbox的值为当前时间值 */
    SPINBOX_SetValue(hSpinbox_n[0], struct_date.Year);
    SPINBOX_SetValue(hSpinbox_n[1], struct_date.Month);
    SPINBOX_SetValue(hSpinbox_n[2], struct_date.Date);
    SPINBOX_SetValue(hSpinbox_n[3], struct_time.Hours);
    SPINBOX_SetValue(hSpinbox_n[4], struct_time.Minutes);
    SPINBOX_SetValue(hSpinbox_n[5], struct_time.Seconds);
}

 /** -------------------- 私有函数 --------------------------------- */
 /**
 * @brief   设置界面回调函数
 * @param   *pMsg：消息指针
 * @retval  None
 */
static void adj_time_window_info_cb(WM_MESSAGE *pMsg)
{
    int Id, NCode;
    
    hWin = pMsg->hWin;
    hWinSrc = pMsg->hWinSrc;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        adj_time_window_init(pMsg);
        break;
    case WM_PAINT:
        adj_time_window_paint(pMsg);
        break;
//    case WM_TIMER:    /**< 定时触发窗口重绘 添加后数值连续更改速度变慢 */
//        WM_RestartTimer(pMsg->Data.v, 10);
//        WM_InvalidateWindow(WM_GetClientWindow(hWin));
//        break;
//    case WM_KEY:
//        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(hWinSrc);
        NCode = pMsg->Data.v;
        switch(NCode)
        {
        case WM_NOTIFICATION_RELEASED:
            switch (Id)
            {
//            case GUI_ID_BUTTON0:
//                toggle_interface(pMsg->hWin, measure_window_info, 
//                                TIMER_ENABLE, TIMER_PERIOD);
//                break;
            case GUI_ID_BUTTON1:
                toggle_interface(pMsg->hWin, sys_info_window_info, 
                                TIMER_DISABLE, 0);
                break;
            case GUI_ID_BUTTON2:   /**< 按下OK表示确定要修改时钟 */
                toggle_interface(pMsg->hWin, measure_window_info, 
                                TIMER_ENABLE, TIMER_PERIOD);
            /** 将在修改后的日期和时间值同步到RTC时钟 */
                spinbox_write_to_rtc();
                break;
            case GUI_ID_BUTTON3:    /**< 按下取消按钮 返回测量界面 */
                toggle_interface(pMsg->hWin, measure_window_info, 
                                TIMER_ENABLE, TIMER_PERIOD);  
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

/**
 * @brief   将spinbox中更改的时间日期写入到RTC
 * @param   None
 * @retval  None
 */ 
static void spinbox_write_to_rtc(void)
{   
    struct_date.Year    = SPINBOX_GetValue(hSpinbox_n[0]);
    struct_date.Month   = SPINBOX_GetValue(hSpinbox_n[1]);
    struct_date.Date    = SPINBOX_GetValue(hSpinbox_n[2]);
    struct_time.Hours   = SPINBOX_GetValue(hSpinbox_n[3]);
    struct_time.Minutes = SPINBOX_GetValue(hSpinbox_n[4]);
    struct_time.Seconds = SPINBOX_GetValue(hSpinbox_n[5]); 
    
    /** 修改后的值写入RTC的寄存器中 */
    RTC_set_date(&struct_date);
    RTC_set_time(&struct_time);
}

/**
 * @brief   初始化对话框
 * @param   *pMsg：消息指针
 * @retval  None
 */ 
static void adj_time_window_init(WM_MESSAGE *pMsg)
{
    uint8_t i;
 
    /** 设置窗口属性 */
    WINDOW_SetBkColor(WM_GetClientWindow(hWin), GUI_GRAY);
    
    /** 初始化spinbox */
    for (i = 0; i <= (GUI_ID_SPINBOX5 - GUI_ID_SPINBOX0); i++)
    {   /** 获取spinbox的句柄 设置皮肤和字体*/
        hSpinbox_n[i] = WM_GetDialogItem(hWin, (i + GUI_ID_SPINBOX0));
        SPINBOX_SetEdge(hSpinbox_n[i], SPINBOX_EDGE_CENTER);
        SPINBOX_SetSkin(hSpinbox_n[i], SPINBOX_SKIN_FLEX);
        SPINBOX_SetFont(hSpinbox_n[i], &GUI_Font16B_1);
    }
   /**< 设置各spinbox的范围 */
    SPINBOX_SetRange(hSpinbox_n[0], 0, 99);
    SPINBOX_SetRange(hSpinbox_n[1], 0, 12);
    SPINBOX_SetRange(hSpinbox_n[2], 0, 31);
    SPINBOX_SetRange(hSpinbox_n[3], 0, 23);
    SPINBOX_SetRange(hSpinbox_n[4], 0, 59);
    SPINBOX_SetRange(hSpinbox_n[5], 0, 59);
    
    /** 初始化text */
    for (i = 0; i <= (GUI_ID_TEXT5 - GUI_ID_TEXT0); i++)
    {
        hText_n[i] = WM_GetDialogItem(hWin, (i + GUI_ID_TEXT0));
        TEXT_SetFont(hText_n[i], &GUI_Font20B_1);
        TEXT_SetTextAlign(hText_n[i], GUI_TA_VCENTER | GUI_TA_RIGHT);
    }
    /** 初始化button */
    for (i = 0; i <= (GUI_ID_BUTTON3 - GUI_ID_BUTTON0); i++)
    {
        hButton_n[i] = WM_GetDialogItem(hWin, (i + GUI_ID_BUTTON0));
        BUTTON_SetFont(hButton_n[i], &GUI_Font13B_1);
    }
}

 /**
 * @brief   绘制设置界面
 * @param   pMsg：消息指针
 * @retval  None
 */ 
static void adj_time_window_paint(WM_MESSAGE *pMsg)
{    
    /** 目前看来这个窗口似乎不需要用此函数重绘 */
}
