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
 *  @brief  功能不详
 *  @param  None
 *  @return None
 */
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
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

/**
 *  @brief  功能不详
 *  @param  None
 *  @return None
 */
eMBErrorCode    eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    static uint8_t flag = 0;
    
    (flag == 0)? led_on(LED_1): led_off(LED_1);
    flag ^= 1;

    return eStatus;
}

/**
 *  @brief  功能不详
 *  @param  None
 *  @return None
 */
eMBErrorCode    eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNCoils, eMBRegisterMode eMode)
{
    static uint8_t flag = 0;

    (flag == 0)? led_on(LED_2): led_off(LED_2);
    flag ^= 1;

    return MB_ENOREG;

}

/**
 *  @brief  功能不详
 *  @param  None
 *  @return None
 */
eMBErrorCode    eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNDiscrete)
{
    static uint8_t flag = 0;

    (flag == 0)? led_on(LED_3): led_off(LED_3);
    flag ^= 1;

    return MB_ENOREG;
}



