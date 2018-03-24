/** 
 *  @file       key.c
 *  @brief      ������������
 *  @author     zbt 
 *  @version    V1.0
 *  @date       2016-08-21
 */
 
#include "key.h"
#include "led.h"
#include "delay.h"

#define NUM_KEYS    (sizeof(GPIO_Key) / sizeof(GPIO_Key[0]))

#define KEY_PRESSED                 0
#define KEY_RELEASED                1
#define KEY_HOLDING                 2

#define KEY_IS_PRESSED(id)         ((uint8_t)(3 * id + 1))
#define KEY_IS_HOLDING(id)         ((uint8_t)(3 * id + 2))
#define KEY_IS_RELEASED(id)        ((uint8_t)(3 * id + 3))

/** ����key�õ��Ķ˿ڡ����� */
const struct GPIO_KEY GPIO_Key[] = 
{
    GPIOA, GPIO_PIN_0,
    GPIOC, GPIO_PIN_13,
};

static struct KEY sKey[NUM_KEYS];
static struct KEY_FIFO sKeyBuffer;

static void key_read(uint8_t id);
static GPIO_PinState key_get_state(uint8_t key_id);

/**
 * @brief  ����FIFO���Գ���  
 * @param  None
 * @return None
 */
void key_fifo_test(void)
{   
    enum KEY_STATE eKeyState = NO_KEY;
    
    eKeyState = key_get_value_from_buffer();
    if (eKeyState != NO_KEY)
    {
        switch (eKeyState)
        {
        case LEFT_PRESSED:
            led_on(LED_1);
            printf("����1���£�����led0\r\n");
            break;
        case LEFT_RELEASED:
            led_off(LED_1);
            printf("����1̧��Ϩ��led0\r\n");
            break;
        case LEFT_HOLDING:
            led_off(LED_3);
            printf("����1���֣�����led2\r\n");
            break;
        case RIGHT_PRESSED:
            led_on(LED_0);
            printf("����0���£�����led1\r\n");
            break;
        case RIGHT_RELEASED:
            led_off(LED_0);
            printf("����0̧��Ϩ��led1\r\n");
            break;
        case RIGHT_HOLDING:
            led_on(LED_3);
            printf("����0���֣�����led3\r\n");
            break;
        default:
            break;
        }
       
    }
    
}

/**
 * @brief  ������ʼ����������GPIO������)    
 * @param  None
 * @return None
 */
void key_init(void)
{
	uint8_t i;
    
    /** ��ʼ������FIFO�ṹ�� */
    sKeyBuffer.read = 0;
    sKeyBuffer.write = 0;
    /** ��ʼ��KEY�ṹ��ĸ���Ա */
	for (i = 0; i < NUM_KEYS; i++)
	{
		sKey[i].state = (enum KEY_STATE)(NO_KEY);       /**< 0���£� 1 δ���� */                            
		printf("key init %d\r\n", sKey[i].state);
        sKey[i].pressed = KEY_RELEASED;
        sKey[i].debounce_count = 0;  
		sKey[i].holding_count = 0;
		sKey[i].holding_time = 0;
		sKey[i].repeat_speed = 0;        /**< ���������ٶ����� 0�� ������*/
		sKey[i].repeat_count = 0;        /**< ���������� */
	}
    
    /** ��������ĳ�������Ĳ��� */
    sKey[0].holding_time = 50;
    sKey[1].holding_time = 100;
}

/**
 * @brief  �ӻ�������ü�ֵ    
 * @param  None
 * @return key_value�� ��õļ�ֵ
 */
enum KEY_STATE key_get_value_from_buffer(void)
{
    enum KEY_STATE eKeyValue;
    
    if (sKeyBuffer.read == sKeyBuffer.write)
    {
        eKeyValue =  NO_KEY;      /**< ��ʾFIFO�޿��ð���ֵ */
    }
    else
    {
        if (sKeyBuffer.read >= KEY_FIFO_SIZE)
        {
            sKeyBuffer.read = 0;
        }
        eKeyValue = (enum KEY_STATE)sKeyBuffer.buffer[sKeyBuffer.read++];
    }
    
//    printf("��ȡ���Ļ�������ֵ�� %d\r\n", eKeyValue);
    return eKeyValue;
}

