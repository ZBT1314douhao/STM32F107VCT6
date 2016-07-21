/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesse    r General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "gpio.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
//static void prvvUARTTxReadyISR( void );
//static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    ENTER_CRITICAL_SECTION();
    if (xRxEnable)
    {
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    }
    else
    {
         __HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);
    }
    
    if (xTxEnable)
    {
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
    }
    else
    {
         __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);
    }
    EXIT_CRITICAL_SECTION();
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL    bInitialized = TRUE;
    UART_InitTypeDef    USART_InitStruct;
    
    MX_USART1_UART_Init();
    MX_GPIO_Init();
    
    USART_InitStruct.BaudRate = ulBaudRate;
    switch ( eParity )
	{
	case MB_PAR_NONE:
		USART_InitStruct.Parity = UART_PARITY_NONE;
		break;
	case MB_PAR_ODD:
		USART_InitStruct.Parity = UART_PARITY_ODD;
		break;
	case MB_PAR_EVEN:
		USART_InitStruct.Parity = UART_PARITY_EVEN;
		break;
	}
	switch ( ucDataBits )
	{
	case 8:
		if(eParity==MB_PAR_NONE)
		USART_InitStruct.WordLength = UART_WORDLENGTH_8B;
		else
		USART_InitStruct.WordLength = UART_WORDLENGTH_9B;
		break;
	case 7:
		break;
	default:
		bInitialized = FALSE;
	}
	if( bInitialized )
	{
		ENTER_CRITICAL_SECTION(  );
		USART_InitStruct.StopBits = UART_STOPBITS_1;
		USART_InitStruct.HwFlowCtl = UART_HWCONTROL_NONE;
		USART_InitStruct.Mode = UART_MODE_TX_RX;
        HAL_UART_Init(&huart1);
		EXIT_CRITICAL_SECTION(  );
	}
	return bInitialized;
//    return TRUE;
//    return FALSE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    if (HAL_UART_Transmit(&huart1, (uint8_t *)&ucByte, 1, 0x01) != HAL_OK)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
//    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    if (HAL_UART_Receive(&huart1, (uint8_t *)&pucByte, 1, 0x01) != HAL_OK)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
//    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
//static 
    void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
//static 
    void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
