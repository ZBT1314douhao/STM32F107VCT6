/**
* @file    dialog.c
* @author  Z B T
* @version V0.1
* @date    2016-07-26
* @brief   对话框测试程序
*/
#include "dialog_user.h"

#define DIALOG_TEST_N   3


/**
 *  @brief  STemwin对话框测试程序
 *  @param  None
 *  @retval None
 */
void dialog_test(void)
{
    #if (DIALOG_TEST_N == 0)
    {
        dialog_count();
    }
    #elif (DIALOG_TEST_N == 1)
    {
        dialog_all();
    }
    #elif (DIALOG_TEST_N == 2)
    {
        dialog_slider_color();
    }
    #elif (DIALOG_TEST_N == 3)
    {
        dialog_msg_box();
    }
    #elif (DIALOG_TEST_N == 4)
    {
        dialog_msg_box_user();
    }
    #elif (DIALOG_TEST_N == 5)
    {
        dialog_nested_modal();
    }
    #elif (DIALOG_TEST_N == 6)
    {
        dialog_radio();
    }
    #elif (DIALOG_TEST_N == 7)
    {
    }

       
    #endif   
}




