/*
*	System clock functions (based on Timer1)
*/
#include <config.h>
#include <timers.h>
#include <clock.h>

#if (!defined(SYSCLK_IPL) || !defined(SYS_TIMER))
#error "SYSCLK_IPL & SYS_TIMER must be defined in config.h"
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

static volatile SYSTIME _sys_time;	// System time and
static volatile int _sys_clock;		// System clock

int sys_clock(void) { return(_sys_clock); }

long sys_time(PSYSTIME ptim)
{
	SYSTIME tim;

	INTERLOCKED_MOD(TIMER, INT(SYS_TIMER), tim = _sys_time);

	if (ptim) *ptim = tim;
	return( tim.time );
}

void TIMER_INTFUNC(SYS_TIMER, no_auto_psv)(void)
{
	TIMER_CLR_FLAG(SYS_TIMER);
	// Restore period register
	TIMER_SET_PR(SYS_TIMER, PR_START);

	++_sys_clock;

	if (++_sys_time.pph == 100)
	{
		_sys_time.pph = 0;
		if (!(++_sys_time.time & 0x000F))
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

void clock_done(void) { TIMER_PWOFF(SYS_TIMER); }

int clock_init(long time)
{
	clock_done(); // Disable all
	_sys_time.time = time; _sys_time.pph = 0; // time
	// Init Timer (Fcy/16 = 2 MHz) whith 10 ms period
	TIMER_INIT(SYS_TIMER, T_MODE_INT|T_PS_8, PR_START, SYSCLK_IPL);
	TIMER_ON(SYS_TIMER); // Run Timer

	return 0;
}
