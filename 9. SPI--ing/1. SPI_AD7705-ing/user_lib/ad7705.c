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
#define USE_SOFT_SPI        /**< 使用GPIO模拟spi */
//#define USE_HARD_SPI        /**< 使用主芯片自带的硬件spi */
/** 寄存器 */
#define REG_COMMUNICATION       0X00        
#define REG_SETUP               0X10
#define REG_CLOCK               0X20
#define REG_DATA                0X30
#define REG_ZERO_CH1            0X60        /**< CH1 偏移寄存器 */
#define REG_FULL_CH1            0X70        /**< CH1 满量程寄存器 */
#define REG_ZERO_CH2            0X61
#define REG_FULL_CH2            0X71
#define REG_WRITE               0X00
#define REG_READ                0X08
/** 设置寄存器位定义 开始 */
/** 设置校准方式 */
#define NORMAL                  (0 << 6)        /**< 正常模式下 转换器进行正常的转换 */
#define SELF_CALIBRATION        (1 << 6)        /**< 自校准 */
#define ZERO_CALIBRATION        (2 << 6)        /**< 零标度系统校准 */
#define FULL_CALIBRATION        (3 << 6)        /**< 满标度系统校准 */
/** 片上PGA的增益设置，若i = 0 则增益为：2的0次方 = 1 (i最大值为7) */            
#define GAIN_POW_2_I(i)         (i << 3)
/** 双极性或单极性输入 */
#define BIPOLARITY              (0 << 2)
#define UINPOLARITY             (1 << 2)
/** 是否启用内部缓冲器 */
#define BUFF_DISABLE            (0 << 1)        /**< 内部缓冲区短路 */
#define BUFF_ENABLE             (1 << 1)
/** 滤波器同步 */
#define FSYNC_0                 0               /**< 低电平时调制器和滤波器开始处理数据 */
#define FSYNC_1                 1
/** 设置寄存器位定义 结束 */
/** 时钟寄存器位定义 开始 */
#define CLK_ENABLE              0x00    /**< 时钟输出使能 外接晶振时必须使能 */
#define CLK_DISABLE             0x10    /**< 外部提供时钟时，设置该位进制MCK_OUT引脚输出省电 */
/** 设置时钟频率 */
#define CLK_4_9152M             0x08
#define CLK_2_4576M             0x00    /**< 转换器主时钟频率为以上频率之一 则CLK位为0 */
#define CLK_1M                  0x04
#define CLK_2M                  0x0c    /**< 转换器主时钟频率为以上频率之一 则CLK位为1 */
/** CLK和FS位一起选择器件的输出频率 以下为在CLK = 1条件下成立 */
#define FS_50HZ                 0x00
#define FS_60HZ                 0x01
#define FS_250HZ                0x02
#define FS_500HZ                0x04
/** */
#define ZERO_0                  0x00
#define ZERO_1                  0x80
/** 时钟寄存器位定义 结束 */
/** 使用软件SPI 开始 */
#ifdef USE_SOFT_SPI
/** 软件SPI GPIO 开始 */
#define SCK_PORT                 GPIOB
#define SCK_PIN                  GPIO_PIN_3

#define DI_PORT                 GPIOB
#define DI_PIN                  GPIO_PIN_5

#define DO_PORT                 GPIOB
#define DO_PIN                  GPIO_PIN_4

#define CS_PORT                 GPIOF
#define CS_PIN                  GPIO_PIN_7

#define DRDY_PORT               GPIOB       /**< H */
#define DRDY_PIN                GPIO_PIN_7

#define RESET_PORT                 GPIOA
#define RESET_PIN                  GPIO_PIN_4
/** 软件SPI  GPIO 结束 */
/** GPIO 置位 复位操作 */
#define spi_set_cs();
#define spi_rst_cs();
#define spi_set_sck();
#define spi_rst_sck();
#define spi_set_di();
#define spi_rst_di();
/** 引脚电平检测　*/
#define DO_IS_HIGH();           (HAL_GPIO_ReadPin(DO_PORT, DO_PIN) == GPIO_PIN_SET);
#define DRDY_IS_LOW();          (HAL_GPIO_ReadPin(DRDY_PORT, DRDY_PIN) == GPIO_PIN_RESET);

#endif
/** 使用软件SPI 结束 */
/** ----------------------- 私有函数原型 ------------------------ */
static void AD7705_write_reg(uint8_t reg_addr, uint32_t vlaue);
static void AD7705_read_reg(uint8_t reg_addr);
static void AD7705_wait_DRDY(void);
static void AD7705_read_3byte(void);
static void AD7705_read_2byte(void);
static void AD7705_read_1byte(void);
static void AD7705_write_3byte(void);
static void AD7705_write_1byte(uint8_t data);
static void AD7705_send_8bit(uint8_t data);
static void AD7705_recv_8bit(void);
static void AD7705_sync_spi(void);
static void AD7705_hard_reset(void);

/** ----------------------- 驱动程序接口 ------------------------ */
/**
 *  @brief  
 *  @param  
 *  @retval None
 */
void AD7705_init(void)
{
    delay_ms(10);
    AD7705_hard_reset();
    delay_ms(5);
    AD7705_sync_spi();
    delay_ms(5);
    
    
}
 
/**
 *  @brief  
 *  @param  
 *  @retval None
 */
uint16_t AD7705_get_adc_value(uint8_t channel)
{
    uint8_t i;
    uint8_t ad_value;
    
    /** 为了避免通道切换造成读数失效，每个通道读两次 */
    for (i = 0; i < 2; i++)
    {
        
        if (channel == 1)
        {
            
        }
        else if (channel == 2)
        {
            
        }
        else
        {
            ad_value = 0;       /** channel错误 返回0 */ 
        }
        
//        ad_value = 
    }
    
    return ad_value;
}

/** ---------------------- 私有函数区 --------------------------- */
/**
 *  @brief  
 *  @param  
 *  @return None
 */
static void AD7705_write_reg(uint8_t reg_addr, uint32_t vlaue)
{
    
}
static void AD7705_read_reg(uint8_t reg_addr)
{
    
}
static void AD7705_wait_DRDY(void)
{
    
}    
static void AD7705_read_3byte(void)
{
    
}
static void AD7705_read_2byte(void)   
{
    
}
static void AD7705_read_1byte(void)
{
    
}
static void AD7705_write_3byte(void)
{
    
}
static void AD7705_write_1byte(uint8_t data)
{
    
}
static void AD7705_send_8bit(uint8_t data)
{
    
}
static void AD7705_recv_8bit(void)
{
    
}

/**
 *  @brief  AD7705在串行接口失布后复位 复位后要延时500us再访问
 *  @param  None  
 *  @retval None
 */
static void AD7705_sync_spi(void)
{
    spi_rst_cs();
    AD7705_send_8bit(0xff);
    AD7705_send_8bit(0xff);
    AD7705_send_8bit(0xff);
    AD7705_send_8bit(0xff);
    spi_set_cs();
}

/**
 *  @brief  AD7705硬件复位
 *  @param  None  
 *  @retval None
 */
static void AD7705_hard_reset(void)
{
    HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_SET);
    delay_ms(1);
    HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_RESET);
    delay_ms(2);
    HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_SET);
    delay_ms(1);
    
}