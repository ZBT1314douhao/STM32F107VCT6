/**
  * @file    uart_drv.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016/05
  * @brief   uart1上层驱动
  *
  */  

/** 头文件包含区 ------------------------------------------------ */
#include "uart_drv.h"
#include "usart.h"

#include "led.h"

/** 私有宏(类型定义) -------------------------------------------- */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

//#define TXBUFFERSIZE                      (COUNTOF(tx_buffer) - 1)
//#define RXBUFFERSIZE                      TXBUFFERSIZE

/** 私有变量 --------------------------------------------------- */
static __IO ITStatus uart_status = RESET;   /**< 此值在中断中被修改 */

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */
static void uart_wait_transfer_completed(void); 

/** 公有函数 --------------------------------------------------- */
/**
  * @brief  uart 测试程序 通过在串口助手中发送相应的指令 控制LED
  * @param  None
  * @retval None
  * @note   在串口助手中勾选16进制发送 比如：发送 1 即可点亮LED_0
  */
void uart_test(void)
{
    uint8_t tx_buffer[] = "uart DMA test\r\n";
    uint8_t cmd_recv[COUNTOF(tx_buffer)];

    
    uart_transmit(tx_buffer, COUNTOF(tx_buffer));
//    uart_transmit((uint8_t *)&tx_buffer[0], COUNTOF(tx_buffer));
    
    uart_receive(cmd_recv, COUNTOF(tx_buffer));    

//    /** */
    uart_transmit((uint8_t*)&cmd_recv[0], COUNTOF(cmd_recv));
}

/**
  * @brief  uart 发送一个字符串
  * @param  data: 数据缓冲区指针
  * @param  size: 要发送数据的大小
  * @retval None
  */
HAL_StatusTypeDef uart_transmit(uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status;
    
    status = HAL_UART_Transmit_DMA(&huart1, data, size);
    uart_wait_transfer_completed();
    
    return status;
}

/**
  * @brief  uart 接收一个字符串
  * @param  data: 数据缓冲区指针
  * @param  size: 要接收数据的大小
  * @retval None
  */
HAL_StatusTypeDef uart_receive(uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status;
    
    HAL_UART_Receive_DMA(&huart1, data, size);
//    uart_wait_transfer_completed();
    
    return status;
}

/** 私有函数 --------------------------------------------------- */
/**
  * @brief  uart 等待传输完成
  * @param  None
  * @retval None
  */
static void uart_wait_transfer_completed(void)
{
    /**方法一　*/
//    while (uart_status != SET)
//    {
//        /** 用户可在等待传输完成过程中运行其他任务 比如 闪烁个led */
//        led_on(LED_3);
//        HAL_Delay(500);
//        led_off(LED_3);
//        HAL_Delay(500);
//    }
//    uart_status = RESET;        /**< 复位等待传输完成标志 */
    
    /**方法二　*/    
    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
    {
//        led_on(LED_3);
//        HAL_Delay(500);
//        led_off(LED_3);
//        HAL_Delay(500);
    }

}

/** uart中断处理回调函数 ------------------------------------------- */
/**
  * @brief  Tx 传输完成回调函数
  * @param  huart: UART结构体指针
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
//    uart_status = SET;
    printf("发送回调 \r\n");
}

/**
  * @brief  Rx 传输完成回调函数
  * @param  huart: UART结构体指针
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//    uart_status = SET;
    printf("接收回调 \r\n");
}

