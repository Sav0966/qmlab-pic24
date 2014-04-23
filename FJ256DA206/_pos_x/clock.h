/*
*	System clock functions (based on Timer1)
*/
#ifndef _CLOCK_INCL_
#define _CLOCK_INCL_
/*
* Module initialization
*/
int clock_init(long time);
void clock_done(void);
/*
* Return system clock (increased every 0.01 sec)
*/
int sys_clock(void);

#endif /*_CLOCK_INCL_*/
