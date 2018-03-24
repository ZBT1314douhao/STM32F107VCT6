/**
  * @file       ui_driver.c 
  * @author     Z B T
  * @version    V0.1
  * @date       2016- 04
  * @brief      AD转换处理程序 DMA方式
  *
  */  

#include "gui.h"
#include "cmsis_os.h"
#include "adc_dup.h"

/**
 * @brief  emwin初始化
 * @param  None  		
 * @return None
*/
void ui_init(void)
{
	GUI_Init();
	/** 采用UTF8编码 */
	GUI_UC_SetEncodeUTF8();
	//WM_SetCreateFlags(WM_CF_MEMDEV);    /**< 开启内存支持, 防刷屏闪烁 */
	GUI_SetColor(GUI_RED);
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font32B_ASCII);
}







