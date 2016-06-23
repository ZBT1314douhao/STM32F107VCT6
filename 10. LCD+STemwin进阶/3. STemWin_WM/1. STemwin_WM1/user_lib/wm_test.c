/**
  *
  * @file       wm_test.c 
  * @author     Z B T
  * @version    V0.1
  * @date       2016.06.08
  * @note   
  *
  */  

#include "wm_test.h"
#include "gui.h"
#include "WM.h"
#include "DIALOG.h"
#include "FRAMEWIN.h"

#define WM_TEST_N       2

/**< 不加这个会出现警告 */
WM_HWIN CreateFramewin(void);

static void WM_callback_foreground_handler(WM_MESSAGE *pMsg);
static void WM_callback_background_handler(WM_MESSAGE *pMsg);
static void WM_system_message_instance(void);
static void move_windows(const char *pText);
static void draw_progbar(void);

static void cbBkWin(WM_MESSAGE *pMsg);
static void cbTop(WM_MESSAGE *pMsg);
static void CreateFrameWinMainTask(void);

static WM_HWIN hWin0;
static WM_HWIN hButton0;

/**
 *  @brief  WM测试程序
 *  @param  None
 *  @retval None
 */
void WM_test_function(void)
{
#if (WM_TEST_N == 0)
    {
        CreateFramewin();       /**< 利用GUIBuilder生成的代码 */
    }  
#elif (WM_TEST_N == 1)
    {
        WM_system_message_instance();
    }  
#elif (WM_TEST_N == 2)
    {
        draw_progbar();
    }
#elif (WM_TEST_N == 3)
    {
        CreateFrameWinMainTask();
    }
     

#endif
}    

/** -------------------------- STemwin WM Demo -------------------- */

/**
 *  @brief  背景窗口回调函数
 *  @param  数据类型为WM_MESSAGE的数据指针
 *  @retval None
 */
static void cbBkWin(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:      /**< 窗口变为无效并应重绘时 发送到窗口 */
        GUI_SetBkColor(GUI_BLACK);
        GUI_Clear();
        GUI_SetColor(GUI_RED);
        break;
    case WM_NOTIFY_PARENT:  /**< 告知父窗口 其子窗口发生了改变 */
        if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)       /**< 按键消息函数 */
        {
            if (pMsg->hWinSrc == hButton0)
            {
                WM_InvalidateWindow(hWin0);
            }
            else
            {
                
            }
        }
        break;
    default:
        WM_DefaultProc(pMsg);   /**< 默认的消息句柄 使用此函数可以处理未处理的函数 */
        
    }
}

/**
 *  @brief  始终置于前面的窗口回调函数
 *  @param  数据类型为WM_MESSAGE的数据指针
 *  @retval None
 */
static void cbTop(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetBkColor(GUI_MAGENTA);
        GUI_Clear();
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

/**
 *  @brief  创建框架窗口回调函数
 *  @param  数据类型为WM_MESSAGE的数据指针
 *  @retval None
 */
static void cbFrameWin0(WM_MESSAGE* pMsg) 
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetBkColor(GUI_GRAY);       /**< 设置FrameWindow背景色 */
        GUI_Clear();
        GUI_SetColor(GUI_BLUE); 
        GUI_FillCircle(25, 25, 15);     /**< 绘制内部蓝色填充 黑色轮廓的实心圆 */
        GUI_SetColor(GUI_BLACK);
        GUI_DrawCircle(25, 25, 15);
        WM_InvalidateWindow(WM_HBKWIN); /**< WM_HBKWIN 位背景窗处理程序 */
        break;
    default:
        WM_DefaultProc(pMsg);
    } 
}

/**
 *  @brief  创建框架窗口
 *  @param  None
 *  @retval None
 */
static void CreateFrameWinMainTask(void)
{
    WM_HWIN hWin0;
    WM_HWIN hFrame0;
    
    while (1)
    {
        WM_SetCallback(WM_HBKWIN, cbBkWin);
        /** 第五个参数为0 代表此窗口为桌面(顶级窗口)的子项 */
        hFrame0 = FRAMEWIN_CreateEx(10, 10, 300, 220, 0, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, "Hello FrameWin", cbFrameWin0);
        /** 倒数第二个参数为未使用状态 供日后使用 见手册 */
        hButton0   = BUTTON_CreateEx( 10, 210, 140, 20, 0, WM_CF_SHOW, 0, 1);
        /** WM_CF_STAYONTOP见WM_CreateWindow()函数的Style参数 */
        hWin0 = FRAMEWIN_CreateEx(60, 80, 40, 40, 0, WM_CF_SHOW | WM_CF_STAYONTOP, FRAMEWIN_CF_MOVEABLE, 0, "I am TOP 0", cbTop);
        
        BUTTON_SetText(hButton0, "Click Me");
        /**< 设定给定框架窗口的可缩放状态 1:可缩放 0:不可缩放 */
        FRAMEWIN_SetResizeable(hWin0, 1);
        
        while (1)   /**< 此while死循环不可去除 否则无法显示 */
        {
            GUI_Delay(50);
        }
    }

}

