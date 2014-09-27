/*
*	System clock functions
*/
#ifndef _SYSTIME_INCL_
#define _SYSTIME_INCL_

typedef struct tagSYSTIME {
	long tim;  // system time
	int pph; // hundredth part
} SYSTIME, *PSYSTIME;

void systime_init(long tim);

long systime_get(PSYSTIME ptim);
void systime_set(PSYSTIME ptim);

extern volatile int _sys_tick_;

#endif /*_SYSTIME_INCL_*/
