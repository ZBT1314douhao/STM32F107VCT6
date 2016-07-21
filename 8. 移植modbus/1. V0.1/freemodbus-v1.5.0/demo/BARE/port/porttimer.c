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
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "tim.h"
#include "stm32f1xx_hal.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
//static 
    void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    /**< 定时器配置函数 实现50us时基时钟 cube已经配置好 */
//    MX_TIM7_Init();
    return TRUE;
//    return FALSE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	
	/**< Clears the specified TIM interrupt flag. */
//    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
	/**< Clear the TIM interrupt pending bits */
//    __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);  	
//    __HAL_TIM_SetCounter(&htim7, 0);
    /**< 只用下面这两个函数似乎就可以 */
	__HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE(&htim7);
    
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
//    __HAL_TIM_SetCounter(&htim7, 0);
//    __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);  
//    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
    /**< 只用下面这两个函数似乎就可以 */
    __HAL_TIM_DISABLE(&htim7);
	__HAL_TIM_DISABLE_IT(&htim7, TIM_IT_UPDATE);

}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
//static 
    void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

