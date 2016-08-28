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
static __IO uint8_t transfer_state = TRANSFER_WAIT;

/** 外部变量 --------------------------------------------------- */
extern SPI_HandleTypeDef hspi1;

/** 私有函数原型 ----------------------------------------------- */

/** 公有函数 --------------------------------------------------- */

/**
  *  @brief  SPI2发送一个字节
  *  @param  byte：要发送的字节值
  *  @retval recv_byte：发送字节后，从MISO信号线上接收到的字节
  */
uint8_t spi_send_onebyte(uint8_t send_byte)
{
    uint8_t recv_byte;
    
    HAL_SPI_TransmitReceive_IT(&hspi1, &send_byte, &recv_byte, 1);
    
    /** 等待SPI2传输完成 */
//    while (transfer_state == TRANSFER_WAIT);
    
    return recv_byte;
}

/**
  * @brief  spi2读取一个字节
  * @param 	None
  * @retval 读取到的字节：spi2_send_onebyte(DUMMY_BYTE)的返回值
  */
uint8_t spi_read_onebyte(void)
{
    return (spi_send_onebyte(DUMMY_BYTE));
}

/** 以下程序无需用户调用 由相应的中断函数进行调用 ---------------- */
/**-
  * @brief  TxRx传送回调函数
  * @param  hspi： SPI 句柄
  * @retval None
  * @note   表征SPI传送是否完成
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
//    transfer_state = TRANSFER_COMPLETE;
}
void set_transfer_state(void)
{
    transfer_state = TRANSFER_COMPLETE;
}

/**-
  * @brief  TxRx错误回调函数
  * @param  hspi： SPI 句柄
  * @retval None
  * @note   表征SPI产生了错误
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  transfer_state = TRANSFER_ERROR;
}

/** 以上程序无需用户调用 由相应的中断函数进行调用 ----------------- */

/** 私有函数 --------------------------------------------------- */

