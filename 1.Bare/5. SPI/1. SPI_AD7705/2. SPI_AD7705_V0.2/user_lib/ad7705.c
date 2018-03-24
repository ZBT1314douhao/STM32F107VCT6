/**
 * @file    ad7705.c
 * @author  zbt
 * @version v0.01
 * @date    2016-07-11
 * @brief   ad7705驱动程序
 */
/** 头文件包含区 --------------------------------------------- */ 
#include "ad7705.h"
#include "delay.h"

/** 私有宏定义 ----------------------------------------------- */
#define AD7705_DEBUGE_MODE              /**< 驱动程序调试模式 */
#define SPI_POLLING_TIMEOUT     5000    /**< 单位：ms */

/** 外部变量 ------------------------------------------------ */
extern SPI_HandleTypeDef hspi1;

/** 私有函数原型--------------------------------------------- */
static uint8_t AD7705_DRDY_read(void);
static uint8_t AD7705_read_byte(void);
static uint16_t AD7705_read_2byte(void);
static void AD7705_write_byte(uint8_t data);
static void AD7705_spi_sync(void);

/** 驱动程序接口 -------------------------------------------- */
/**
  * @brief  AD7705初始化
  * @param  None
  * @return None
  */
void AD7705_init(void)
{
    uint8_t clk_read_byte;
    
    AD7705_spi_sync();      /**< 同步SPI接口时序 */
    
    #ifdef AD7705_DEBUGE_MODE
    /** 读取初始状态时的时钟寄存器的值，用于测试SPI通讯是否成功 */
    AD7705_write_byte(CLOCK_REG | READ_REG);
    clk_read_byte = AD7705_read_byte();
    printf("clk register: %d\n", clk_read_byte);
    if (clk_read_byte == 0x05)
    {
        printf("SPI communication is succeed\n");
    }
    else
    {
        printf("SPI communication is failed\n");
    }
    #endif
    
    /** 激活通道0 下一步操作是写时钟寄存器 */
    AD7705_write_byte(CHANNEL_0 | WRITE_REG | CLOCK_REG);
    /** 主时钟输出使能 输出更新速率50Hz */
    AD7705_write_byte(MASTER_CLK_EN | CLK_FREQ_4_9152M 
                        | CLK_BIT_VALUE | OUT_UPDATE_RATE_50HZ);
    
    /** 上电后进行自校准 */
    AD7705_self_calibration(1);
    delay_ms(20);
    printf("AD7705_self_calibration: OK!\n");
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
        if (AD7705_DRDY_read() == 0)
        {
            if (channel == 1)
            {   /** 读取AIN1+和AIN1-通道的数据寄存器的值 */
                AD7705_write_byte(DATA_REG | READ_REG | CHANNEL_0);
            }
            else if (channel == 2)
            {
                /** 读取AIN2+和AIN2-通道的数据寄存器的值 */
                AD7705_write_byte(DATA_REG | READ_REG | CHANNEL_1);
            }
            else
            {
                printf("channel parameter is error!");
            }
        }

        ad_value = AD7705_read_2byte();
    }
    printf("ad_value is %d\n", ad_value);
    
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
        /** 激活通道0 下一步是写设置寄存器 */
        AD7705_write_byte(CHANNEL_0 | WRITE_REG | STEUP_REG);
        /** 使能自校准工作模式，设置增益为1，单极性，开缓冲和滤波器同步 */
        AD7705_write_byte(SELF_CALIB_MODE | SET_GAIN_1 | UNIPOLAR 
                            | BUFFER_ON | FILTER_SYNC_ON);
        
        while (AD7705_DRDY_read() == 1)
        {
            printf("Calibration is in progress\n");
        }
    }
    else
    {
        ;
    }
}

/**
  * @brief  读取DRDY引脚的状态
  * @param  None
  * @return None
  */
static uint8_t AD7705_DRDY_read(void)
{
    uint32_t i = 0;
    uint8_t pin_status;
    
    for (i = 0; i < 4000000; i++)
    {
        pin_status = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
        if (pin_status == GPIO_PIN_RESET)
        {
            printf("DRDY is low\n");
            break;
        }
        else if (pin_status == GPIO_PIN_SET)
        {
            if (i % 100 == 0)
            {
                printf("DRDY is high\n");
            } 
        }
        else
        {
            ;
        }
    }
    /** 超时 */
    if (i >= 4000000)   
    {
        pin_status = 1;
        printf("AD7705_DRDY_read time out\n");
    }
    
    return pin_status;    
}  

/**
  * @brief  AD7705写一个字节的数据，利用SPI1
  * @param  data：要写入寄存器的数据
  * @return None
  * @note   此函数为轮询模式下配置，如使用其他模式，
            需更换其实现函数，超时时间为5s
  */
static void AD7705_write_byte(uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, (uint8_t*)&data, 1, SPI_POLLING_TIMEOUT);
}

/**
  * @brief  AD7705读一个字节的数据，利用SPI1
  * @param  None
  * @return data：从SPI总线/寄存器上读取到的数据
  * @note   此函数为轮询模式下配置，如使用其他模式，
            需更换其实现函数，超时时间为5s
  */
static uint8_t AD7705_read_byte(void)
{
    uint8_t data;
    
    HAL_SPI_Receive(&hspi1, (uint8_t*)&data, 1, SPI_POLLING_TIMEOUT);
    
    return data;
}

/**
  * @brief  读取AD7705的16位数据寄存器（只读）
  * @param  None
  * @return data：从数据寄存器中读取到的数据
  */
static uint16_t AD7705_read_2byte(void)
{
    uint16_t high_byte, low_byte;
    uint16_t data;
    
    high_byte = AD7705_read_byte();
    low_byte  = AD7705_read_byte();

    data = ((high_byte << 8) | low_byte);
    
    return data; 
} 

/**
  * @brief  AD7705同步接口时序
  * @param  None
  * @return None
  * @note   持续给DIN写入高电平并持续至少32个串行时钟周期，串口复位
            最好在完成此操作后，再次设置所有的寄存器
  */
static void AD7705_spi_sync(void)
{
    uint8_t i;
    
    for (i = 0; i < 100; i++)
    {
        AD7705_write_byte(0xff);
    }
    
}


