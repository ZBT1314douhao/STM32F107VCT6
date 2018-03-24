/** 
 *  @file		lcd_ssd1289.h
 *	@brief	lcd驱动程序(SSD1289)
 *	@par		Copyright (c):  
 *          南京工业大学自动化与电气工程学院
 *  @par		硬件连接:
 *  -        stc15      ssd1289 
 *  -        P2      -- DB0~7
 *  -        P0      -- DB8~15  
 *  -        P4_0    -- nCS 
 *  -        P4_1    -- nRS 
 *  -        P4_2    -- nWR
 *  -        P4_3    -- nRD 
 *  -        P4_7    -- nReset 
 *  -        VCC     -- BK_LED
 *	@par		修改日志:
 *					版本			  日期           作者      说明
 *					v0.1			2013-07-06     王晓荣     
 */ 

#ifndef LCD_SSD1289_H
#define LCD_SSD1289_H 

#include "stm32f1xx_hal.h"

#define WHITE          0xFFFF
#define BLACK          0x0000
#define GREY           0xF7DE
#define BLUE           0x001F
#define BLUE2          0x051F
#define RED            0xF800
#define MAGENTA        0xF81F		 /**< 品红 */
#define GREEN          0x07E0
#define CYAN           0x7FFF		 /**< 青色 */
#define YELLOW         0xFFE0
#define HYALINE        0x0001		 /**< 透明 */


struct LCD
{
    GPIO_TypeDef      *port;
    uint16_t          pin;
};

void lcd_config(void);
void lcd_clr_screen(uint16_t color);
void lcd_set_cursor(uint16_t y,uint16_t x);	 
void lcd_set_point(uint16_t y, uint16_t x, uint16_t value);

void lcd_put_char_16x24(uint16_t x, uint16_t y, uint8_t ch, uint16_t ch_color, uint16_t bk_color); 
void lcd_put_string_16x24(uint16_t x, uint16_t y, uint8_t *str,  uint16_t ch_color, uint16_t bk_color);
void lcd_put_data_16x24(uint16_t x, uint16_t y, uint32_t dat, uint8_t len, uint16_t ch_color, uint16_t bk_color);
	


#endif
 
