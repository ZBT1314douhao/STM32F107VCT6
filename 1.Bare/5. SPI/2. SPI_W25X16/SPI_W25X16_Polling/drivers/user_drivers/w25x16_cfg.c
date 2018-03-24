/**
  * @file  w25x16_cfg.c
  * @brief w25x16驱动程序
  * @par   date        version    author    remarks
  *        2016-08-14  v1.0       zbt       初次创建
  *                                         bug: TEST_BUFFER_SIZE的大小最多为100
                                            大于100测试程序运行失败
  */

/** 头文件包含区 ------------------------------------------------ */
#include "w25x16_cfg.h"
#include "spi_dup.h"

/** 私有宏(类型定义) -------------------------------------------- */ 
#define DRIVER_TEST         1
#define TEST_BUFFER_SIZE    16     /**< 测试成功的最大值 100 */
/** 读写指令 */
#define W25X16_WRITE_ENABLE					0x06
#define W25X16_WRITE_DISABLE				0x04
#define W25X16_WRITE_STATUS_REG				0X01
#define W25X16_READ_STATUS_REG				0X05
#define W25X16_READ_DATA					0X03
#define W25X16_FAST_READ_DATA				0X0B
#define W25X16_FAST_READ_DUAL				0X3B    /**< 快读双输出 */

/** 页编程指令 */
#define W25X16_PAGE_PROGRAM					0X02

/** 掉电指令和释放掉电/器件ID指令 */	
#define W25X16_POWER_DOWN					0XB9
/** 此指令有两个作用 释放掉电&读出器件ID */
#define W25X16_RELEASE_POWER_DOWN			0XAB

/** 读ID指令 */
#define W25X16_READ_DEVICE_ID			    0XAB
#define W25X16_READ_MANUFACTURE_DEVICE_ID	0X90
#define W25X16_READ_JEDEC_DEVICE_ID			0X9F
/** W25X16的各种ID */
#define W25X16_DEVICE_ID                    0x14        
#define W25X16_MANUFACTURE_ID               0xEF14     
#define W25X16_JEDEC_ID                     0xEF3015    
//#define W25Q16_JEDEC_ID                     0xEF4015    
#define W25X16_MEMORY_ID                    0x30
//#define W25Q16_MEMORY_ID                    0x40


/** 私有变量 --------------------------------------------------- */ 

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */
static void w25x16_set_cs_pin_value(GPIO_PinState pin_value);
static void w25x16_set_write_enable(FunctionalState state);
static void w25x16_send_address(uint32_t addr);
static void w25x16_wait(void);
static struct W25X16_ID w25x16_read_id(uint8_t id_select);

/** 公有函数 --------------------------------------------------- */
/**
  * @brief  w25x16的SPI测试程序
  * @param	None
  * @retval None
  * @note   通过读取芯片的ID测试主控制器和W25X16的SPI通讯是否正常
  */
void w25x16_test(void)
{
    uint16_t i;
    uint8_t read_buffer[TEST_BUFFER_SIZE];
    uint8_t write_buffer[TEST_BUFFER_SIZE];
    
#if (DRIVER_TEST == 0)                      /**< 读取芯片ID测试 */    
    /** 更改参数（0、1、2）可获得各种ID */ 
    printf("read id start\r\n");
    w25x16_read_id(0);    
    
#elif (DRIVER_TEST == 1)                    /**< 读写数据测试 */
    uint8_t compare_flag = 0;
    
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    { 
        write_buffer[i] = i;
    }
    w25x16_page_program(0, write_buffer, TEST_BUFFER_SIZE);
    printf("写入完毕 \r\n");
    w25x16_read(0, read_buffer, TEST_BUFFER_SIZE);
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
        if (read_buffer[i] != write_buffer[i])
        {
            compare_flag = 1;
            break;
        }
    }
    if (compare_flag == 0)
    {
        printf("读写测试成功 读取到的数据是\r\n");
        for (i = 0; i < TEST_BUFFER_SIZE; i++)
        {
            printf("%d ", read_buffer[i]);
        }
        printf("\r\n");
    }
    else
    {
        printf("读取失败\r\n");
    }
    

#elif (DRIVER_TEST == 2)                /**< 擦除数据测试 */    
    printf("开始擦除芯片\r\n");
    w25x16_erase(CHIP_ERASE, 0);
    w25x16_read(0, read_buffer, TEST_BUFFER_SIZE);
    printf("擦出后读取到的数据是 \r\n");
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
//        if (read_buffer[i] != 0xFF)
//        {
//            printf("擦除失败 \r\n");
//            break;
//        }
        printf("%d ", read_buffer[i]);
    }
    if (i == TEST_BUFFER_SIZE)
    {
        printf("擦除数据成功\r\n");
    }
    
