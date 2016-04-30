/** 
 *  @file       clock.h
 *  @brief      µç×ÓÖÓ
 *  @author     zbt   
 *  @version    
 *  @date       206-04 
 */
 
#ifndef	_CLOCK_H
#define	_CLOCK_H 

#include "stm32f1xx_hal.h"

enum  CLOCK_STATE 				 
{
	CLOCK_RUN, CLOCK_SEC_ADJ, CLOCK_MIN_ADJ, CLOCK_HOUR_ADJ,
}; 

struct CLOCK_VALUE
{
    uint8_t     sec;
    uint8_t     min;
    uint8_t     hour;
};

void clock(void);
void clock_run(void);
void clock_sec_adj(void);
void clock_min_adj(void);
void clock_hour_adj(void);

uint8_t get_clock_sec(void);
uint8_t get_clock_min(void);
uint8_t get_clock_hour(void);

struct CLOCK_VALUE get_clock_value(void);


#endif
