/**
  * @file  spi_dup.c
  * @brief SPI上层函数
  * @par   date        version    author    remarks
  *        2016-08-14  v1.0       zbt       初次创建
  *
  */  

/** 头文件包含区 ------------------------------------------------ */
#include "spi_dup.h"
#include "spi.h"

/** 私有宏(类型定义) -------------------------------------------- */ 

/** 私有变量 --------------------------------------------------- */

/** 外部变量 --------------------------------------------------- */
extern SPI_HandleTypeDef hspi1;

/** 私有函数原型 ----------------------------------------------- */

/** 公有函数 --------------------------------------------------- */

/**
  * @brief  SPI2发送并接收一个字节
  * @param  byte：要发送的字节值
  * @retval recv_byte：发送字节后，从MISO信号线上接收到的字节
  */
uint8_t spi_readwrite_onebyte(uint8_t send_byte)
{
    uint8_t recv_byte;
    
    HAL_SPI_TransmitReceive(&hspi1, &send_byte, &recv_byte, 1, 5000);
    
    return recv_byte;
}

/** 私有函数 --------------------------------------------------- */