/** --------------------- ProgBar Widget Demo ------------------------- */
/**
 *  @brief  进度条显示0-100% 显示过的为绿色 未显示的为白色
 *  @param  None
 *  @retval None
 */
static void draw_progbar(void)
{
    uint16_t i;
    PROGBAR_Handle hProgBar[2];    /**< 为小工具的句柄保留内存 */
    
    while (1)
    {
        /**< 由窗口管理器调用WM_EXec()时自动绘制小工具 */
        hProgBar[0] = PROGBAR_Create(100, 40, 100, 20, WM_CF_SHOW);
        hProgBar[1] = PROGBAR_Create(200, 80, 100, 20, WM_CF_SHOW);
        PROGBAR_EnableMemdev(hProgBar[0]);          /**< 利用内存设备 效果更好 */
        PROGBAR_EnableMemdev(hProgBar[1]);
        PROGBAR_SetMinMax(hProgBar[0], 0, 500);
        PROGBAR_SetMinMax(hProgBar[1], 0, 500);
        PROGBAR_SetFont(hProgBar[0], &GUI_Font8x16);
        GUI_Delay(500);
        /**< 参数含义 进度条句柄 进度条左侧部分(0) 颜色 */
        PROGBAR_SetBarColor(hProgBar[0], 0, GUI_GREEN);
        /**< 参数含义 进度条句柄 进度条右侧部分(1) 颜色 */
        PROGBAR_SetBarColor(hProgBar[1], 1, GUI_BLUE);
        
        for (i = 0; i <= 500; i++)
        {
            PROGBAR_SetValue(hProgBar[0], i);
            PROGBAR_SetValue(hProgBar[1], i);
            GUI_Delay(5);
        }
//        GUI_Delay(400);
//        /**< 删除进度条 */
//        PROGBAR_Delete(hProgBar[0]);
//        GUI_ClearRect(0, 50, 319, 239);
//        GUI_Delay(750);
    }
}
/**----------------- Redraw or not Redraw --------------------------------------*/
/**
 *  @brief  WM的回调的使用  
 *  @param  None
 *  @note   用于演示两种情况    1 在移动窗口的情况下不做窗口重绘
                                2 在移动窗口的情况下执行窗口重绘
 *  @retval None
 */
static void WM_system_message_instance(void)
{
    WM_CALLBACK *callback_bk;
    
    GUI_SetColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("Hello Windows Manager", 160, 5);
    GUI_SetFont(&GUI_Font8x16);
    
    while (1)
    {
        WM_SetCallback(WM_HBKWIN, callback_bk);
        move_windows("WM has not been redrawn");
        GUI_ClearRect(0, 50, 319, 239);
        GUI_Delay(1000);
        callback_bk = WM_SetCallback(WM_HBKWIN, WM_callback_background_handler);
        move_windows("WM has been redrawn");        
   
    }  
}

/**
 *  @brief  实现窗口的相对移动
 *  @param  const char *pText 移动窗口显示的文本
 *  @retval None
 */
static void move_windows(const char *pText)
{
    uint8_t i;
    GUI_HWIN hWnd;
    
    /** 创建前景窗口 */
    hWnd = WM_CreateWindow(10, 50, 150, 100, WM_CF_SHOW, WM_callback_foreground_handler, 0);
    GUI_Delay(50);
    for (i = 0; i < 40; i++)
    {
        WM_MoveWindow(hWnd, 2, 2);
        GUI_Delay(10);
    }
    if (pText)
    {
        GUI_DispStringAt(pText, 50, 50);
        GUI_Delay(2500);
    }
    WM_DeleteWindow(hWnd);
    WM_Invalidate(WM_HBKWIN);       /**< 使得创建的窗口无效 */
    GUI_Exec();                     /**< 执行重新绘制命令 */

}

/**
 *  @brief  桌面窗口创建回调函数，并在回调函数中清除一块区域
 *  @param  *pMsg    指向WM_MESSAGE的数据结构指针
 *  @retval None
 */
static void WM_callback_background_handler(WM_MESSAGE *pMsg)
{   
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_ClearRect(0, 50, 319, 239);
    break;
    default:
        WM_DefaultProc(pMsg);
    }
}

/**
 *  @brief  在前景窗口创建回调函数
 *  @param  *pMsg    指向WM_MESSAGE的数据结构指针
 *  @retval None
 */
static void WM_callback_foreground_handler(WM_MESSAGE *pMsg)
{    
    GUI_RECT Rect;
    
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        WM_GetInsideRect(&Rect);        /**< 返回活动窗口减去边界后的尺寸(GUI_RECT结构的指针) */
        GUI_SetColor(GUI_GRAY);
        GUI_SetBkColor(GUI_BLUE);
        GUI_ClearRectEx(&Rect);         /**< 清除部分区域 */
        GUI_DrawRectEx(&Rect);          /**< 绘制矩形 参数为结构体类型 */
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font8x16);
        GUI_DispStringAt("Hello WM", 75, 40);
    break;
    default:
        WM_DefaultProc(pMsg);
    }
}
