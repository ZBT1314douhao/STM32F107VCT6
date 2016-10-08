/**
 * @file    ad7705.c
 * @author  zbt
 * @version v0.01
 * @date    2016-07-11
 * @brief   ad7705驱动程序
 * @note    MCU配置为主机模式，CPOL&CPHA位皆配置为逻辑1，使得SCLK
            在两次数据传输之间为高电平
 */
/** 头文件包含区 --------------------------------------------- */ 
#include "ad7705.h"
#include "delay.h"

/** 私有宏定义 ----------------------------------------------- */
//#define AD7705_SPI_DEBUG                /**< 测试AD7705的SPI通讯是否正常 */
#define AD7705_DEBUGE_MODE              /**< 驱动程序调试模式 */

#define AD7705_CLK_REGISTER_SETUP   (MASTER_CLK_EN | CLK_FREQ_4_9152M | CLK_BIT_VALUE\
                                        | OUT_UPDATE_RATE_50HZ)
/** 通道0(AIN1+，AIN1-)的设置寄存器设置 */                                        
#define CH1_REGISTER_SETUP          (SET_GAIN_1 | UNIPOLAR | BUFFER_ON | FILTER_SYNC_ON)
#define AD7705_VREF                 2.5f    /**< AD7705的参考电压 */
#define SPI_POLLING_TIMEOUT         5000    /**< 单位：ms */
#define DUMMY_BYTE                  0XFF

/** 外部变量 ------------------------------------------------ */
extern SPI_HandleTypeDef hspi1;

/** 私有函数原型--------------------------------------------- */
static void spi_test(void);
static void AD7705_spi_sync(void);
static uint8_t AD7705_DRDY_read(void);
static uint16_t AD7705_read_2byte(void);
static uint8_t AD7705_readwrite_byte(uint8_t tx_data);

static void AD7705_sys_zero_calibration(uint8_t channel);
static void AD7705_sys_full_calibration(uint8_t channel);

/** 驱动程序接口 -------------------------------------------- */


/**
  * @brief  AD7705初始化
  * @param  None
  * @return None
  */
void AD7705_init(void)
{
    uint8_t clk_read_byte;
    
    #ifdef AD7705_SPI_DEBUG
//    spi_test();     /**< SPI配置测试用于调试 */
    #endif
      
    AD7705_spi_sync();      /**< 同步SPI接口时序 */
    HAL_Delay(1);
    
    #ifdef AD7705_DEBUGE_MODE
    /** 读取初始状态时的时钟寄存器的值，用于测试SPI通讯是否成功 */
    AD7705_readwrite_byte(CLOCK_REG | READ_REG);
    clk_read_byte = AD7705_readwrite_byte(DUMMY_BYTE);
    printf("clk register: 0x%x\n", clk_read_byte);
    if (clk_read_byte == 0x05)
    {
        printf("SPI: OK\n");
    }
    else
    {
        printf("SPI: Error\n");
    }
    #endif
    
    /** 激活通道0 下一步操作是写时钟寄存器 */
    AD7705_readwrite_byte(CHANNEL_0 | WRITE_REG | CLOCK_REG);
    /** 主时钟输出使能 输出更新速率50Hz */
    AD7705_readwrite_byte(AD7705_CLK_REGISTER_SETUP);
//    AD7705_readwrite_byte(MASTER_CLK_EN | CLK_FREQ_4_9152M 
//                        | CLK_BIT_VALUE | OUT_UPDATE_RATE_50HZ);
    
    /** 上电后进行自校准 */
    AD7705_self_calibration(1);
    HAL_Delay(1);
}
 
/**
  * @brief  获取AD7705通道采集到值
  * @param  channel：1 or 2
  * @return ad_value：采集到的值
  */
float AD7705_get_adc_value(uint8_t channel)
{
    uint8_t i;
    uint16_t ad_value = 0;
    float voltage = 0;
    
    /** 为了避免通道切换造成读数失效，每个通道读两次 */
    for (i = 0; i < 2; i++)
    {
        if (AD7705_DRDY_read() == 0)
        {
            if (channel == 1)
            {   /** 读取AIN1+和AIN1-通道的数据寄存器的值 */
                AD7705_readwrite_byte(DATA_REG | READ_REG | CHANNEL_0);
            }
//            else if (channel == 2)
//            {
//                /** 读取AIN2+和AIN2-通道的数据寄存器的值 */
//                AD7705_readwrite_byte(DATA_REG | READ_REG | CHANNEL_1);
//            }
            else
            {
                printf("channel parameter error!");
            }
        }

        ad_value = AD7705_read_2byte();
    }
    
    /** 获取从通道得到的AD值 */
    voltage = (float)ad_value * AD7705_VREF / (float)65535;
    printf("ad_value is %d\n", ad_value);
    printf("voltage is %f\n", voltage);
    
    return voltage;
}

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
        /** 激活通道0(AIN1+，AIN1-) 下一步是写设置寄存器 */
        AD7705_readwrite_byte(CHANNEL_0 | WRITE_REG | STEUP_REG);
        /** 使能自校准工作模式，设置增益为1，单极性，开缓冲和滤波器同步 */
        AD7705_readwrite_byte(SELF_CALIB_MODE | CH1_REGISTER_SETUP);
