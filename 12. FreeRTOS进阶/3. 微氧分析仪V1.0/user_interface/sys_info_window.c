/**
  * @file	sys_info_window.c 
  * @brief	系统信息界面
  * @par	date        version     author      remarks
  *			2016-07-25	V1.0		zbt			初次创建
  */

/** -------------------- 头文件包含区 ------------------------------ */
#include "sys_info_window.h"
#include "measure_window.h"
#include "adjust_time_window.h"

/** -------------------- 文件私有变量 ------------------------------ */
/** 定义菜单ID */
#define ID_MENU_MEASURE     (GUI_ID_USER + 0)
#define ID_MENU_ADJ_TIME    (GUI_ID_USER + 1)
#define ID_MENU_HELP        (GUI_ID_USER + 2)

typedef struct 
{
  char      *sText;
  uint16_t  Id;
  uint16_t  Flags;
} MENU_ITEM;

static MENU_ITEM menu_items[] = 
{
    {"Measure",  ID_MENU_MEASURE,   0, },
    {"Adj time", ID_MENU_ADJ_TIME,  0, },
    { 0,         0,                 MENU_IF_SEPARATOR },
    {"Help",     ID_MENU_HELP,      0, },
};

static GUI_WIDGET_CREATE_INFO sys_info_window_interface[] = 
{   /** GUI_WIDGET_CREATE_FUNC* pfCreateIndirect,
        *pName Id x0, y0, xSize, ySize, Flags, Para, NumExtraBytes */
    { WINDOW_CreateIndirect,    NULL,       0, 0, 0, 320, 240, 0, 0, 0 },
    /** 创建菜单 */
//    { MENU_CreateIndirect,      "measure",  ID_MENU_MEASURE,  0, 0, 0,  0, 0, 0, 0 },
//    { MENU_CreateIndirect,      "Setting",  ID_MENU_ADJ_TIME, 0, 0, 0,  0, 0, 0, 0 },
};

static WM_HWIN hWin;
/** static WM_HWIN hWinSrc; */


/** -------------------- 外部引用变量 ------------------------------ */
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFounderMiaoWu28;

/** -------------------- 私有函数原型 ------------------------------ */
static void sys_info_window_info_cb(WM_MESSAGE *pMsg);
static void sys_info_window_paint(WM_MESSAGE *pMsg);
static void sys_info_window_init(WM_MESSAGE *pMsg);

static void add_menu_item(MENU_Handle hMenu, MENU_Handle hSubMenu, 
                        const char* pText, uint16_t Id, uint16_t Flags);
static void open_popup_menu(WM_HWIN hParent, MENU_ITEM * pMenuItems, 
                            int NumItems, int x, int y);
/** -------------------- 公有函数 --------------------------------- */
/**
 * @brief  读取设置界面的资源表、空间数量和回调函数地址
 * @param  info:存放上述内容的结构体指针
 * @retval None
 */
void sys_info_window_info(WINDOW_INFO *info)
{
    info->p      = sys_info_window_interface;
    info->length = GUI_COUNTOF(sys_info_window_interface);
    info->func   = &sys_info_window_info_cb;
}
 /** -------------------- 私有函数 --------------------------------- */
 /**
 * @brief   设置界面回调函数
 * @param   *pMsg：消息指针
 * @retval  None
 */