#elif (DRIVER_TEST == 3)
    /** 用户添加要测试的驱动程序 */
    
#else
    /** 用户添加要测试的驱动程序 */


#endif
}

/**
  * @brief  写w25x16的状态寄存器
  * @param	data：要写入状态寄存器的值
  * @retval None
  * @note   芯片上电之后自动进入写保护状态（状态寄存器中 WEL 位为0）
            在执行页编程、扇区擦除、写状态寄存器指令之前，必须先执行
            写使能指令，执行完以上动作后，自动恢复写禁能状态
            状态寄存器的部分位为只读 因此只有特定为可以写入
  */
void w25x16_write_status_register(uint8_t data)
{
    w25x16_set_write_enable(ENABLE);
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    /** 先发送写状态寄存器指令 再将数据写入状态寄存器 */    
    spi_readwrite_onebyte(W25X16_WRITE_STATUS_REG);   
    spi_readwrite_onebyte(data);    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
}    

/**
  * @brief  读w25x16的状态寄存器
  * @param	None
  * @retval None
  * @note   读取到的数据可以用来判定芯片存储器阵列的可写状态
            或者是否处于写保护状态
  */
uint8_t w25x16_read_status_register(void)
{
    uint8_t value;
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    /** 先发送读取状态寄存器指令 再读取状态寄存器 */
    spi_readwrite_onebyte(W25X16_READ_STATUS_REG);
    value = spi_readwrite_onebyte(DUMMY_BYTE);
    w25x16_set_cs_pin_value(GPIO_PIN_SET);   
    
    return value;
} 

/**
  * @brief  向w25x16写入数据
  * @param	addr： 写入数据的地址（地址长度24位）
  * @param	buffer： 要写入的数据
  * @param	byte_num： 要写入数据的字节长度（小于65535）
  * @retval None
  * @note   
            写addr处的数据后，addr自动增加
  */
void w25x16_write(uint32_t addr, uint8_t *buffer, uint16_t byte_num)
{
    
}

/**
  * @brief  从w25x16读取数据
  * @param	addr： 读取数据要访问的地址（地址长度24位）
  * @param	buffer： 存放读取到的数据
  * @param	byte_num： 要读取数据的字节长度（小于65535）
  * @retval None
  * @note   读取到的数据从DO引脚发送出去，高位在前，
            读完addr处的数据后，addr自动增加
  */
void w25x16_read(uint32_t addr, uint8_t *buffer, uint16_t byte_num)
{    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_READ_DATA);   
    /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
//    spi_readwrite_onebyte((uint8_t)((addr & 0xff0000) >> 16));
//    spi_readwrite_onebyte((uint8_t)((addr & 0x00ff00) >> 8));
//    spi_readwrite_onebyte((uint8_t)((addr & 0x0000ff) >> 0));
    w25x16_send_address(addr);
    
    byte_num = (byte_num > W25X16_PAGE_SIZE)? W25X16_PAGE_SIZE : byte_num;
    while (byte_num--)
    {
        *buffer = spi_readwrite_onebyte(DUMMY_BYTE);
        buffer++;
    }
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
  * @brief  从w25x16页编程指令 每次可以编程256B
  * @param	addr： 写入数据的地址（地址长度24位）
  * @param	buffer： 要写入的数据指针
  * @param	byte_num： 要写入数据的字节长度（小于256）
  * @retval None
  * @note   芯片上电之后自动进入写保护状态（状态寄存器中 WEL 位为0）
            在执行页编程、扇区擦除、写状态寄存器指令之前，必须先执行
            写使能指令，执行完以上动作后，自动恢复写禁能状态
            最大可编程一页(256B)的数据
  */
void w25x16_page_program(uint32_t addr, uint8_t *buffer, uint16_t byte_num)
{
    uint16_t i;
    
    w25x16_set_write_enable(ENABLE);
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_PAGE_PROGRAM);
    /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
//    spi_readwrite_onebyte((uint8_t)((addr & 0xff0000) >> 16));
//    spi_readwrite_onebyte((uint8_t)((addr & 0x00ff00) >> 8));
//    spi_readwrite_onebyte((uint8_t)((addr & 0x0000ff) >> 0));
    w25x16_send_address(addr);
    
    byte_num = (byte_num >= W25X16_PAGE_SIZE)? W25X16_PAGE_SIZE : byte_num;
    for (i = 0; i < byte_num; i++)
    {
        spi_readwrite_onebyte(buffer[i]);
    }
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
    /** 读状态寄存器 确认写操作完毕 */
    w25x16_wait();
}

