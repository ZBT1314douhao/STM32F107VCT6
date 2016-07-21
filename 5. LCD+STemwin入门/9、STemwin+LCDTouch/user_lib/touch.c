/**
 * @file    touch.c
 * @brief   触摸屏驱动
 * @author  王晓荣    2014-05-01           初次创建
 * @version V0.0.2    2016.07.10   zbt     修改程序风格&从V3.5库移植到Cube平台 
                                            修复LCDConf_FlexColor_Template.c的液晶配置错误
 */
#include "touch.h"
#include "lcd_ssd1289.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "gui.h"

extern SPI_HandleTypeDef hspi3;


static uint8_t  spi_write_read_byte(uint8_t byte);
static uint16_t touch_read_ad(uint8_t cmd);
static uint16_t touch_read_xy(uint8_t cmd);

#define TOUCH_SPI_X                 SPI3  
#define TOUCH_PORT                  GPIOC                
#define TOUCH_INT_PIN               GPIO_PIN_6 
#define TOUCH_CS_PIN                GPIO_PIN_8            

#define CMD_READ_X                  0X90 
#define CMD_READ_Y                  0XD0 

#define TOUCH_READ_TIMES            12          /**< 读取次数 */
#define TOUCH_LOST_VALUE            4           /**< 舍去左右两端的最(大、小)、次(大、小)共四个值 */

/** --------------------- 外部接口函数 --------------------------------- */
/**
 * @brief   读取X方向AD采样值 
 * @param   None
 * @retval  X方向的AD采样值
 */
uint16_t touch_read_x(void)
{
	return touch_read_xy(CMD_READ_X);
} 

/**
 * @brief   读取Y方向AD采样值
 * @param   None
 * @retval  Y方向的AD采样值
 */
uint16_t touch_read_y(void)
{
	return touch_read_xy(CMD_READ_Y);
}

/** 以下为测试SPI是否配置成功的代码 zbt */
/**
 * @brief   读取液晶显示屏的ID
 * @param   None
 * @retval  读取到的设备ID：0x8989
 */
uint16_t read_lcd_id(void)
{
    uint8_t dummy_byte = 0xff;
    uint16_t device_id;
    
    HAL_GPIO_WritePin(TOUCH_PORT, TOUCH_CS_PIN, GPIO_PIN_RESET);   
    spi_write_read_byte(0x0000);
    /** 第一个字节为无效字节 */
    spi_write_read_byte(dummy_byte);
    /** 读取lcd的Device ID */
    device_id = spi_write_read_byte(dummy_byte);
    device_id = device_id << 8;
    device_id |= spi_write_read_byte(dummy_byte);  

    HAL_GPIO_WritePin(TOUCH_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);

    return device_id;
}
/** 以上为测试SPI是否配置成功的代码 zbt */
/** --------------------- 私有函数 --------------------------------- */

/**
 * @brief   读取X或者Y方向AD采样值
 * @param   cmd 读取X方向或者Y方向的命令
 * @retval  读取到的采样值得平均值
 */
static uint16_t touch_read_xy(uint8_t cmd)
{
	uint16_t i, j;
	uint16_t ad_buf[TOUCH_READ_TIMES];
	uint16_t temp, ad_value;
    
    /** 采TOUCH_READ_TIMES个点 */   
	for(i = 0; i < TOUCH_READ_TIMES; i++)
    {
        ad_buf[i] = touch_read_ad(cmd);
    }	    
    /** 升序排列 */    
	for(i = 0; i < (TOUCH_READ_TIMES - 1); i++)
	{
		for(j = i + 1; j < TOUCH_READ_TIMES; j++)
		{
			if(ad_buf[i] > ad_buf[j])
			{
				temp   = ad_buf[i];
				ad_buf[i] = ad_buf[j];
				ad_buf[j] = temp;
			}
		}
	}	
    /** 去掉两端极值 */
    for (i = (TOUCH_LOST_VALUE - 2); i < (TOUCH_READ_TIMES - 2); i++)
    {
        ad_value += ad_buf[i];
    }
    ad_value >>= 3;             /**< 取剩余值得平均值 */

    return ad_value;
} 

/**
 * @brief   利用SPI读写1个字节
 * @param   byte 写入1个字节
 * @retval  读1个字节 
 */
static uint8_t spi_write_read_byte(uint8_t tx_byte)
{
    uint8_t rx_byte;
    
    HAL_SPI_TransmitReceive(&hspi3, &tx_byte, &rx_byte, 1, 1000);
    
    return rx_byte;
}

/**
 * @brief   读ad值 
 * @param   cmd 读取命令
 * @retval  读取到的AD值
 */
static uint16_t touch_read_ad(uint8_t cmd)	  
{ 
    uint16_t ad;

    HAL_GPIO_WritePin(TOUCH_PORT, TOUCH_CS_PIN, GPIO_PIN_RESET);
    spi_write_read_byte(cmd);
    ad = spi_write_read_byte(0x00);
    ad <<= 8;
    ad |= spi_write_read_byte(0x00);
    ad >>= 3;
    HAL_GPIO_WritePin(TOUCH_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);
    
    return ad;     
}