static void sys_info_window_info_cb(WM_MESSAGE *pMsg)
{
//    int Id, NCode;
    MENU_MSG_DATA* pData;
    
    hWin = pMsg->hWin;   
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        sys_info_window_init(pMsg);
        break;
    case WM_PAINT:
        sys_info_window_paint(pMsg);
        break;
//    case WM_TIMER:
//        break;
//    case WM_KEY:
//        break;
//    case WM_NOTIFY_PARENT:    
//        break;
    case WM_MENU:
        pData = (MENU_MSG_DATA*)pMsg->Data.p;
        switch (pData->MsgType)
        {
        case MENU_ON_ITEMSELECT:
            switch (pData->ItemId)
            {
            case ID_MENU_MEASURE:   /**< 切换到测量界面 */
                toggle_interface(hWin, measure_window_info, 
                TIMER_ENABLE, TIMER_PERIOD);
                break;              
            case ID_MENU_ADJ_TIME:  /**< 切换到调时界面 */
                toggle_interface(hWin, adj_time_window_info, 
                TIMER_ENABLE, TIMER_PERIOD);
                break;
            case ID_MENU_HELP:
                GUI_MessageBox("This is a popup menu,\n\
if you  have any question about this project,\n\
please contact zbt,\n\
E-Mail is displayed in sys_info_window", "Help", GUI_MESSAGEBOX_CF_MOVEABLE);
                break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
        }
        break;
    case WM_PID_STATE_CHANGED:  
        open_popup_menu(hWin, menu_items, GUI_COUNTOF(menu_items), 
                        ((WM_PID_STATE_CHANGED_INFO*)pMsg->Data.p)->x,
                        ((WM_PID_STATE_CHANGED_INFO*)pMsg->Data.p)->y);
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

 /**
 * @brief   绘制设置界面
 * @param   *pMsg：消息指针
 * @retval  None
 */
static void sys_info_window_paint(WM_MESSAGE *pMsg)
{
    uint8_t i;
    const uint8_t y_line_coor = 40;
    const uint8_t y_string_coor = 50;
    
    GUI_SetColor(GUI_BLACK);
    GUI_SetPenSize(2);                  /**< 设置画笔大小(单位：像素) */

    for (i = 0; i <= 5; i++)             /**< 绘制主界面分割线 */
    {
        GUI_DrawLine(0, (y_line_coor + 40 * i), 320, (y_line_coor + 40 * i));
    }
    GUI_DrawLine(70, 40, 70, 240);
    
    GUI_SetFont(&GUI_Font24B_1);
    GUI_DispStringAt("Oxygen analysis instrument", 16, 8);
    
    GUI_SetFont(&GUI_FontFounderMiaoWu28);
    GUI_DispStringAt("主控", 9, y_string_coor);
    GUI_DispStringAt("界面", 9, (y_string_coor + 40));
    GUI_DispStringAt("地址", 9, (y_string_coor + 80));
    GUI_DispStringAt("配置", 9, (y_string_coor + 120));
    GUI_DispStringAt("编写", 9, (y_string_coor + 160));

    GUI_SetFont(&GUI_Font24B_1);
    GUI_DispStringAt("STM32F107VCT6", 80, y_string_coor);
    GUI_DispStringAt("STemwin V5.22", 80, (y_string_coor + 40));    
    GUI_DispStringAt("649503571@163.com", 80, (y_string_coor + 80));    
    GUI_DispStringAt("STM32Cube MX V4.16", 80, (y_string_coor + 120));
    GUI_DispStringAt("MDK V5.20", 80, (y_string_coor + 160));
}

/**
 * @brief   初始化对话框
 * @param   *pMsg：消息指针
 * @retval  None
 */ 
static void sys_info_window_init(WM_MESSAGE *pMsg)
{
    /** 设置窗口属性 */
    WINDOW_SetBkColor(WM_GetClientWindow(hWin), GUI_GRAY);   
}

/**
 * @brief   为给定菜单添加菜单项
 * @param   hMenu：菜单句柄
            hSubMenu：子菜单句柄
            *pText：菜单文本指针
            Id：菜单ID
            Flags：MENU_IF_DISABLED or MENU_IF_SEPARATOR 
                    参见MENU_ITEM_DATA的数据结构
 * @retval  None
 */ 
static void add_menu_item(MENU_Handle hMenu, MENU_Handle hSubMenu, 
                            const char* pText, uint16_t Id, uint16_t Flags)
{
    MENU_ITEM_DATA Item;
    
    Item.pText    = pText;
    Item.hSubmenu = hSubMenu;
    Item.Flags    = Flags;
    Item.Id       = Id;
    
    MENU_AddItem(hMenu, &Item);
}

/**
 * @brief   打开弹出式菜单 
 * @param   hParent：父窗口句柄
            pMenuItems：指向MENU_ITEM的指针
            NumItems：菜单项数量
            x：菜单的x坐标
            y：菜单的y坐标
 * @retval  None
 */
static void open_popup_menu(WM_HWIN hParent, MENU_ITEM * pMenuItems, 
                            int NumItems, int x, int y)
{
    uint8_t i;
    static MENU_Handle hMenu;
    
    if (!hMenu)
    {   /** WM_HBKWIN  WM_UNATTACHED  */
        hMenu = MENU_CreateEx(0, 0, 0, 0, 0, 0, MENU_CF_VERTICAL, 0);
        MENU_SetFont(hMenu, &GUI_Font24B_1);
        for (i = 0; i < NumItems; i++)
        {
            add_menu_item(hMenu, 0, pMenuItems[i].sText, pMenuItems[i].Id, pMenuItems[i].Flags);      
        }
    }
    
    MENU_Popup(hMenu, hParent, x, y, 0, 0, 0);
}
