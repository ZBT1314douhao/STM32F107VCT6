/**
  * @file  w25x16_cfg.c
  * @brief w25x16��������
  * @par   date        version    author    remarks
  *        2016-08-14  v1.0       zbt       ���δ���
  *
  */

/** ͷ�ļ������� ------------------------------------------------ */
#include "w25x16_cfg.h"
#include "spi_dup.h"

/** ˽�к�(���Ͷ���) -------------------------------------------- */ 
//#define countof(a) (sizeof(a) / sizeof(*(a)))
#define COUNTOF(buffer)                     (sizeof(buffer) / sizeof(buffer[0]))

#define W25X16_CS_PORT                      GPIOB
#define W25X16_CS_PIN                       GPIO_PIN_9

#define W25X16_PAGE_SIZE                    256
#define W25X16_SECTOR_SIZE                  (16 * W25X16_PAGE_SIZE)
#define W25X16_BLOCK_SIZE                   (256 * W25X16_PAGE_SIZE)
/** ��дָ�� */
#define W25X16_WRITE_ENABLE					0x06
#define W25X16_WRITE_DISABLE				0x04
#define W25X16_WRITE_STATUS_REG				0X01
#define W25X16_READ_STATUS_REG				0X05
#define W25X16_READ_DATA					0X03
#define W25X16_FAST_READ_DATA				0X0B
/** ���˫��� */
#define W25X16_FAST_READ_DUAL				0X3B

/** ҳ���ָ�� */
#define W25X16_PAGE_PROGRAM					0X02
/** �顢������оƬ����ָ�� */
#define W25X16_BLOCK_ERASE					0XD8    /**< 64KB */
#define W25X16_SECTOR_ERASE					0X20    /**< 4kB */
#define W25X16_CHIP_ERASE					0XC7

/** ����ָ����ͷŵ���/����IDָ�� */	
#define W25X16_POWER_DOWN					0XB9
/** ��ָ������������ �ͷŵ���&��������ID */
#define W25X16_RELEASE_POWER_DOWN			0XAB\

/** ��IDָ�� */
#define W25X16_READ_DEVICE_ID			    0XAB
#define W25X16_READ_MANUFACTURE_DEVICE_ID	0X90
#define W25X16_READ_JEDEC_DEVICE_ID			0X9F
/** W25X16�ĸ���ID */
#define W25X16_DEVICE_ID                    0x14        
#define W25X16_MANUFACTURE_ID               0xEF14     
#define W25X16_JEDEC_ID                     0xEF3015    /**< W25Q16 0xef4015 */
#define W25X16_MEMORY_ID                    0x30        /**< W25Q16 0x40 */

/** ˽�б��� --------------------------------------------------- */ 
#define DRIVER_TEST                         1

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
    /** 200Ϊ���Գ����ó��ȵ����ֵ�� ����200�����룬
        �򴮿ڴ�ӡ��w25x16 test��ֹͣ��ӡ ԭ���ݲ���
    */
    const uint16_t buffer_length = 100; 
    uint8_t read_buffer[buffer_length];
    uint8_t write_buffer[buffer_length];
    uint8_t compare_flag = 0;

#if (DRIVER_TEST == 0)                      /**< ��ȡоƬID���� */    
    /** ���Ĳ�����0��1��2���ɻ�ø���ID */ 
    w25x16_read_id(0);       
#elif (DRIVER_TEST == 1)                    /**< ��д���ݲ��� */    
    for (i = 0; i < buffer_length; i++)
    {
        write_buffer[i] = (i + 1);
        read_buffer[i] = 0;
    }
    for (i = 0; i < buffer_length; i++)
    {
        printf("%d ", write_buffer[i]);
    }
    printf("\r\n");
    w25x16_page_program(0, write_buffer, buffer_length);
    printf("д�����ݳɹ� \r\n");
    
    w25x16_read(0, read_buffer, buffer_length);
    printf("д����ȡ���������� \r\n");
    for (i = 0; i < buffer_length; i++)
    {
        printf("%d ", read_buffer[i]);
    }
    printf("\r\n");
    
    for (i = 0; i < buffer_length; i++)
    {
        if (read_buffer[i] != write_buffer[i])
        {
            compare_flag = 1;
            break;
        }
    }
    if (compare_flag == 0)
    {
        printf("��д���Գɹ�\r\n");
    }
    else
    {
        printf("��д����ʧ��\r\n");
    }
    
