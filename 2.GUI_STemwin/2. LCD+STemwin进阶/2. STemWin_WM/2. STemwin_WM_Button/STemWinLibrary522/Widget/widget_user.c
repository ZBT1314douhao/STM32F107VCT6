/**
  *
  * @file    widget.c
  * @author  Z B T
  * @version V0.1
  * @date    2016-07-22
  * @brief   
  *
  */  
#include "widget_user.h"

#define WIDGET_TEST_N   0


/**
 *  @brief  STemwin小工具测试程序
 *  @param  None
 *  @retval None
 */
void widget_test(void)
{
    #if (WIDGET_TEST_N == 0)
    {
        button_test();
    }
    #elif (WIDGET_TEST_N == 1)
    {
        Video();
    }
    #elif (WIDGET_TEST_N == 2)
    {
        Sample();
    }
    #elif (WIDGET_TEST_N == 3)
    {
        LateClipping();
    }
    #elif (WIDGET_TEST_N == 4)
    {
        Window();
    }
    #elif (WIDGET_TEST_N == 5)
    {
        TreeviewTryit();
    }
    #elif (WIDGET_TEST_N == 6)
    {
        SortedListview();
    }
    #elif (WIDGET_TEST_N == 7)
    {
        SimpleListBox();
    }
    #elif (WIDGET_TEST_N == 8)
    {
        SCROLLBAR_Move();
    }
    #elif (WIDGET_TEST_N == 9)
    {
        PopupMenu();
    }
    #elif (WIDGET_TEST_N == 10)
    {
        NumPad();
    }
    #elif (WIDGET_TEST_N == 11)
    {
        Multipage();
    }    
    #elif (WIDGET_TEST_N == 12)
    {
        MultiEdit();
    }
    #elif (WIDGET_TEST_N == 13)
    {
        Multiedit();
    }
    #elif (WIDGET_TEST_N == 14)
    {
        Menu();
    }
    #elif (WIDGET_TEST_N == 15)
    {
        ListView();
    }
    #elif (WIDGET_TEST_N == 16)
    {
        ListBoxOwnerDraw();
    }
    #elif (WIDGET_TEST_N == 17)
    {
        ListBox();
    }
    #elif (WIDGET_TEST_N == 18)
    {
        Header();
    }
    #elif (WIDGET_TEST_N == 19)
    {
        GraphYT();
    }
    #elif (WIDGET_TEST_N == 20)
    {
        GraphXY();
    }
    #elif (WIDGET_TEST_N == 21)
    {
        FrameWin();
    }
    #elif (WIDGET_TEST_N == 21)
    {
        Effect();
    }
    #elif (WIDGET_TEST_N == 22)
    {
        EditWinmode();
    }
    #elif (WIDGET_TEST_N == 23)
    {
        Edit();
    }
    #elif (WIDGET_TEST_N == 24)
    {
        Dropdown();
    }
    #elif (WIDGET_TEST_N == 25)
    {
        CustomWidgetType();
    }
    #elif (WIDGET_TEST_N == 26)
    {
        CustomTreeview();
    }
    #elif (WIDGET_TEST_N == 27)
    {
        CustomEffect();
    }
    #elif (WIDGET_TEST_N == 28)
    {
        PhoneButton();
    }
    #elif (WIDGET_TEST_N == 29)
    {
        Spinbox();
    }    
    #endif   
}




