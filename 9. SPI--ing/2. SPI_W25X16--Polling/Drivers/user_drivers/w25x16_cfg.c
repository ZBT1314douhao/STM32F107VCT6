/**
  * @file  w25x16_cfg.c
  * @brief 模板
  * @par   date        version    author    remarks
  *        2016-08-14  v1.0       zbt       初次创建
  *
  */

/** 头文件包含区 ------------------------------------------------ */
#include "w25x16_cfg.h"
#include "spi_dup.h"
#include "gui.h"

/** 私有宏(类型定义) -------------------------------------------- */ 
#define W25X16_PAGE_SIZE                    256

#define W25X16_WRITE_ENABLE					0x06
#define W25X16_WRITE_DISABLE				0x04
#define W25X16_WRITE_STATUS_REG				0X01

#define W25X16_READ_STATUS_REG				0X05
#define W25X16_READ_DATA					0X03
#define W25X16_FAST_READ_DATA				0X0B
/** 快读双输出 */
#define W25X16_FAST_READ_DUAL				0X3B

/** 页编程指令 */
#define W25X16_PAGE_PROGRAM					0X02
/** 块、扇区、芯片擦除指令 */
#define W25X16_BLOCK_ERASE					0XD8
#define W25X16_SECTOR_ERASE					0X20
#define W25X16_CHIP_ERASE					0XC7

/** 掉电指令和释放掉电/器件ID指令 */	
#define W25X16_POWER_DOWN					0XB9
/** 此指令有两个作用 释放掉电&读出器件ID */
#define W25X16_RELEASE_POWER_DOWN			0XAB

/** 读ID指令 */
#define W25X16_DEVICE_ID					0XAB
#define W25X16_MANUFACTURE_DEVICE_ID		0X90
#define W25X16_JEDEC_DEVICE_ID				0X9F
/** W25X16的各种ID */
#define DEVICE_ID                           0x14        
#define MANUFACTURE_ID                      0xEF14     
#define JEDEC_ID                            0xEF3015    
#define MEMORY_ID                           0x30

/** 私有变量 --------------------------------------------------- */ 

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */
static void w25x16_set_cs_pin_value(GPIO_PinState pin_value);
static void w25x16_set_write_enable(FunctionalState state);
static void w25x16_read_status_register(void);
static struct W25X16_ID w25x16_read_id(uint8_t id_select);
/** get_chip_id函数功能为w25x16_read_id的子集 */
static uint32_t get_chip_id(void);  

/** 公有函数 --------------------------------------------------- */
/**
  * @brief  w25x16的SPI测试程序
  * @param	None
  * @retval None
  * @note   通过读取芯片的ID测试主控制器和W25X16的SPI通讯是否正常
  */
void w25x16_test(void)
{
    uint32_t chip_id;
    struct W25X16_ID Id;
    
    Id = w25x16_read_id(2);     /**< 更改参数（0、1、2）可获得各种ID */
       
    printf("memory: 0x%X\r\n", Id.memory);
    printf("compatibility: 0x%X\r\n", Id.compatibility);
    printf("maunfacture: 0x%X\r\n", Id.maunfacture);
    printf("device: 0x%X\r\n", Id.device);
    
    /** 通过以下程序单独获得JEDEC_ID */
    chip_id = get_chip_id();
    
    printf("chip id: 0x%X\r\n", chip_id);
}

/**
  * @brief  写w25x16的状态寄存器
  * @param	written_data：要写入状态寄存器的值
  * @retval None
  */
void w25x16_write_status_register(uint8_t written_data)
{
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_WRITE_STATUS_REG);
    
    spi_readwrite_onebyte(written_data);
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
}    

/**
  * @brief  从w25x16读取数据
  * @param	addr： 读取数据要访问的地址（地址长度24位）
  * @retval None
  * @note   读取到的数据从DO引脚发送出去，高位在前
  */
void w25x16_read_data(uint32_t addr)
{
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_READ_DATA);
    
    /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
    spi_readwrite_onebyte((addr & 0xff0000) >> 16);
    spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
    spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
}

