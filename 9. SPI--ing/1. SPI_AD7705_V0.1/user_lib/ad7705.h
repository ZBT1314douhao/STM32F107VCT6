/**
 * @file    ad7705.h
 * @author  zbt
 * @version v0.01
 * @date    2016-07-11
 * @brief   ad7705驱动程序
 */
 
#ifndef __AD7705_H
#define __AD7705_H

#include "stm32f1xx_hal.h"

/** 通信寄存器位定义 */
enum CommunicationRegister
{   
    /** 寄存器选择 */
    COM_REG    = (0x00 << 4),       /**< 通信寄存器 */
    STEUP_REG  = (0x01 << 4),       /**< 设置寄存器 */
    CLOCK_REG  = (0x02 << 4),       /**< 时钟寄存器 */
    DATA_REG   = (0x03 << 4),       /**< 数据寄存器 16位 只读 */
    TEST_REG   = (0x04 << 4),       /**< 测试寄存器 用户不要改变此寄存器的任何位的值 */
    OFFSET_REG = (0x06 << 4),       /**< 偏移（零位校准）寄存器 24位，可读可写 */
    GAIN_REG   = (0x07 << 4),       /**< 增益（全量程校准）寄存器 24位，可读可写 */
    
    /** 读写寄存器选择 */
    WRITE_REG = (0x00 << 3),
    READ_REG  = (0x01 << 3),
    
    /** 通道选择 */
    CHANNEL_0 = 0x00,   /**< AIN1+ AIN1- 校准寄存器组0 */ 
    CHANNEL_1 = 0x01,   /**< AIN2+ AIN2- 校准寄存器组1 */ 
    CHANNEL_2 = 0x02,   /**< AIN1- AIN1- 校准寄存器组0 */ 
    CHANNEL_3 = 0x03,   /**< AIN1- AIN2- 校准寄存器组2 */ 
};

/** 设置寄存器位定义 */
enum SetupRegister
{
    /** 工作模式选择 */
    NORMAL_MODE     = (0x00 << 6),     /**< 正常工作模式 */
    SELF_CALIB_MODE = (0x01 << 6),     /**< 自校准模式 */
    ZERO_CALIB_MODE = (0x02 << 6),     /**< 零位校准模式 */
    FULL_CALIB_MODE = (0x03 << 6),     /**< 满量程校准模式 */
    
    /** 增益选择 SET_GAIN_x: x为设置的增益大小 */
    SET_GAIN_1   = (0x00 << 3),
    SET_GAIN_2   = (0x01 << 3),
    SET_GAIN_4   = (0x02 << 3),
    SET_GAIN_8   = (0x03 << 3),
    SET_GAIN_16  = (0x04 << 3),
    SET_GAIN_32  = (0x05 << 3),
    SET_GAIN_64  = (0x06 << 3),
    SET_GAIN_128 = (0x07 << 3),
    
    /** 输入信号的极性选择 */
    BIPOLAR = (0x00 << 2),
    UNIPOLAR = (0x01 << 2),
    
    /** 缓冲控制 */
    BUFFER_OFF = (0x00 << 1),
    BUFFER_ON  = (0x01 << 1),   /**< 打开内部缓冲，用于输入处理高源阻抗 */
    
    /** 滤波器同步 */
    FILTER_SYNC_OFF = (0x01),
    FILTER_SYNC_ON  = (0x00),

};

/** 时钟寄存器位定义 */
enum ClockRegister
{
    /** 当利用外部晶振时，必须失能主时钟 */
    /** CLKDIS位设置主时钟，0：使能 1：失能 */
    MASTER_CLK_EN  = (0x00 << 4), 
    MASTER_CLK_DIS = (0x01 << 4),  
    
    /** 如果主时钟的频率为4.9152MHz or 2.4576MHz，则CLK位必须设置为1
        如果主时钟的频率为1MHz or 2MHz，则CLK位必须设置为0 
    */
    CLK_FREQ_4_9152M = (0x01 << 4),
    CLK_FREQ_2_4576M = (0x00 << 4),
    CLK_BIT_VALUE = (0x01 << 3),
    
    /** 输出更新速度设置 */
    OUT_UPDATE_RATE_50HZ  = (0x00),
    OUT_UPDATE_RATE_60HZ  = (0x01),
    OUT_UPDATE_RATE_250HZ = (0x02),
    OUT_UPDATE_RATE_500HZ = (0x03),
};

void AD7705_init(void);
uint16_t AD7705_get_adc_value(uint8_t channel);

void AD7705_self_calibration(uint8_t channel);
void AD7705_system_calibration_zero(uint8_t channel);
void AD7705_system_calibration_full(uint8_t channel);

#endif 