#elif (DRIVER_TEST == 2)                /**< �������ݲ��� */       
    
    printf("��ʼ����оƬ\r\n");
    w25x16_erase(CHIP_ERASE, 0);
    printf("������� \r\n");
//    for (i = 0; i < buffer_length; i++)
//    {
//        read_buffer[i] = 0;
//    }
//    w25x16_read(0, read_buffer, buffer_length / 1);
//    printf("�������ȡ���������� \r\n");
//    for (i = 0; i < (buffer_length / 1); i++)
//    {
////        if (read_buffer[i] != 0xFF)
////        {
////            printf("����ʧ�� \r\n");
////            break;
////        }
//        printf("%d ", read_buffer[i]);
//    }
//    printf("\r\n");
    
#elif (DRIVER_TEST == 3)

#else
    
#endif
}

/**
  * @brief  дw25x16��״̬�Ĵ���
  * @param	data��Ҫд��״̬�Ĵ�����ֵ
  * @retval None
  * @note   оƬ�ϵ�֮���Զ�����д����״̬��״̬�Ĵ����� WEL λΪ0��
            ��ִ��ҳ��̡�����������д״̬�Ĵ���ָ��֮ǰ��������ִ��
            дʹ��ָ�ִ�������϶������Զ��ָ�д����״̬
            ״̬�Ĵ����Ĳ���λΪֻ�� ���ֻ���ض�Ϊ����д�� bit 7.5.4.3.2
  */
void w25x16_write_status_register(uint8_t data)
{
    w25x16_set_write_enable(ENABLE);
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    spi_flash_write_onebyte(W25X16_WRITE_STATUS_REG);   
    spi_flash_write_onebyte(data);    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
}    

/**
  * @brief  ��w25x16��״̬�Ĵ���
  * @param	None
  * @retval value�� ��״̬�Ĵ����ж�ȡ����ֵ
  * @note   ��ȡ�������ݿ��������ж�оƬ�洢�����еĿ�д״̬
            �����Ƿ���д����״̬
  */
uint8_t w25x16_read_status_register(void)
{
    uint8_t value;
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    spi_flash_write_onebyte(W25X16_READ_STATUS_REG);
    value = spi_flash_read_onebyte();
    w25x16_set_cs_pin_value(GPIO_PIN_SET);   
    
    return value;
} 