/**
  * @brief  从w25x16快速读取数据（快读和快读双输出）
  * @param	addr：读取数据要访问的地址（地址长度24位）
  * @retval None
  * @note   快读：读取到的数据从DO引脚发送出去，高位在前
            快读双输出：读取到的数据从DO引脚发送出去，
                        DIO送偶数位，DI送基数位
            暂时不用
  */
//void w25x16_fast_read(void)
//{
//    
//}

/**
  * @brief  从w25x16快速读取数据（快读和快读双输出）
  * @param	addr： 读取数据要访问的地址（地址长度24位）
  * @retval None
  * @note   快读： 读取到的数据从DO引脚发送出去，高位在前
            快读双输出： 读取到的数据从DO引脚发送出去，
                        DIO送偶数位，DI送基数位
  */
void w25x16_page_program(uint8_t *buffer, uint32_t addr, uint16_t byte_num)
{
    w25x16_set_write_enable(ENABLE);
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_PAGE_PROGRAM);
    /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
    spi_readwrite_onebyte((addr & 0xff0000) >> 16);
    spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
    spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
    
    byte_num = (byte_num > W25X16_PAGE_SIZE)? W25X16_PAGE_SIZE : byte_num;
    
    while (byte_num--)
    {
        spi_readwrite_onebyte(*buffer);
        buffer++;
    }
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
    /** 读状态寄存器 确认写操作完毕 */
    w25x16_read_status_register();
}

/**
  * @brief  擦除w25x16的块 or 页 or 整个芯片的数据
  * @param	EraseArea：选择擦除的区域
                @arg： BLOCK_ERASE
                @arg： SECTOR_ERASE
                @arg： CHIP_ERASE
            addr： 24位的地址，仅在块 or 扇区擦除的时候使用此参数
  * @retval None
  * @note   读取到的数据从DO引脚发送出去，高位在前
  */
void w25x16_erase(enum W25X16_ERASE EraseArea, uint32_t addr)
{
    uint8_t area_select;
    
    w25x16_set_write_enable(ENABLE);
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    if (area_select == BLOCK_ERASE)
    {
        spi_readwrite_onebyte(W25X16_BLOCK_ERASE);
        /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
        spi_readwrite_onebyte((addr & 0xff0000) >> 16);
        spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
        spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
    }
    else if (area_select == SECTOR_ERASE)
    {
        spi_readwrite_onebyte(W25X16_SECTOR_ERASE);
         /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
        spi_readwrite_onebyte((addr & 0xff0000) >> 16);
        spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
        spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
        
    }
    else if (area_select == CHIP_ERASE)
    {
        spi_readwrite_onebyte(W25X16_CHIP_ERASE);
    }
    else
    {
        ;
    }
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
    w25x16_read_status_register();      /**< 等待器件指令执行完毕 */
    
}

/**
  * @brief  设置w25x16的电源状态
  * @param	state： 掉电 or 释放掉电指令
            @arg： ENABLE    释放掉电执行
            @arg： DISABLE   掉电指令
  * @retval 
  */
void w25x16_set_power(FunctionalState state)
{
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    if (state == DISABLE)
    {
        spi_readwrite_onebyte(W25X16_POWER_DOWN);
    }
    else 
    {
        spi_readwrite_onebyte(W25X16_RELEASE_POWER_DOWN);
    }
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);

}

/** 私有函数 --------------------------------------------------- */
/**
  * @brief  设置w25x16的 写使能状态
  * @param	state： 写使能/失能指令
            @arg： ENABLE    写使能
            @arg： DISABLE   写失能
  * @retval None
  */
static void w25x16_set_write_enable(FunctionalState state)
{
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    if (state == DISABLE)
    {
        spi_readwrite_onebyte(W25X16_WRITE_DISABLE);
    }
    else
    {
        spi_readwrite_onebyte(W25X16_WRITE_ENABLE);
    }
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
}