//        AD7705_readwrite_byte(SELF_CALIB_MODE | SET_GAIN_1 | UNIPOLAR 
//                                | BUFFER_ON | FILTER_SYNC_ON);
        
        /** 读取校准完毕信号 */
        if (AD7705_DRDY_read() == 0)
        {
            printf("self calibration ok\n");
            printf("AD7705 init ok\n");
        }
        else
        {
            printf("self calibration error\n");
        }
    }
    else
    {
        ;/**< 此else可用于双通道的AD7705应用 */
    }
}

/**
  * @brief  AD7705系统校准
  * @param  channel：1 or 2
  * @return None
  * @note   系统校准分为两个步骤 零位校准 + 全量程校准
  */
void AD7705_syy_calibration(uint8_t channel)
{
    AD7705_sys_zero_calibration(channel);
    HAL_Delay(5);
    AD7705_sys_full_calibration(channel);
}



/** ---------------------- 私有函数区 --------------------------- */
/**
  * @brief  AD7705零位校准
  * @param  channel：1 or 2
  * @return None
  * @note   在进行零位校准之前，必须要设置好零位电压，并保持稳定直到
            校准工作完成，
            校准花费时间 (3 * (1 / output rate))
  */
static void AD7705_sys_zero_calibration(uint8_t channel)
{
    if (channel == 1)
    {
        AD7705_readwrite_byte(CHANNEL_0 | WRITE_REG | STEUP_REG);
        /** 使能零位校准工作模式，设置增益为1，单极性，开缓冲和滤波器同步 */
        AD7705_readwrite_byte(ZERO_CALIB_MODE | CH1_REGISTER_SETUP);
//        AD7705_readwrite_byte(ZERO_CALIB_MODE | SET_GAIN_1 | UNIPOLAR 
//                                | BUFFER_ON | FILTER_SYNC_ON);
        
        /** 读取校准完毕信号 */
        if (AD7705_DRDY_read() == 0)
        {
            printf("self calibration ok\n");
            printf("AD7705 init ok\n");
        }
        else
        {
            printf("self calibration error\n");
        }
    }
    else
    {
        printf("channel parameter error!\n");
    }
}

/**
  * @brief  AD7705满量程校准
  * @param  channel：1 or 2
  * @return None
  * @note   在进行满量程校准之前，必须要设置好满量程电压，并保持稳定直到
            校准工作完成，
            校准花费时间 (3 * (1 / output rate))
  */
static void AD7705_sys_full_calibration(uint8_t channel)
{
    if (channel == 1)
    {
        AD7705_readwrite_byte(CHANNEL_0 | WRITE_REG | STEUP_REG);
        /** 使能满量程工作模式，设置增益为1，单极性，开缓冲和滤波器同步 */
        AD7705_readwrite_byte(FULL_CALIB_MODE | CH1_REGISTER_SETUP);
//        AD7705_readwrite_byte(FULL_CALIB_MODE | SET_GAIN_1 | UNIPOLAR 
//                                | BUFFER_ON | FILTER_SYNC_ON);
        
        /** 读取校准完毕信号 */
        if (AD7705_DRDY_read() == 0)
        {
            printf("self calibration ok\n");
            printf("AD7705 init ok\n");
        }
        else
        {
            printf("self calibration error\n");
        }
    }
    else
    {
        printf("channel parameter error!\n");
    }
}
/**
  * @brief  读取DRDY引脚的状态
  * @param  None
  * @return pin_status： 1：超时，0：ok
  * @note   当自校准开始的时候，DRDY置位，直到数据寄存器产生
            一个新的有效值
            花费时间大概 （9 * （1 / output rate))
  */
static uint8_t AD7705_DRDY_read(void)
{
    uint32_t i = 0;
    uint8_t pin_status = 1;
    
    for (i = 0; i < 4000000; i++)
    {
        pin_status = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
        if (pin_status == GPIO_PIN_RESET)
        {
            printf("DRDY: low \n");
            break;
        }
//        else if (pin_status == GPIO_PIN_SET)
//        {
//            if (i % 10000 == 0)
//            {
//                printf("DRDY: high \n");
//            } 
//        }
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
  * @brief  AD7705读写一个字节的数据，利用SPI1
  * @param  tx_data： 要写入寄存器的数据
  * @return rx_data： 从SPI总线/寄存器上读取到的数据
  * @note   此函数为轮询模式下配置，如使用其他模式，
            需更换其实现函数，超时时间为5s
  */
static uint8_t AD7705_readwrite_byte(uint8_t tx_data)
{
    uint8_t rx_data;
    
    HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&tx_data, (uint8_t*)&rx_data, 0x01, SPI_POLLING_TIMEOUT);
    
    return rx_data;
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
    
    high_byte = AD7705_readwrite_byte(DUMMY_BYTE);
    low_byte  = AD7705_readwrite_byte(DUMMY_BYTE);

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
    
    for (i = 0; i < 255; i++)
    {
        AD7705_readwrite_byte(0XFF);
    }
}

/**
  * @brief  SPI配置测试程序
  * @param  None
  * @return None
  * @note   将MOSI和MISO短接，通过串口观察
            如果rx_data = 0x22，则表示spi配置是没问题的
            嗯 测试结果表明里面的两个函数是没问题的
  */
static void spi_test(void)
{
    uint8_t rx_data;
    uint16_t data_2byte;
    
    rx_data = AD7705_readwrite_byte(0xff);
    printf("rx_data is %d\n", rx_data);
    data_2byte = AD7705_read_2byte();
    printf("data_2byte is %d\n", data_2byte); 
}
