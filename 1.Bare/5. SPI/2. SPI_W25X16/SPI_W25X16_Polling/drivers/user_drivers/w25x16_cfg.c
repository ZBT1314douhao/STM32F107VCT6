/**
  * @file  w25x16_cfg.c
  * @brief w25x16��������
  * @par   date        version    author    remarks
  *        2016-08-14  v1.0       zbt       ���δ���
  *                                         bug: TEST_BUFFER_SIZE�Ĵ�С���Ϊ100
                                            ����100���Գ�������ʧ��
  */

/** ͷ�ļ������� ------------------------------------------------ */
#include "w25x16_cfg.h"
#include "spi_dup.h"

/** ˽�к�(���Ͷ���) -------------------------------------------- */ 
#define DRIVER_TEST         1
#define TEST_BUFFER_SIZE    16     /**< ���Գɹ������ֵ 100 */
/** ��дָ�� */
#define W25X16_WRITE_ENABLE					0x06
#define W25X16_WRITE_DISABLE				0x04
#define W25X16_WRITE_STATUS_REG				0X01
#define W25X16_READ_STATUS_REG				0X05
#define W25X16_READ_DATA					0X03
#define W25X16_FAST_READ_DATA				0X0B
#define W25X16_FAST_READ_DUAL				0X3B    /**< ���˫��� */

/** ҳ���ָ�� */
#define W25X16_PAGE_PROGRAM					0X02

/** ����ָ����ͷŵ���/����IDָ�� */	
#define W25X16_POWER_DOWN					0XB9
/** ��ָ������������ �ͷŵ���&��������ID */
#define W25X16_RELEASE_POWER_DOWN			0XAB

/** ��IDָ�� */
#define W25X16_READ_DEVICE_ID			    0XAB
#define W25X16_READ_MANUFACTURE_DEVICE_ID	0X90
#define W25X16_READ_JEDEC_DEVICE_ID			0X9F
/** W25X16�ĸ���ID */
#define W25X16_DEVICE_ID                    0x14        
#define W25X16_MANUFACTURE_ID               0xEF14     
#define W25X16_JEDEC_ID                     0xEF3015    
//#define W25Q16_JEDEC_ID                     0xEF4015    
#define W25X16_MEMORY_ID                    0x30
//#define W25Q16_MEMORY_ID                    0x40


/** ˽�б��� --------------------------------------------------- */ 

/** �ⲿ���� --------------------------------------------------- */

/** ˽�к���ԭ�� ----------------------------------------------- */
static void w25x16_set_cs_pin_value(GPIO_PinState pin_value);
static void w25x16_set_write_enable(FunctionalState state);
static void w25x16_send_address(uint32_t addr);
static void w25x16_wait(void);
static struct W25X16_ID w25x16_read_id(uint8_t id_select);

/** ���к��� --------------------------------------------------- */
/**
  * @brief  w25x16��SPI���Գ���
  * @param	None
  * @retval None
  * @note   ͨ����ȡоƬ��ID��������������W25X16��SPIͨѶ�Ƿ�����
  */
void w25x16_test(void)
{
    uint16_t i;
    uint8_t read_buffer[TEST_BUFFER_SIZE];
    uint8_t write_buffer[TEST_BUFFER_SIZE];
    
#if (DRIVER_TEST == 0)                      /**< ��ȡоƬID���� */    
    /** ���Ĳ�����0��1��2���ɻ�ø���ID */ 
    printf("read id start\r\n");
    w25x16_read_id(0);    
    
#elif (DRIVER_TEST == 1)                    /**< ��д���ݲ��� */
    uint8_t compare_flag = 0;
    
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    { 
        write_buffer[i] = i;
    }
    w25x16_page_program(0, write_buffer, TEST_BUFFER_SIZE);
    printf("д����� \r\n");
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
        printf("��д���Գɹ� ��ȡ����������\r\n");
        for (i = 0; i < TEST_BUFFER_SIZE; i++)
        {
            printf("%d ", read_buffer[i]);
        }
        printf("\r\n");
    }
    else
    {
        printf("��ȡʧ��\r\n");
    }
    