/**
  * @brief  擦除w25x16的块 or 页 or 整个芯片的数据
  * @param	EraseArea：选择擦除的区域
                @arg： BLOCK_ERASE  可以擦除16页    4K
                @arg： SECTOR_ERASE 可以擦除256页   64K
                @arg： CHIP_ERASE   可擦除整个芯片   耗时很长
  * @param	addr： 24位的地址，仅在块 or 扇区擦除的时候使用此参数
            对于BLOCK_ERASE  addr 从0-31
            对于SECTOR_ERASE addr 从0-511
  * @retval None
  * @note   芯片上电之后自动进入写保护状态（状态寄存器中 WEL 位为0）
            在执行页编程、扇区擦除、写状态寄存器指令之前，必须先执行
            写使能指令，执行完以上动作后，自动恢复写禁能状态
            读取到的数据从DO引脚发送出去，高位在前
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
//        spi_readwrite_onebyte((addr & 0xff0000) >> 16);
//        spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
//        spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
        w25x16_send_address(addr);
    }
    else if (area_select == SECTOR_ERASE)
    {
        spi_readwrite_onebyte(W25X16_SECTOR_ERASE);
         /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
//        spi_readwrite_onebyte((addr & 0xff0000) >> 16);
//        spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
//        spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
        w25x16_send_address(addr);
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
    w25x16_wait();      /**< 等待器件指令执行完毕 */
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
  * @brief  读w25x16的状态寄存器BUSY位的值
  * @param	None
  * @retval None
  * @note   读取BUSY位的值，用于判断编程、擦除和写状态寄存器的周期有没有结束
            从而可以判断芯片是否可以接收下一条指令
            BUSY = 1，器件正忙；BUSY = 0， 器件可以接收其他指令
  */
static void w25x16_wait(void)
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
//    while ((w25x16_read_status_register() & 0x01) == 0x01);
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
  * @brief  向w25x16发送地址
  * @param	addr： 要发送的24位地址
  * @retval None
  */
static void w25x16_send_address(uint32_t addr)
{
    /** 发送24位的地址 以8位为单位分三次依次发送（从高字节开始发送） */
    spi_readwrite_onebyte((uint8_t)((addr & 0xff0000) >> 16));
    spi_readwrite_onebyte((uint8_t)((addr & 0x00ff00) >> 8));
    spi_readwrite_onebyte((uint8_t)((addr & 0x0000ff) >> 0));
//    spi_readwrite_onebyte((addr & 0xff0000) >> 16);
//    spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
//    spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
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
    uint32_t tmp0, tmp1, tmp2;
    struct W25X16_ID Id = {0, 0, 0, 0, 0, };
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    printf("id_seclect is %d\r\n", id_select);
    if (id_select == 0)         /**< 读取JEDEC ID */
    {
        spi_readwrite_onebyte(W25X16_READ_JEDEC_DEVICE_ID);
        tmp0 = spi_readwrite_onebyte(DUMMY_BYTE);
        tmp1 = spi_readwrite_onebyte(DUMMY_BYTE);
        tmp2 = spi_readwrite_onebyte(DUMMY_BYTE);
        
        Id.maunfacture = (uint8_t)tmp0;
        Id.memory = (uint8_t)tmp1;
        Id.compatibility = (uint8_t)tmp2;
        Id.jedec_device = (tmp0 << 16) | (tmp1 << 8) | tmp2;
    }
    else if (id_select == 1)    /**< 读取器件ID */
    {
        spi_readwrite_onebyte(W25X16_READ_DEVICE_ID);
        spi_readwrite_onebyte(DUMMY_BYTE);
        spi_readwrite_onebyte(DUMMY_BYTE);
        spi_readwrite_onebyte(DUMMY_BYTE);
        
        Id.device = spi_readwrite_onebyte(DUMMY_BYTE);
    }
    else if (id_select == 2)    /**< 读取制造 & 器件 ID */
    {
        spi_readwrite_onebyte(W25X16_READ_MANUFACTURE_DEVICE_ID);
        spi_readwrite_onebyte(DUMMY_BYTE);
        spi_readwrite_onebyte(DUMMY_BYTE);
        spi_readwrite_onebyte(0x00);
        
        Id.maunfacture = spi_readwrite_onebyte(DUMMY_BYTE);
        Id.device = spi_readwrite_onebyte(DUMMY_BYTE);
    }
    else
    {
        ;
    }
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
    w25x16_wait();
    
    printf("memory: 0x%X\r\n", Id.memory);
    printf("compatibility: 0x%X\r\n", Id.compatibility);
    printf("maunfacture: 0x%X\r\n", Id.maunfacture);
    printf("device: 0x%X\r\n", Id.device);
    printf("jedec device: 0x%X\r\n", Id.jedec_device);
    
    return Id;
}
