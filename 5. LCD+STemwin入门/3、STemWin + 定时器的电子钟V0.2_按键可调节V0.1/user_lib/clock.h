/** 
 *  @file       clock.h
 *  @brief      µç×ÓÖÓ
 *  @author     ÍõÏþÈÙ   
 *  @version    
 *  @date       2014-08-29 
 */
 
#ifndef	_CLOCK_H
#define	_CLOCK_H 

enum  CLOCK_STATE 				 
{
	CLOCK_RUN, CLOCK_SEC_ADJ, CLOCK_MIN_ADJ, CLOCK_HOUR_ADJ,
}; 

void clock(void);
void clock_run(void);
void clock_sec_adj(void);
void clock_min_adj(void);
void clock_hour_adj(void);


#endif
