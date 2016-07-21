/**
  *
  * @file    portreg.c 
  * @author  Z B T
  * @version V0.1
  * @date    2016.05.22
  * @brief   
  *
  */  

#include "mb.h"
#include "led.h"

#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

/**
*  @brief  为了实现demo中的自加而构造的函数
 *  @param  None
 *  @retval None
 */
void reg_input_buff_inc(void)
{
    usRegInputBuf[0]++;
}

/**
 *  @brief  读数字寄存器  功能码0x04
 *  @param  None
 *  @retval None
 */
eMBErrorCode    eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNRegs) 
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

//eMBErrorCode    eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
//	USHORT usNRegs)
//{
//    eMBErrorCode    eStatus = MB_ENOERR;
//    static uint8_t flag = 0;

//    (flag == 0)? led_on(LED_0): led_off(LED_0);
//    flag ^= 1;

//    return eStatus;
//}

/**
 *  @brief  寄存器读写函数 读 0x03 写0x06
 *  @param  None
 *  @retval None
 */
eMBErrorCode    eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
//    static uint8_t flag = 0;
//    
//    (flag == 0)? led_on(LED_1): led_off(LED_1);
//    flag ^= 1;

    return eStatus;
}

/**
 *  @brief  读写开关寄存器 0x01 0x05
 *  @param  None
 *  @retval None
 */
eMBErrorCode    eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNCoils, eMBRegisterMode eMode)
{
//    static uint8_t flag = 0;

//    (flag == 0)? led_on(LED_2): led_off(LED_2);
//    flag ^= 1;

    return MB_ENOREG;

}

/**
 *  @brief  读开关寄存器 0x02
 *  @param  None
 *  @retval None
 */
eMBErrorCode    eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNDiscrete)
{
//    static uint8_t flag = 0;

//    (flag == 0)? led_on(LED_3): led_off(LED_3);
//    flag ^= 1;

    return MB_ENOREG;
}



