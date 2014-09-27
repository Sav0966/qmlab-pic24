/*
*	System clock functions (based on Timer1)
*/
#include <config.h>
#include <timers.h>
#include "systime.h"

#if (!defined(SYS_TIMER_IPL) || !defined(SYS_TIMER))
#error "SYS_TIMER_IPL & SYS_TIMER must be defined"
#endif

#define CLK_TIM		10E-3 /* Дискрет вемени системы в сек */
#define CORR_TIM	16 /* Раз в 16 сек коррекция остатка */

#define CORR_CLK ((unsigned long)(CORR_TIM / CLK_TIM))
#define PR_PERT(t) ((unsigned long)((t * (FCY2 / 8))))
#define PR_START ((unsigned int)(PR_PERT(CLK_TIM))-1)
#define PR_CORR ((unsigned int)\
(FCY /* *(CORR_TIM/16) */ - CORR_CLK * PR_PERT(CLK_TIM)))

#ifndef DISPATCH
#define DISPATCH()
#endif

static volatile SYSTIME _sys_time;	// System time
volatile int _sys_tick_;		// = 0 Tik counter

long systime_get(PSYSTIME ptim)
{
	SYSTIME tim;
	// Get system time
	INTERLOCKED(tim = _sys_time);

	if (ptim) *ptim = tim;
	return( tim.tim );
}

void systime_set(PSYSTIME ptim)
{
	DISABLE();
	_sys_time = *ptim; // Set system time
	ENABLE();
}

void TIMER_INTFUNC(SYS_TIMER, no_auto_psv)(void)
{
	TIMER_CLR_FLAG(SYS_TIMER);
	// Restore period register
	TIMER_SET_PR(SYS_TIMER, PR_START);

	++_sys_tick_;
	if (++_sys_time.pph == 100)
	{
		_sys_time.pph = 0;
		if (!(++_sys_time.tim & 0x000F))
		{	// Once per 16 seccond:
			// Doing correction of period register
			__asm__ volatile ("mov #%0, W0\n add PR1"
					:/* no outputs */: "i" (PR_CORR));

			if (!REFOCONbits.ROON) // If REFO is not used
				__asm__ volatile (/* Mark pulse on REFO */\
				"	btg LATB, #15 ; Toggle latch REFO\n"\
				"	btg LATB, #15 ; Toggle latch REFO\n");
		}
	}

	DISPATCH();
}

void systime_init(long tim)
{
	// Init Timer (Fcy/16 = 2 MHz) whith 10 ms period
	TIMER_INIT(SYS_TIMER,	// T_ON = 0 (Timer is off)
		T_MODE_INT | T_PS_8, PR_START, SYS_TIMER_IPL);

	_sys_time.tim = tim; _sys_time.pph = 0;
}
