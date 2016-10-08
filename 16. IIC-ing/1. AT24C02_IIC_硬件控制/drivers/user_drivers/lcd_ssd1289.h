/**
  * @file  lcd_ssd1289.h
  * @brief lcd驱动程序(SSD1289)
  * @par   date        version    author    remarks
  *        2014-04-02  v0.2       王晓荣    初次创建
  *        2016-05-18  v1.0       zbt       修改程序风格，移植到Cube平台   
  *
  */
  
#ifndef __LCD_SSD1289_H
#define __LCD_SSD1289_H 

/** 头文件包含区 ------------------------------------------------ */
#include "stm32f1xx_hal.h"

/** 公有宏(类型定义) -------------------------------------------- */ 
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

/** 枚举、结构体 ------------------------------------------------ */ 

/** 公有函数原型 ------------------------------------------------ */
void lcd_test(void);
void lcd_config(void);
void lcd_clr(uint16_t color);
void lcd_set_cursor(uint16_t x,uint16_t y);	 
void lcd_set_windows(uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y);
uint16_t lcd_get_point(uint16_t x,uint16_t y);
void lcd_set_point(uint16_t x, uint16_t y, uint16_t value);
void lcd_draw_picture(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t *picture);
void lcd_putchar_8x16(uint16_t x, uint16_t y, uint8_t ch, uint16_t ch_color, uint16_t bk_color);
void lcd_putchar_16x24(uint16_t x, uint16_t y, uint8_t ch, uint16_t ch_color, uint16_t bk_color); 

/** 用于STemwin */
void lcd_write_addr(uint16_t dat);
void lcd_write_data(uint16_t dat);
void lcd_read_data_multiple(uint16_t *p_dat, uint16_t num_items);

#endif 
 
