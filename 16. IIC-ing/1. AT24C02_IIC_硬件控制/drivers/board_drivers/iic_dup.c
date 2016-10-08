/**
* @file  iic_dup.c
* @brief IIC上层程序
* @par   date        version    author    remarks
*        2016-03-21  v1.0       zbt       初次创建
*
*/

#include "iic_dup.h"
//#include "delay.h"

/** 头文件包含区 ------------------------------------------------ */


/** 私有宏(类型定义) -------------------------------------------- */
#define AT24C02_TIMEOUT             3000   /**< 设置IIC通讯超时时间为10s */
#define IIC1_SLAVE_ADDRESS7       0x00
#define AT24C02_ADDRESS             0xA0
#define AT24C02_WRITE               (AT24C02_ADDRESS |0x00)
#define AT24C02_READ                (AT24C02_ADDRESS |0x01)

#define AT24C02_PAGE_SIZE           8
#define AT24C02_MEM_SIZE            256

#define BUFFER_SIZE                 16
/** 私有变量 --------------------------------------------------- */
static uint8_t s_test_buffer[BUFFER_SIZE];

/** 外部变量 --------------------------------------------------- */
extern I2C_HandleTypeDef hi2c1;

/** 私有函数原型 ----------------------------------------------- */
//static void AT24C02_I2C1_error_handle(void);
static void AT24C02_I2C1_self_check(void);
/** 公有函数 --------------------------------------------------- */
/**
  * @brief  AT24C02与主控制器的IIC通讯测试
  * @param  None
  * @retval None
  */
void AT24C02_iic_test(void)
{
//    uint8_t i;
    
    AT24C02_I2C1_self_check();
    
//    for (i = 0; i < BUFFER_SIZE; i++)
//    {
//        s_test_buffer[i] = i;
//    }
//    
//    if (AT24C02_write(s_test_buffer, AT24C02_WRITE, BUFFER_SIZE) == HAL_OK)
//    {
//        printf("write test is ok\n");
//    }
//    else
//    {
//        printf("write test is failed\n");
//    }
//    HAL_Delay(10);
    
//    if (AT24C02_read(s_test_buffer, AT24C02_READ, BUFFER_SIZE) == HAL_OK)
//    {
//        printf("read test is ok\n");
//    }
//    else
//    {
//        printf("read test is failed\n");
//    }
}

/**
  * @brief  向AT24C02中写入多个字节
  * @param  buffer： 
  * @param  mem_addr：
  * @param  num_to_write：
  * @retval None
  */
HAL_StatusTypeDef AT24C02_write(uint8_t *buffer, uint8_t mem_addr, uint16_t num_to_write)
{
    uint16_t i;
    HAL_StatusTypeDef iic_status;
    
    for (i = 0; i < num_to_write; i++)
    {
        mem_addr += i;
        
        iic_status = HAL_I2C_Mem_Write(&hi2c1, AT24C02_WRITE,  mem_addr, 
                                        I2C_MEMADD_SIZE_8BIT, buffer, 0x01, AT24C02_TIMEOUT);
        if (iic_status != HAL_OK)
        {
            printf("write error and iic_status = %d\n", iic_status);
            return iic_status;
        }
        HAL_Delay(5);
    }
    
    return iic_status;
}

/**
  * @brief  从AT24C02中读取多个字节
  * @param  buffer： 
  * @param  mem_addr： 
  * @param  num_to_write： 
  * @retval None
  */
HAL_StatusTypeDef AT24C02_read(uint8_t *buffer, uint8_t mem_addr, uint16_t num_to_read)
{
    uint16_t i;
    HAL_StatusTypeDef iic_status;
    
    for (i = 0; i < num_to_read; i++)
    {
        mem_addr += i;
        
        iic_status = HAL_I2C_Mem_Read(&hi2c1, AT24C02_READ, mem_addr, 
                                        I2C_MEMADD_SIZE_8BIT, buffer, 0x01, AT24C02_TIMEOUT);
        if (iic_status != HAL_OK)
        {
            printf("read error\n");
            return iic_status;
        }
        HAL_Delay(5);
    }
    
    return iic_status;
}


/** 私有函数 --------------------------------------------------- */
/**
  * @brief  IIC初始化后的自检函数
  * @param  None
  * @retval None
  */
static void AT24C02_I2C1_self_check(void)
{
    HAL_StatusTypeDef iic_status;
    HAL_I2C_StateTypeDef iic_state;
    
    /** 检查外设（IIC）是否初始化成功 */
    iic_state = HAL_I2C_GetState(&hi2c1);
    if (iic_state != HAL_I2C_STATE_READY)
    {
        printf("iic state is %d\n", iic_state);
    }
    else
    {
        printf("iic initialized and ready for use\n");
    }
    HAL_Delay(5);
    /** 检查目标设备是否准备好通信 */
    iic_status = HAL_I2C_IsDeviceReady(&hi2c1, AT24C02_ADDRESS, 3, AT24C02_TIMEOUT);
    if (iic_status != HAL_OK)
    {
        printf("IIC STATUS IS %d(2 = HAL_BUSY)\n", iic_status);
    }
    else
    {
        printf("iic status is ok\n");
    }
}

/**
  * @brief  AT24C02的IIC通讯错误处理程序
  * @param  None
  * @retval None
  */
//static void AT24C02_I2C1_error_handle(void)
//{
//    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//    {
//        printf("IIC ERROE Acknowledeg failure");
//    }
//}