/**
  * @brief  读w25x16的状态寄存器
  * @param	None
  * @retval None
  * @note   读取BUSY位的值，用于判断编程、擦除和写状态寄存器的周期有没有结束
            从而可以判断芯片是否可以接收下一条指令
            BUSY = 1，器件正忙；BUSY = 0， 器件可以接收其他指令
  */
static void w25x16_read_status_register(void)
{
    uint8_t status = RESET;
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_READ_STATUS_REG);
    
    do
    {
        /** 发送伪字节以得到BUSY的值 */
        status = spi_readwrite_onebyte(DUMMY_BYTE);
    }
    while ((status & 0x01) == 0x01);      /**< 等待非忙标志 */

    w25x16_set_cs_pin_value(GPIO_PIN_SET);    
} 

/**
  * @brief  设置w25x16的CS引脚的电平值
  * @param	pin_value： CS引脚的电平值
  * @retval None
  * @note   开发板原理图显示PB9为液晶屏和W25X16的共用CS引脚
  */
static void w25x16_set_cs_pin_value(GPIO_PinState pin_value)
{
	if (pin_value == GPIO_PIN_RESET)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	}
    else if (pin_value == GPIO_PIN_SET)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    }
    else
    {
        ;
    }
}

/**
  * @brief  获取w25x16的JEDEC_ID
  * @param	None
  * @retval tmp： 芯片的JEDEC_ID
  */
static uint32_t get_chip_id(void)
{
    uint32_t tmp, tmp0, tmp1, tmp2;
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_JEDEC_DEVICE_ID);
    tmp0 = spi_readwrite_onebyte(DUMMY_BYTE);
    tmp1 = spi_readwrite_onebyte(DUMMY_BYTE);
    tmp2 = spi_readwrite_onebyte(DUMMY_BYTE);
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
    
    tmp = (tmp0 << 16) | (tmp1 << 8) | tmp2;
    
    return tmp;
}

/**
  * @brief  读取w25x16的 ID
  * @param	id_select：  选择读取哪个ID
                @arg： 0 读取W25X16_JEDEC_DEVICE_ID
                @arg： 1 读取W25X16_DEVICE_ID
                @arg： 2 读取W25X16_MANUFACTURE_DEVICE_ID
  * @retval Id： 包含读取到的ID的结构体
  * @note   将读到的ID拆分并存放到结构体中
  */
static struct W25X16_ID w25x16_read_id(uint8_t id_select)
{
    const uint32_t addr = 0x000000;
    struct W25X16_ID Id = {0, 0, 0, 0, };
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    if (id_select == 0)         /**< 读取JEDEC ID */
    {
        spi_readwrite_onebyte(W25X16_JEDEC_DEVICE_ID);
        Id.maunfacture = spi_readwrite_onebyte(DUMMY_BYTE);
        Id.memory = spi_readwrite_onebyte(DUMMY_BYTE);
        Id.compatibility = spi_readwrite_onebyte(DUMMY_BYTE);
    }
    else if (id_select == 1)    /**< 读取器件ID */
    {
        spi_readwrite_onebyte(W25X16_DEVICE_ID);
        spi_readwrite_onebyte(DUMMY_BYTE);
        spi_readwrite_onebyte(DUMMY_BYTE);
        spi_readwrite_onebyte(DUMMY_BYTE);
        
        Id.device = spi_readwrite_onebyte(DUMMY_BYTE);
    }
    else if (id_select == 2)    /**< 读取制造 & 器件 ID */
    {
        spi_readwrite_onebyte(W25X16_MANUFACTURE_DEVICE_ID);
        /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
        spi_readwrite_onebyte((addr & 0xff0000) >> 16);
        spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
        spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
        
        Id.maunfacture = spi_readwrite_onebyte(DUMMY_BYTE);
        Id.device = spi_readwrite_onebyte(DUMMY_BYTE);
    }
    else
    {
        ;
    }
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
    
    return Id;
}