#elif (DRIVER_TEST == 2)                /**< �������ݲ��� */    
    printf("��ʼ����оƬ\r\n");
    w25x16_erase(CHIP_ERASE, 0);
    w25x16_read(0, read_buffer, TEST_BUFFER_SIZE);
    printf("�������ȡ���������� \r\n");
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
//        if (read_buffer[i] != 0xFF)
//        {
//            printf("����ʧ�� \r\n");
//            break;
//        }
        printf("%d ", read_buffer[i]);
    }
    if (i == TEST_BUFFER_SIZE)
    {
        printf("�������ݳɹ�\r\n");
    }
    
#elif (DRIVER_TEST == 3)
    /** �û����Ҫ���Ե��������� */
    
#else
    /** �û����Ҫ���Ե��������� */


#endif
}

/**
  * @brief  дw25x16��״̬�Ĵ���
  * @param	data��Ҫд��״̬�Ĵ�����ֵ
  * @retval None
  * @note   оƬ�ϵ�֮���Զ�����д����״̬��״̬�Ĵ����� WEL λΪ0��
            ��ִ��ҳ��̡�����������д״̬�Ĵ���ָ��֮ǰ��������ִ��
            дʹ��ָ�ִ�������϶������Զ��ָ�д����״̬
            ״̬�Ĵ����Ĳ���λΪֻ�� ���ֻ���ض�Ϊ����д��
  */
void w25x16_write_status_register(uint8_t data)
{
    w25x16_set_write_enable(ENABLE);
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    /** �ȷ���д״̬�Ĵ���ָ�� �ٽ�����д��״̬�Ĵ��� */    
    spi_readwrite_onebyte(W25X16_WRITE_STATUS_REG);   
    spi_readwrite_onebyte(data);    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
}    

/**
  * @brief  ��w25x16��״̬�Ĵ���
  * @param	None
  * @retval None
  * @note   ��ȡ�������ݿ��������ж�оƬ�洢�����еĿ�д״̬
            �����Ƿ���д����״̬
  */
uint8_t w25x16_read_status_register(void)
{
    uint8_t value;
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    /** �ȷ��Ͷ�ȡ״̬�Ĵ���ָ�� �ٶ�ȡ״̬�Ĵ��� */
    spi_readwrite_onebyte(W25X16_READ_STATUS_REG);
    value = spi_readwrite_onebyte(DUMMY_BYTE);
    w25x16_set_cs_pin_value(GPIO_PIN_SET);   
    
    return value;
} 

/**
  * @brief  ��w25x16д������
  * @param	addr�� д�����ݵĵ�ַ����ַ����24λ��
  * @param	buffer�� Ҫд�������
  * @param	byte_num�� Ҫд�����ݵ��ֽڳ��ȣ�С��65535��
  * @retval None
  * @note   
            дaddr�������ݺ�addr�Զ�����
  */
void w25x16_write(uint32_t addr, uint8_t *buffer, uint16_t byte_num)
{
    
}

/**
  * @brief  ��w25x16��ȡ����
  * @param	addr�� ��ȡ����Ҫ���ʵĵ�ַ����ַ����24λ��
  * @param	buffer�� ��Ŷ�ȡ��������
  * @param	byte_num�� Ҫ��ȡ���ݵ��ֽڳ��ȣ�С��65535��
  * @retval None
  * @note   ��ȡ�������ݴ�DO���ŷ��ͳ�ȥ����λ��ǰ��
            ����addr�������ݺ�addr�Զ�����
  */
