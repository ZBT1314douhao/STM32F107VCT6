/**
  * @file    uart_drv.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016/05
  * @brief   uart1�ϲ�����
  *
  */  

/** ͷ�ļ������� ------------------------------------------------ */
#include "uart_drv.h"
#include "usart.h"

#include "led.h"

/** ˽�к�(���Ͷ���) -------------------------------------------- */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

//#define TXBUFFERSIZE                      (COUNTOF(tx_buffer) - 1)
//#define RXBUFFERSIZE                      TXBUFFERSIZE

/** ˽�б��� --------------------------------------------------- */
static __IO ITStatus uart_status = RESET;   /**< ��ֵ���ж��б��޸� */

/** �ⲿ���� --------------------------------------------------- */

/** ˽�к���ԭ�� ----------------------------------------------- */
static void uart_wait_transfer_completed(void); 

/** ���к��� --------------------------------------------------- */
/**
  * @brief  uart ���Գ��� ͨ���ڴ��������з�����Ӧ��ָ�� ����LED
  * @param  None
  * @retval None
  * @note   �ڴ��������й�ѡ16���Ʒ��� ���磺���� 1 ���ɵ���LED_0
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
  * @brief  uart ����һ���ַ���
  * @param  data: ���ݻ�����ָ��
  * @param  size: Ҫ�������ݵĴ�С
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
  * @brief  uart ����һ���ַ���
  * @param  data: ���ݻ�����ָ��
  * @param  size: Ҫ�������ݵĴ�С
  * @retval None
  */
HAL_StatusTypeDef uart_receive(uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status;
    
    HAL_UART_Receive_DMA(&huart1, data, size);
//    uart_wait_transfer_completed();
    
    return status;
}

/** ˽�к��� --------------------------------------------------- */
/**
  * @brief  uart �ȴ��������
  * @param  None
  * @retval None
  */
static void uart_wait_transfer_completed(void)
{
    /**����һ��*/
//    while (uart_status != SET)
//    {
//        /** �û����ڵȴ�������ɹ����������������� ���� ��˸��led */
//        led_on(LED_3);
//        HAL_Delay(500);
//        led_off(LED_3);
//        HAL_Delay(500);
//    }
//    uart_status = RESET;        /**< ��λ�ȴ�������ɱ�־ */
    
    /**��������*/    
    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
    {
//        led_on(LED_3);
//        HAL_Delay(500);
//        led_off(LED_3);
//        HAL_Delay(500);
    }

}

/** uart�жϴ���ص����� ------------------------------------------- */
/**
  * @brief  Tx ������ɻص�����
  * @param  huart: UART�ṹ��ָ��
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
//    uart_status = SET;
    printf("���ͻص� \r\n");
}

/**
  * @brief  Rx ������ɻص�����
  * @param  huart: UART�ṹ��ָ��
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//    uart_status = SET;
    printf("���ջص� \r\n");
}

