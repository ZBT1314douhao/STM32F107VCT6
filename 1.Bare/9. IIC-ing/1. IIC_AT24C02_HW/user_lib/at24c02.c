/**
  * @file  at24c02.c
  * @brief at24c02��������
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       ���δ���
                                            �����򵥵�ʾ��IIC���ʹ��
  *                                         
  */

/** ͷ�ļ������� ------------------------------------------------ */
#include "at24c02.h"

/** ˽�к�(���Ͷ���) -------------------------------------------- */ 
#define AT24C02_IIC                 hi2c1

#define AT24C02_DEVICE_ADDR         0xA0
#define AT24C02_PAGE_SIZE           8		/**< ҳ�ռ�8B */
#define AT24C02_MEM_SIZE            256		/**< ��32ҳ �ܿռ�256B */
#define AT24C02_ADDR_BYTE           1

#define AT24C02_WRITE               0x00
#define AT24C02_READ                0x01

/** ˽�б��� --------------------------------------------------- */


/** �ⲿ���� --------------------------------------------------- */
extern I2C_HandleTypeDef hi2c1;

/** ˽�к���ԭ�� ----------------------------------------------- */
static void at24c02_blocking_mode_tx_rx_test(void);
static void at24c02_blocking_mode_mem_test(void);


/** ���к��� --------------------------------------------------- */
/**
  * @brief  AT24C02������������IICͨѶ���Դ���
  * @param  None
  * @retval None
  */
void AT24C02_iic_test(void)
{
    HAL_StatusTypeDef iic_status;
    
    if ((iic_status = HAL_I2C_IsDeviceReady(&hi2c1, AT24C02_DEVICE_ADDR, 3, 100)) != HAL_OK)
    {
        printf("iic_status ready is %d\r\n", iic_status);
    }
    else
    {
        printf("iic device ready\r\n");
    }   
    HAL_Delay(500);
    at24c02_blocking_mode_tx_rx_test();
    HAL_Delay(2000);
    at24c02_blocking_mode_mem_test();


}

/**
  * @brief  AT24C02��ָ����ַд��ָ����С������
  * @param  addr�� Ҫд�����ݵĵ�ַ
  * @param  *p_data�� Ҫд�������
  * @param  size�� д�����ݵĳ���
  * @retval None
  */
void at24c02_write(uint8_t addr, uint8_t *p_data, uint8_t size)
{
    uint8_t data_index;
    uint8_t write_data[size + 1];
    
    write_data[0] = addr;
    for (data_index = 1; data_index <= size; data_index++)
    {
        write_data[data_index] = (*p_data++);
    }
    
    HAL_I2C_Master_Transmit(&AT24C02_IIC, AT24C02_DEVICE_ADDR, write_data, (size + 1), 500);
        
}

/**
  * @brief  AT24C02��ָ����ַ�ж�ȡ����
  * @param  addr�� Ҫ��ȡ���ݵĵ�ַ 
  * @param  size�� Ҫ��ȡ���ݵĳ���
  * @retval None
  */
void at24c02_read(uint8_t addr, uint8_t *p_data, uint8_t size)
{
    /** �ȷ���Ҫ��ȡ�����ݵĵ�ַ �ڴӸõ�ַ��ʼ��ȡ���� */
    HAL_I2C_Master_Transmit(&AT24C02_IIC, AT24C02_DEVICE_ADDR, &addr, 1, 500);
    HAL_I2C_Master_Receive(&AT24C02_IIC, AT24C02_DEVICE_ADDR, p_data, size, 500);
}

/** ˽�к��� --------------------------------------------------- */
/**
  * @brief  AT24C02����ģʽ�µĶ�д���Գ���
  * @param  None
  * @retval None
  */
static void at24c02_blocking_mode_mem_test(void)
{
    uint8_t i;
    uint8_t mem_addr = 0x20;
    uint8_t mem_tx[AT24C02_PAGE_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, };
    uint8_t mem_rx[AT24C02_PAGE_SIZE];
    
    HAL_I2C_Mem_Write(&AT24C02_IIC, AT24C02_DEVICE_ADDR, mem_addr, I2C_MEMADD_SIZE_8BIT, mem_tx, AT24C02_PAGE_SIZE, 500);
    printf("Med write is");
    for (i = 0; i < AT24C02_PAGE_SIZE; i++)
    {
        printf(" %d ", mem_tx[i]);
    }
    HAL_Delay(50);
    HAL_I2C_Mem_Read(&AT24C02_IIC, AT24C02_DEVICE_ADDR, mem_addr, I2C_MEMADD_SIZE_8BIT, mem_rx, AT24C02_PAGE_SIZE, 500);
    
    printf("\r\nMed read is ");
    for (i = 0; i < AT24C02_PAGE_SIZE; i++)
    {
        printf(" %d ", mem_rx[i]);
    }
    printf("\r\n");
}

/**
  * @brief  AT24C02����ģʽ�µĶ�д���Գ���
  * @param  None
  * @retval None
  */
static void at24c02_blocking_mode_tx_rx_test(void)
{
    uint8_t i;
    uint8_t addr_single = 0x02;
    uint8_t tx_data = 0x09;
    uint8_t rx_data = 0;
    uint8_t addr_multiple = 0x10;
    uint8_t tx[3] = {0x01, 0x02, 0x03, };
    uint8_t rx[3];
    
    printf("��д�������ݲ���\r\n");
    at24c02_write(addr_single, (uint8_t *)&tx_data, 1);
    HAL_Delay(500);
    printf("�ɹ�д�� %d\r\n", tx_data);
    at24c02_read(addr_single, &rx_data, 1);
    printf("�ɹ���ȡ %d\r\n", rx_data);
    
    printf("��д������ݲ���\r\n");
    at24c02_write(addr_multiple, (uint8_t *)&tx, 3);
    printf("д���������");
    for (i = 0; i < 3; i++)
    {
        printf(" %d ", tx[i]);
    }
    printf("\r\n");
    HAL_Delay(500);
    at24c02_read(addr_multiple, rx, 3);
    printf("��ȡ��������");
    for (i = 0; i < 3; i++)
    {
        printf(" %d ", rx[i]);
    }
    printf("\r\n");
    
}
