/**
 * @file    ad7705.c
 * @author  zbt
 * @version v0.01
 * @date    2016-07-11
 * @brief   ad7705驱动程序
 */
 
#include "ad7705.h"
#include "delay.h"

/** ----------------------- 私有宏定义 ------------------------ */

extern SPI_HandleTypeDef hspi2;

/** ----------------------- 私有函数原型 ------------------------ */
static uint8_t AD7705_DRDY_read(void);
static uint8_t AD7705_read_byte(void);
static uint16_t AD7705_read_2byte(void);
static void AD7705_write_byte(uint8_t data);

/** ----------------------- 驱动程序接口 ------------------------ */
/**
  * @brief  AD7705初始化
  * @param  None
  * @return None
  */
void AD7705_init(void)
{
    AD7705_write_byte(CLOCK_REG | WRITE_REG | CHANNEL_0);
    
    AD7705_write_byte(MASTER_CLK_EN | CLK_FREQ_4_9152M | OUT_UPDATE_RATE_50HZ);
    
    AD7705_self_calibration(1);
    delay_ms(20);
}
 
/**
  * @brief  获取AD7705通道采集到值
  * @param  channel：1 or 2
  * @return ad_value：采集到的值
  */
uint16_t AD7705_get_adc_value(uint8_t channel)
{
    uint8_t i;
    uint16_t ad_value = 0;
    
    /** 为了避免通道切换造成读数失效，每个通道读两次 */
    for (i = 0; i < 2; i++)
    {
        if (AD7705_DRDY_read())
        {
            if (channel == 1)
            {
                AD7705_write_byte(0x38);
            }
            else if (channel == 2)
            {
                AD7705_write_byte(0x39);
            }
            else
            {
                ;
            }
        }

        ad_value = AD7705_read_2byte();
    }
    
    return ad_value;
}

/** ---------------------- 私有函数区 --------------------------- */
/**
  * @brief  AD7705自校准
  * @param  channel：1 or 2
  * @return None
  * @note   自校准包括零位校准和满量程校准，校准时间约200ms
  */
void AD7705_self_calibration(uint8_t channel)
{
    if (channel == 1)
    {
        AD7705_write_byte(STEUP_REG | WRITE_REG | CHANNEL_0);
        AD7705_write_byte(SELF_CALIB_MODE | SET_GAIN_1 | UNIPOLAR 
                            | BUFFER_ON | FILTER_SYNC_ON);
        while (AD7705_DRDY_read());
    }
}

/**
  * @brief  读取DRDY引脚的状态
  * @param  None
  * @return None
  */
static uint8_t AD7705_DRDY_read(void)
{
    uint32_t i;
    
    for (i = 0; i < 4000000; i++)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET)
        {
            return 1;
        }
        else 
        {
            return 0;
        }
    }
    
    return 0;    
}  

/**
  * @brief  AD7705写一个字节的数据，利用SPI2
  * @param  data：要写入寄存器的数据
  * @return None
  * @note   此函数为在SPI2的轮询模式下配置，如使用其他模式，
            需更换其实现函数
  */
static void AD7705_write_byte(uint8_t data)
{
    HAL_SPI_Transmit(&hspi2, (uint8_t*)&data, 1, 5000);
}

/**
  * @brief  AD7705读一个字节的数据，利用SPI2
  * @param  None
  * @return data：从SPI总线/寄存器上读取到的数据
  * @note   此函数为在SPI2的轮询模式下配置，如使用其他模式，
            需更换其实现函数
  */
static uint8_t AD7705_read_byte(void)
{
    uint8_t data;
    
    HAL_SPI_Receive(&hspi2, (uint8_t*)&data, 1, 5000);
    
    return data;
}

/**
  * @brief  读取AD7705的16位数据寄存器
  * @param  None
  * @return data：从数据寄存器中读取到的数据
  */
static uint16_t AD7705_read_2byte(void)
{
    uint8_t  high_byte, low_byte;
    uint16_t data;
    
    high_byte = AD7705_read_byte();
    low_byte  = AD7705_read_byte();

    data = ((high_byte << 8) | low_byte);
    
    return data; 
} 




