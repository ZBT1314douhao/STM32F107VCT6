/**
  * @file  at24c02.c
  * @brief at24c02驱动程序
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       初次创建
  *
  */

/** 头文件包含区 ------------------------------------------------ */
#include "at24c02.h"
#include "iic_dup.h"

/** 私有宏(类型定义) -------------------------------------------- */ 
#define AT24C02_DEVICE_ADDR         0xA0

#define AT24C02_PAGE_SIZE           8
#define AT24C02_MEM_SIZE            256
#define AT24C02_ADDR_BYTE           1

/** 私有变量 --------------------------------------------------- */
static uint8_t s_write_buffer[AT24C02_MEM_SIZE];
static uint8_t s_read_buffer[AT24C02_MEM_SIZE];

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */
//static void AT24C02_ack(void);
static void AT24C02_error_handle(void);
static void AT24C02_read_test(void);
static void AT24C02_write_test(void);

/** 公有函数 --------------------------------------------------- */
/**
  * @brief  AT24C02与主控制器的IIC通讯测试代码
  * @param  None
  * @retval None
  */
void AT24C02_iic_test(void)
{
    iic_stop();     /**< 必须先复位IIC总线上的设备到待机模式 */
    HAL_Delay(10);
    
    /** 检测总线上是否挂接了IIC设备（此处为AT24C02） */
    if (iic_check_device_status(AT24C02_DEVICE_ADDR) == 0)
    {
        printf("\r\niic device exists\r\n\r\n");
    }
    else
    {
        printf("\r\nno iic device exists\r\n\r\n");
    }
    
    AT24C02_write_test();
    HAL_Delay(5);
    AT24C02_read_test();
}

/**
  * @brief  从AT24C02中读取数据
  * @param  read_data： 读取到的数据
  * @param  start_addr： 读取数据的起始地址
  * @param  data_length： 数据的长度
  * @retval None
  */
void AT24C02_read_data(uint8_t *read_data, uint16_t start_addr, uint16_t data_length)
{
    uint16_t i;
    
    iic_start();
    iic_sendbyte(AT24C02_DEVICE_ADDR | IIC_WRITE);
//    AT24C02_ack();
    if (iic_wait_ack() != 0)
    {
        AT24C02_error_handle();
        printf("first read error\r\n");
    }
    
    if (AT24C02_ADDR_BYTE == 1)
    {
        iic_sendbyte((uint8_t)start_addr & 0xff);
//        AT24C02_ack();
        if (iic_wait_ack() != 0)
        {
            AT24C02_error_handle();
            printf("addr byte error\r\n");
        }
    }
    
    iic_start();
    iic_sendbyte(AT24C02_DEVICE_ADDR | IIC_READ);
//    AT24C02_ack();
    if (iic_wait_ack() != 0)
    {
        AT24C02_error_handle();
        printf("read data error\r\n");
    }
    
    for (i = 0; i < data_length; i++)
    {
        read_data[i] = iic_readbyte();
        
        if (i != data_length - 1)
        {
            iic_ack();      /**< 读完非最后一个字节后， 产生应答信号 */
        }
        else
        {
            iic_nack();     /**< 最后一个字节读完后 产生非应答信号 */
        }
//        printf("read data is %d\n", read_data[i]);  /**< 调试代码 */
    }
    
    iic_stop();
}

/**
  * @brief  通过IIC向AT24C02写数据
  * @param  write_data：  要写入AT24C02的数据指针
  * @param  start_addr：  要写入AT24C02的起始地址
  * @param  data_length： 要写入AT24C02的数据长度
  * @retval None
  */
void AT24C02_write_data(uint8_t *write_data, uint16_t start_addr, uint16_t data_length)
{
    uint16_t i, j;
    uint16_t start_addr_dup;
    
    start_addr_dup = start_addr;
    for (i = 0; i < data_length; i++)
    {
        
        if ((i == 0) || (start_addr_dup & (AT24C02_PAGE_SIZE - 1)) == 0)
        {
            iic_stop();
            
            for (j = 0; j < 2000; j++)
            {
                iic_start();
                iic_sendbyte(AT24C02_DEVICE_ADDR | IIC_WRITE);
                
                if (iic_wait_ack() == 0)
                {
                    break;
                }
            }
            if (j >= 2000)
            {
                printf("j = 1000\r\n");
                AT24C02_error_handle();
            }
            
            if (AT24C02_ADDR_BYTE == 1)
            {
                iic_sendbyte((uint8_t)start_addr_dup & 0xff);
//                AT24C02_ack();
                if (iic_wait_ack() != 0)
                {
                    AT24C02_error_handle();
                    printf("addr_byte wrong\r\n");
                }
            }
        }
        
        iic_sendbyte(write_data[i]);
//        AT24C02_ack();
        if (iic_wait_ack() != 0)
        {
            AT24C02_error_handle();
            printf("write failed\r\n");
        }
        
//        printf("write_data is %d \n", write_data[i]);    /**< 调试代码 */ 
        start_addr_dup++;
    }
    
    iic_stop();
}

/** 私有函数 --------------------------------------------------- */
/** 以下为测试用程序 ------------------------------------------- */
/**
  * @brief  AT24C02读取数据测试
  * @param  None
  * @retval None
  */
static void AT24C02_read_test(void)
{
    uint16_t i;
    
    AT24C02_read_data(s_read_buffer, 0, (AT24C02_MEM_SIZE % 10));
    
    printf("read data is:\t");
    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)
    {
        printf("%d ", s_read_buffer[i]);
    }
    
    printf("\r\nread test ok\r\n");
}

/**
  * @brief  AT24C02写数据测试
  * @param  None
  * @retval None
  */
static void AT24C02_write_test(void)
{
    uint16_t i;
    
    printf("write data is: \t");
    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)
    {
        s_write_buffer[i] = i;
        printf("%d ", s_write_buffer[i]);
    }
    
    AT24C02_write_data(s_write_buffer, 0, (AT24C02_MEM_SIZE % 10));
    
    printf("\r\nwrite test ok\r\n");
}


/** 以上为测试用程序 ------------------------------------------- */

/**
  * @brief  AT24C02应答错误处理程序
  * @param  None
  * @retval None
  */
static void AT24C02_error_handle(void)
{
    iic_stop();
//    printf("At24C02 read failed\r\n");   /**< 调试用 */
}

//static void AT24C02_ack(void)
//{
//    if (iic_wait_ack() != 0)
//    {
//        AT24C02_error_handle();
//    }
//}


