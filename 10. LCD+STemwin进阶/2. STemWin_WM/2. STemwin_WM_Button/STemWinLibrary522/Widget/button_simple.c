/**
  *
  * @file    button.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016-07-22
  * @brief   
  *
  */  
#include "GUI.h"
#include "button.h"
#include "widget.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontMicrosoftYaHeiMono16;

static void create_button(void);

/**
 *  @brief  简单的按钮测试程序
 *  @param  None
 *  @retval None
 */
void button_test(void)
{
    GUI_Init();
    GUI_SetBkColor(GUI_BLACK);
    GUI_UC_SetEncodeUTF8();
    GUI_SetColor(GUI_WHITE);
    
    while (1)
    {
        create_button();
    }
}

/**
 *  @brief  创建按钮
 *  @param  None
 *  @retval None
 */
static void create_button(void)
{
	BUTTON_Handle hButton;

	GUI_SetFont(&GUI_Font20B_1);
	GUI_DispStringHCenterAt("Click on button", 160, 90);

	hButton = BUTTON_Create(110, 110, 100, 40, GUI_ID_OK, WM_CF_SHOW);  /**< 创建确定按钮 */  
    BUTTON_SetFont(hButton, &GUI_Font20B_1);
    /**< 按钮控件不能显示中文  待解决 */
//    BUTTON_SetFont(hButton, &GUI_FontMicrosoftYaHeiMono16);
	BUTTON_SetText(hButton, "Click Me");
    /** 等待按键被按下 按下：1，未按下：0  GUI_ID_OK：“确定”按钮小工具的ID */
	while (GUI_WaitKey() != GUI_ID_OK)
	{
		BUTTON_Delete(hButton);
		GUI_ClearRect(0, 50, 319, 239);
		GUI_Delay(1000);
	}
}