void w25x16_read(uint32_t addr, uint8_t *buffer, uint16_t byte_num)
{    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_READ_DATA);   
    /** ����24λ�ĵ�ַ ��8λΪ��λ���������η��ͣ��Ӹ��ֽڿ�ʼ���ͣ� */
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
  * @brief  ��w25x16���ٶ�ȡ���ݣ�����Ϳ��˫�����
  * @param	addr����ȡ����Ҫ���ʵĵ�ַ����ַ����24λ��
  * @retval None
  * @note   �������ȡ�������ݴ�DO���ŷ��ͳ�ȥ����λ��ǰ
            ���˫�������ȡ�������ݴ�DO���ŷ��ͳ�ȥ��
                        DIO��ż��λ��DI�ͻ���λ
            ��ʱ����
  */
//void w25x16_fast_read(void)
//{
//    
//}

/**
  * @brief  ��w25x16ҳ���ָ�� ÿ�ο��Ա��256B
  * @param	addr�� д�����ݵĵ�ַ����ַ����24λ��
  * @param	buffer�� Ҫд�������ָ��
  * @param	byte_num�� Ҫд�����ݵ��ֽڳ��ȣ�С��256��
  * @retval None
  * @note   оƬ�ϵ�֮���Զ�����д����״̬��״̬�Ĵ����� WEL λΪ0��
            ��ִ��ҳ��̡�����������д״̬�Ĵ���ָ��֮ǰ��������ִ��
            дʹ��ָ�ִ�������϶������Զ��ָ�д����״̬
            ���ɱ��һҳ(256B)������
  */
void w25x16_page_program(uint32_t addr, uint8_t *buffer, uint16_t byte_num)
{
    uint16_t i;
    
    w25x16_set_write_enable(ENABLE);
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_PAGE_PROGRAM);
    /** ����24λ�ĵ�ַ ��8λΪ��λ���������η��ͣ��Ӹ��ֽڿ�ʼ���ͣ� */
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
    /** ��״̬�Ĵ��� ȷ��д������� */
    w25x16_wait();
}

/**
  * @brief  ����w25x16�Ŀ� or ҳ or ����оƬ������
  * @param	EraseArea��ѡ�����������
                @arg�� BLOCK_ERASE  ���Բ���16ҳ    4K
                @arg�� SECTOR_ERASE ���Բ���256ҳ   64K
                @arg�� CHIP_ERASE   �ɲ�������оƬ   ��ʱ�ܳ�
  * @param	addr�� 24λ�ĵ�ַ�����ڿ� or ����������ʱ��ʹ�ô˲���
            ����BLOCK_ERASE  addr ��0-31
            ����SECTOR_ERASE addr ��0-511
  * @retval None
  * @note   оƬ�ϵ�֮���Զ�����д����״̬��״̬�Ĵ����� WEL λΪ0��
            ��ִ��ҳ��̡�����������д״̬�Ĵ���ָ��֮ǰ��������ִ��
            дʹ��ָ�ִ�������϶������Զ��ָ�д����״̬
            ��ȡ�������ݴ�DO���ŷ��ͳ�ȥ����λ��ǰ
  */
