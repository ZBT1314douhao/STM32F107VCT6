/**
  * @file  spi_dup.c
  * @brief SPI上层函数
  * @par   date        version    author    remarks
  *        2016-08-14  v1.0       zbt       初次创建
  *
  */  

/** 头文件包含区 ------------------------------------------------ */
#include "spi_dup.h"

/** 私有宏(类型定义) -------------------------------------------- */ 
#define SPI_FLASH               &hspi1
#define SPI_FLASH_TIME_OUT      500
/** 私有变量 --------------------------------------------------- */

/** 外部变量 --------------------------------------------------- */
extern SPI_HandleTypeDef hspi1;

/** 私有函数原型 ----------------------------------------------- */

/** 公有函数 --------------------------------------------------- */

/**
  * @brief  SPI发送并接收一个字节
  * @param  send_byte： 要发送的字节
  * @retval recv_byte：发送字节后，从MISO信号线上接收到的字节
  */
uint8_t spi_flash_write_onebyte(uint8_t send_byte)
{
    uint8_t recv_byte;
    
    HAL_SPI_TransmitReceive(SPI_FLASH, &send_byte, &recv_byte, 1, SPI_FLASH_TIME_OUT);
    
    return recv_byte;
}

/**
  * @brief  SPI接收一个字节
  * @param  None
  * @retval 接收到的字节
  */
uint8_t spi_flash_read_onebyte(void)
{
    return spi_flash_write_onebyte(SPI_FLASH_DUMMY_BYTE);
}

/**
  * @brief  SPI发送数据
  * @param  size： 数据长度
  * @retval HAL状态
  */
HAL_StatusTypeDef spi_flash_transmit(uint8_t *tx_data, uint16_t size)
{
    return HAL_SPI_Transmit(SPI_FLASH, tx_data, size, SPI_FLASH_TIME_OUT);
}


/**
  * @brief  SPI收发数据
  * @param  tx_data： 要发送的数据
  * @param  rx_data： 存储接收到的数据
  * @param  size： 数据长度
  * @retval HAL状态
  */
HAL_StatusTypeDef spi_flash_transmit_receive(uint8_t *tx_data, uint8_t *rx_data, uint16_t size)
{
    return HAL_SPI_TransmitReceive(SPI_FLASH, tx_data, rx_data, size, SPI_FLASH_TIME_OUT);
}


/** 私有函数 --------------------------------------------------- */