/**
 * @brief  �򻺳���д��һ����ֵ    
 * @param  key_value�� Ҫд��ļ�ֵ
 * @return None
 */
void key_put_value_to_buffer(enum KEY_STATE eKeyValue)
{
    if (sKeyBuffer.write >= KEY_FIFO_SIZE)
    {
        sKeyBuffer.write = 0;
    }
    sKeyBuffer.buffer[sKeyBuffer.write++] = eKeyValue;
    printf("д�뻺������ֵ�� %d\r\n", eKeyValue);
}

/**
 * @brief  ����ɨ�躯��    
 * @param  None
 * @return None
 * @note   ���жϺ����б������Եĵ���
 */
void key_scan(void)
{
    uint8_t i;
    
    for (i = 0; i < NUM_KEYS; i++)
    {
        key_read(i);
    }
}

/**
 *  @brief  ��ȡ����ֵ    
 *  @param  None
 *  @return enum KEY
 */
static void key_read(uint8_t id)
{     
    
    if (key_get_state(id) == KEY_PRESSED)
    {
        printf("key %d pressed\r\n", id);
        /** �������� */
        if (sKey[id].debounce_count < KEY_DEBOUNCE_TIME)
        {
            ++sKey[id].debounce_count;
            printf("debounce %d\r\n", sKey[id].debounce_count);
        }
        else
        {   /** �ڼ�⵽�����ڼ���������ͷ����Ͱ����ͷ� */
            if (sKey[id].holding_time > 0)
            {

                printf("key state %d\r\n",sKey[id].state);
                if (sKey[id].holding_count < sKey[id].holding_time)
                {
                    sKey[id].holding_count++;
                    printf("���ּ���ֵ %d\r\n", sKey[id].holding_count);
                    if(sKey[id].holding_count == KEY_PRESSED_TIME)
                    {
                        sKey[id].state = KEY_PRESSED;       /**< ��ʾ�̰� */
                        key_put_value_to_buffer((enum KEY_STATE)KEY_IS_PRESSED(id));
                        printf("���� %d �̰�\r\n", id);
                    }
//                    else 
                        if((sKey[id].holding_count >= sKey[id].holding_time))
                    {
                        sKey[id].state = KEY_HOLDING;       /**< ��ʾ���� */
                        key_put_value_to_buffer((enum KEY_STATE)KEY_IS_HOLDING(id));
                        printf("���� %d ����\r\n", id);
                    }
                }
            }
            else 
            {
                sKey[id].state = KEY_PRESSED;
                key_put_value_to_buffer((enum KEY_STATE)KEY_IS_PRESSED(id));
                printf("���� %d �����µȴ��ͷ�\r\n", id);
            }
        }     
    }
    else    /**< �����ͷ� */
    {
        if (sKey[id].debounce_count != 0)
        {
            if (sKey[id].debounce_count > KEY_DEBOUNCE_TIME)
            {
                sKey[id].debounce_count = KEY_DEBOUNCE_TIME;
            }
            sKey[id].debounce_count--;
            printf("�ͷ�����ʱ�� %d\r\n", sKey[id].debounce_count);
            if (sKey[id].debounce_count == 0)
            {
                sKey[id].state = KEY_RELEASED;
            }
        }
        if (sKey[id].state == KEY_RELEASED)
        {
            sKey[id].state = NO_KEY;
            key_put_value_to_buffer((enum KEY_STATE)KEY_IS_RELEASED(id));
            printf("���� %d �ͷ�\r\n", id);
        }
        sKey[id].holding_count = 0;
        sKey[id].repeat_count = 0;
    }
}

/**
 *  @brief  ��ⰴ���Ƿ񱻰���    
 *  @param  key_id�� Ҫ��ⰴ�����к�
 *  @return ��⵽�İ�����״̬
 */
static GPIO_PinState key_get_state(uint8_t id)
{    
    sKey[id].pressed = HAL_GPIO_ReadPin(GPIO_Key[id].port, GPIO_Key[id].pin);
    
    return (GPIO_PinState)sKey[id].pressed;
}