void w25x16_erase(enum W25X16_ERASE EraseArea, uint32_t addr)
{
    uint8_t area_select;
    
    w25x16_set_write_enable(ENABLE);
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    if (area_select == BLOCK_ERASE)
    {
        spi_readwrite_onebyte(W25X16_BLOCK_ERASE);
        /** ����24λ�ĵ�ַ ��8λΪ��λ���������η��ͣ��Ӹ��ֽڿ�ʼ���ͣ� */
//        spi_readwrite_onebyte((addr & 0xff0000) >> 16);
//        spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
//        spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
        w25x16_send_address(addr);
    }
    else if (area_select == SECTOR_ERASE)
    {
        spi_readwrite_onebyte(W25X16_SECTOR_ERASE);
         /** ����24λ�ĵ�ַ ��8λΪ��λ���������η��ͣ��Ӹ��ֽڿ�ʼ���ͣ� */
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
    w25x16_wait();      /**< �ȴ�����ָ��ִ����� */
}

/**
  * @brief  ����w25x16�ĵ�Դ״̬
  * @param	state�� ���� or �ͷŵ���ָ��
            @arg�� ENABLE    �ͷŵ���ִ��
            @arg�� DISABLE   ����ָ��
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

/** ˽�к��� --------------------------------------------------- */
/**
  * @brief  ����w25x16�� дʹ��״̬
  * @param	state�� дʹ��/ʧ��ָ��
            @arg�� ENABLE    дʹ��
            @arg�� DISABLE   дʧ��
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
  * @brief  ��w25x16��״̬�Ĵ���BUSYλ��ֵ
  * @param	None
  * @retval None
  * @note   ��ȡBUSYλ��ֵ�������жϱ�̡�������д״̬�Ĵ�����������û�н���
            �Ӷ������ж�оƬ�Ƿ���Խ�����һ��ָ��
            BUSY = 1��������æ��BUSY = 0�� �������Խ�������ָ��
  */
static void w25x16_wait(void)
{
    uint8_t status = RESET;
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    spi_readwrite_onebyte(W25X16_READ_STATUS_REG);
    
    do
    {
        /** ����α�ֽ��Եõ�BUSY��ֵ */
        status = spi_readwrite_onebyte(DUMMY_BYTE);
    }
    while ((status & 0x01) == 0x01);      /**< �ȴ���æ��־ */

    w25x16_set_cs_pin_value(GPIO_PIN_SET);   
//    while ((w25x16_read_status_register() & 0x01) == 0x01);
} 

/**
  * @brief  ����w25x16��CS���ŵĵ�ƽֵ
  * @param	pin_value�� CS���ŵĵ�ƽֵ
  * @retval None
  * @note   ������ԭ��ͼ��ʾPB9ΪҺ������W25X16�Ĺ���CS����
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
  * @brief  ��w25x16���͵�ַ
  * @param	addr�� Ҫ���͵�24λ��ַ
  * @retval None
  */
static void w25x16_send_address(uint32_t addr)
{
    /** ����24λ�ĵ�ַ ��8λΪ��λ���������η��ͣ��Ӹ��ֽڿ�ʼ���ͣ� */
    spi_readwrite_onebyte((uint8_t)((addr & 0xff0000) >> 16));
    spi_readwrite_onebyte((uint8_t)((addr & 0x00ff00) >> 8));
    spi_readwrite_onebyte((uint8_t)((addr & 0x0000ff) >> 0));
//    spi_readwrite_onebyte((addr & 0xff0000) >> 16);
//    spi_readwrite_onebyte((addr & 0x00ff00) >> 8);
//    spi_readwrite_onebyte((addr & 0x0000ff) >> 0);
}

/**
  * @brief  ��ȡw25x16�� ID
  * @param	id_select��  ѡ���ȡ�ĸ�ID
                @arg�� 0 ��ȡW25X16_JEDEC_DEVICE_ID
                @arg�� 1 ��ȡW25X16_DEVICE_ID
                @arg�� 2 ��ȡW25X16_MANUFACTURE_DEVICE_ID
  * @retval Id�� ������ȡ����ID�Ľṹ��
  * @note   ��������ID��ֲ���ŵ��ṹ����
  */
static struct W25X16_ID w25x16_read_id(uint8_t id_select)
{
    uint32_t tmp0, tmp1, tmp2;
    struct W25X16_ID Id = {0, 0, 0, 0, 0, };
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    printf("id_seclect is %d\r\n", id_select);
    if (id_select == 0)         /**< ��ȡJEDEC ID */
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
    else if (id_select == 1)    /**< ��ȡ����ID */
    {
        spi_readwrite_onebyte(W25X16_READ_DEVICE_ID);
        spi_readwrite_onebyte(DUMMY_BYTE);
        spi_readwrite_onebyte(DUMMY_BYTE);
        spi_readwrite_onebyte(DUMMY_BYTE);
        
        Id.device = spi_readwrite_onebyte(DUMMY_BYTE);
    }
    else if (id_select == 2)    /**< ��ȡ���� & ���� ID */
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
