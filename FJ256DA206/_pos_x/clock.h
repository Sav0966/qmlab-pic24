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
/*
*
*/
typedef struct tagSYSTIME {
	long time;  // system time
	int pph; // hundredth part
} SYSTIME, *PSYSTIME;

long sys_time(PSYSTIME ptim);

#endif /*_CLOCK_INCL_*/
