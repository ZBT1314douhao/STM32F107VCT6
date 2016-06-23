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

/**< �����������־��� */
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
 *  @brief  WM���Գ���
 *  @param  None
 *  @retval None
 */
void WM_test_function(void)
{
#if (WM_TEST_N == 0)
    {
        CreateFramewin();       /**< ����GUIBuilder���ɵĴ��� */
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
 *  @brief  �������ڻص�����
 *  @param  ��������ΪWM_MESSAGE������ָ��
 *  @retval None
 */
static void cbBkWin(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:      /**< ���ڱ�Ϊ��Ч��Ӧ�ػ�ʱ ���͵����� */
        GUI_SetBkColor(GUI_BLACK);
        GUI_Clear();
        GUI_SetColor(GUI_RED);
        break;
    case WM_NOTIFY_PARENT:  /**< ��֪������ ���Ӵ��ڷ����˸ı� */
        if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)       /**< ������Ϣ���� */
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
        WM_DefaultProc(pMsg);   /**< Ĭ�ϵ���Ϣ��� ʹ�ô˺������Դ���δ����ĺ��� */
        
    }
}

/**
 *  @brief  ʼ������ǰ��Ĵ��ڻص�����
 *  @param  ��������ΪWM_MESSAGE������ָ��
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
 *  @brief  ������ܴ��ڻص�����
 *  @param  ��������ΪWM_MESSAGE������ָ��
 *  @retval None
 */
static void cbFrameWin0(WM_MESSAGE* pMsg) 
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetBkColor(GUI_GRAY);       /**< ����FrameWindow����ɫ */
        GUI_Clear();
        GUI_SetColor(GUI_BLUE); 
        GUI_FillCircle(25, 25, 15);     /**< �����ڲ���ɫ��� ��ɫ������ʵ��Բ */
        GUI_SetColor(GUI_BLACK);
        GUI_DrawCircle(25, 25, 15);
        WM_InvalidateWindow(WM_HBKWIN); /**< WM_HBKWIN λ������������� */
        break;
    default:
        WM_DefaultProc(pMsg);
    } 
}

/**
 *  @brief  ������ܴ���
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
        /** ���������Ϊ0 ����˴���Ϊ����(��������)������ */
        hFrame0 = FRAMEWIN_CreateEx(10, 10, 300, 220, 0, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, "Hello FrameWin", cbFrameWin0);
        /** �����ڶ�������Ϊδʹ��״̬ ���պ�ʹ�� ���ֲ� */
        hButton0   = BUTTON_CreateEx( 10, 210, 140, 20, 0, WM_CF_SHOW, 0, 1);
        /** WM_CF_STAYONTOP��WM_CreateWindow()������Style���� */
        hWin0 = FRAMEWIN_CreateEx(60, 80, 40, 40, 0, WM_CF_SHOW | WM_CF_STAYONTOP, FRAMEWIN_CF_MOVEABLE, 0, "I am TOP 0", cbTop);
        
        BUTTON_SetText(hButton0, "Click Me");
        /**< �趨������ܴ��ڵĿ�����״̬ 1:������ 0:�������� */
        FRAMEWIN_SetResizeable(hWin0, 1);
        
        while (1)   /**< ��while��ѭ������ȥ�� �����޷���ʾ */
        {
            GUI_Delay(50);
        }
    }

}

/** --------------------- ProgBar Widget Demo ------------------------- */
/**
 *  @brief  ��������ʾ0-100% ��ʾ����Ϊ��ɫ δ��ʾ��Ϊ��ɫ
 *  @param  None
 *  @retval None
 */
static void draw_progbar(void)
{
    uint16_t i;
    PROGBAR_Handle hProgBar[2];    /**< ΪС���ߵľ�������ڴ� */
    
    while (1)
    {
        /**< �ɴ��ڹ���������WM_EXec()ʱ�Զ�����С���� */
        hProgBar[0] = PROGBAR_Create(100, 40, 100, 20, WM_CF_SHOW);
        hProgBar[1] = PROGBAR_Create(200, 80, 100, 20, WM_CF_SHOW);
        PROGBAR_EnableMemdev(hProgBar[0]);          /**< �����ڴ��豸 Ч������ */
        PROGBAR_EnableMemdev(hProgBar[1]);
        PROGBAR_SetMinMax(hProgBar[0], 0, 500);
        PROGBAR_SetMinMax(hProgBar[1], 0, 500);
        PROGBAR_SetFont(hProgBar[0], &GUI_Font8x16);
        GUI_Delay(500);
        /**< �������� ��������� ��������ಿ��(0) ��ɫ */
        PROGBAR_SetBarColor(hProgBar[0], 0, GUI_GREEN);
        /**< �������� ��������� �������Ҳಿ��(1) ��ɫ */
        PROGBAR_SetBarColor(hProgBar[1], 1, GUI_BLUE);
        
        for (i = 0; i <= 500; i++)
        {
            PROGBAR_SetValue(hProgBar[0], i);
            PROGBAR_SetValue(hProgBar[1], i);
            GUI_Delay(5);
        }
//        GUI_Delay(400);
//        /**< ɾ�������� */
//        PROGBAR_Delete(hProgBar[0]);
//        GUI_ClearRect(0, 50, 319, 239);
//        GUI_Delay(750);
    }
}
/**----------------- Redraw or not Redraw --------------------------------------*/
/**
 *  @brief  WM�Ļص���ʹ��  
 *  @param  None
 *  @note   ������ʾ�������    1 ���ƶ����ڵ�����²��������ػ�
                                2 ���ƶ����ڵ������ִ�д����ػ�
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
 *  @brief  ʵ�ִ��ڵ�����ƶ�
 *  @param  const char *pText �ƶ�������ʾ���ı�
 *  @retval None
 */
static void move_windows(const char *pText)
{
    uint8_t i;
    GUI_HWIN hWnd;
    
    /** ����ǰ������ */
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
    WM_Invalidate(WM_HBKWIN);       /**< ʹ�ô����Ĵ�����Ч */
    GUI_Exec();                     /**< ִ�����»������� */

}

/**
 *  @brief  ���洰�ڴ����ص����������ڻص����������һ������
 *  @param  *pMsg    ָ��WM_MESSAGE�����ݽṹָ��
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
 *  @brief  ��ǰ�����ڴ����ص�����
 *  @param  *pMsg    ָ��WM_MESSAGE�����ݽṹָ��
 *  @retval None
 */
static void WM_callback_foreground_handler(WM_MESSAGE *pMsg)
{    
    GUI_RECT Rect;
    
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        WM_GetInsideRect(&Rect);        /**< ���ػ���ڼ�ȥ�߽��ĳߴ�(GUI_RECT�ṹ��ָ��) */
        GUI_SetColor(GUI_GRAY);
        GUI_SetBkColor(GUI_BLUE);
        GUI_ClearRectEx(&Rect);         /**< ����������� */
        GUI_DrawRectEx(&Rect);          /**< ���ƾ��� ����Ϊ�ṹ������ */
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font8x16);
        GUI_DispStringAt("Hello WM", 75, 40);
    break;
    default:
        WM_DefaultProc(pMsg);
    }
}