/**
  * @brief  ��w25x16д������
  * @param	addr�� д�����ݵĵ�ַ����ַ����24λ��
  * @param	buffer�� Ҫд�������
  * @param	byte_num�� Ҫд�����ݵ��ֽڳ��ȣ�С��65535��
  * @retval None
  * @note   дaddr�������ݺ�addr�Զ�����
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
    
    spi_flash_write_onebyte(W25X16_READ_DATA);   
    w25x16_send_address(addr);   
    byte_num = (byte_num > W25X16_PAGE_SIZE)? W25X16_PAGE_SIZE : byte_num;
    while (byte_num--)
    {
        *buffer = spi_flash_read_onebyte();
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
    uint8_t i;
    
    w25x16_set_write_enable(ENABLE);    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);    
    spi_flash_write_onebyte(W25X16_PAGE_PROGRAM);
    w25x16_send_address(addr);
    byte_num = (byte_num >= W25X16_PAGE_SIZE)? W25X16_PAGE_SIZE : byte_num;   
//    spi_flash_transmit(buffer, byte_num);
//    while (byte_num--)
//    {
//        spi_readwrite_onebyte(*buffer++);
//    }
    for (i = 0; i < byte_num; i++)
    {
        spi_flash_write_onebyte(buffer[i]);
    }
    w25x16_set_cs_pin_value(GPIO_PIN_SET);        
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
    w25x16_wait();
    printf("����ǰ״̬�Ĵ�����ֵ %x \r\n", w25x16_read_status_register());
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    
    if (area_select == BLOCK_ERASE)
    {
        /** 32���ɲ����� */
        addr = (addr >= 31)? (31 * W25X16_BLOCK_SIZE) : (addr * W25X16_BLOCK_SIZE);
        spi_flash_write_onebyte(W25X16_BLOCK_ERASE);
        w25x16_send_address(addr);
    }
    else if (area_select == SECTOR_ERASE)
    {
        /** 512���ɲ������� */
        addr = (addr >= 511)? (511 * W25X16_SECTOR_SIZE) : (addr * W25X16_SECTOR_SIZE);
        spi_flash_write_onebyte(W25X16_SECTOR_ERASE);
        w25x16_send_address(addr);
    }
    else if (area_select == CHIP_ERASE)
    {
        spi_flash_write_onebyte(W25X16_CHIP_ERASE);
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
        spi_flash_write_onebyte(W25X16_POWER_DOWN);
    }
    else 
    {
        spi_flash_write_onebyte(W25X16_RELEASE_POWER_DOWN);
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
        spi_flash_write_onebyte(W25X16_WRITE_DISABLE);
    }
    else
    {
        spi_flash_write_onebyte(W25X16_WRITE_ENABLE);
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
    uint8_t status = 0x00;
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);   
    spi_flash_write_onebyte(W25X16_READ_STATUS_REG);
    
    do
    {
        /** ����α�ֽ��Եõ�BUSY��ֵ */
        status = spi_flash_read_onebyte();
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
		HAL_GPIO_WritePin(W25X16_CS_PORT, W25X16_CS_PIN, GPIO_PIN_RESET);
	}
    else if (pin_value == GPIO_PIN_SET)
    {
        HAL_GPIO_WritePin(W25X16_CS_PORT, W25X16_CS_PIN, GPIO_PIN_SET);
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
    uint8_t address[3];
    
    address[0] = (uint8_t)((addr & 0xff0000) >> 16);
    address[1] = (uint8_t)((addr & 0xff0000) >> 8);
    address[2] = (uint8_t)((addr & 0xff0000) >> 0);
    spi_flash_transmit(address, COUNTOF(address));
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
    uint8_t tmp[4];
    uint8_t jedec_device_cmd[] = {W25X16_READ_JEDEC_DEVICE_ID, SPI_FLASH_DUMMY_BYTE, 
                                SPI_FLASH_DUMMY_BYTE, SPI_FLASH_DUMMY_BYTE, };
    uint8_t device_cmd[] = {W25X16_READ_DEVICE_ID, SPI_FLASH_DUMMY_BYTE, SPI_FLASH_DUMMY_BYTE, 
                            SPI_FLASH_DUMMY_BYTE, };
    uint8_t manufacture_device_cmd[] = {W25X16_READ_MANUFACTURE_DEVICE_ID, 
                                        SPI_FLASH_DUMMY_BYTE, SPI_FLASH_DUMMY_BYTE, 0x00, };
    struct W25X16_ID Id = {0, 0, 0, 0, 0, };
    
    
    w25x16_set_cs_pin_value(GPIO_PIN_RESET);
    if (id_select == 0)         /**< ��ȡJEDEC ID */
    {
        /** ���յ��ĵ�һ���ֽ� tmp[0] ����Ч�ֽ� */
        spi_flash_transmit_receive(jedec_device_cmd, tmp, COUNTOF(jedec_device_cmd));      
        Id.maunfacture = tmp[1];;
        Id.memory = tmp[2];;
        Id.compatibility = tmp[3];
        Id.jedec_device = ((uint32_t)tmp[1] << 16) | ((uint32_t)tmp[2] << 8) | tmp[3];
    }
    else if (id_select == 1)    /**< ��ȡ����ID */
    {
        spi_flash_transmit(device_cmd, COUNTOF(device_cmd));
        Id.device = spi_flash_read_onebyte();
    }
    else if (id_select == 2)    /**< ��ȡ���� & ���� ID */
    {
        spi_flash_transmit(manufacture_device_cmd, COUNTOF(manufacture_device_cmd));
        Id.maunfacture = spi_flash_read_onebyte();
        Id.device = spi_flash_read_onebyte();
    }
    else
    {
        ;
    }
    
    w25x16_set_cs_pin_value(GPIO_PIN_SET);
    w25x16_wait();
    
    printf("�ڴ�: 0x%X\r\n", Id.memory);
    printf("������: 0x%X\r\n", Id.compatibility);
    printf("������: 0x%X\r\n", Id.maunfacture);
    printf("�豸: 0x%X\r\n", Id.device);
    printf("jedec device: 0x%X\r\n", Id.jedec_device);
    
    return Id;
}
