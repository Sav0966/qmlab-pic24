/*
*	System clock functions (based on Timer1)
*/
#include <config.h>
#include <timers.h>
#include <clock.h>

#define CLK_TIM		10E-3 /* Дискрет вемени системы в сек */
#define CORR_TIM	16 /* Раз в 16 сек коррекция остатка */

#define CORR_CLK ((unsigned long)(CORR_TIM / CLK_TIM))
#define PR1_PERT(t) ((unsigned long)((t * (FCY2 / 8))))
#define PR1_START ((unsigned int)(PR1_PERT(CLK_TIM))-1)
#define PR1_CORR ((unsigned int)\
(FCY /* *(CORR_TIM/16) */ - CORR_CLK * PR1_PERT(CLK_TIM)))

#ifndef SYSCLK_IPL
 #error SYSCLK_IPL is not defined in "config.h"
#endif

static long sys_time; // System time and
static int sys_pph; // its hundredth part
static int sys_clock; // System clock

void __attribute__((__interrupt__, /* shadow, */ no_auto_psv))
_T1Interrupt(void)
{
	TIMER_SET_PR(1, PR1_START); // Restore period register

	++sys_clock;

	if (++sys_pph == 100)
	{
		sys_pph = 0;
		if (!(++sys_time & 0x000F))
		{	// Once per 16 seccond:
			// Doing correction of period register
			__asm__ volatile ("mov #%0, W0\n add PR1"
					:/* no outputs */: "i" (PR1_CORR));

			if (!REFOCONbits.ROON) // If REFO is not used
				__asm__ volatile (/* Mark pulse on REFO */\
				"	btg LATB, #15 ; Toggle latch REFO\n"\
				"	btg LATB, #15 ; Toggle latch REFO\n");
		}
	}

	TIMER_CLR_INTFLAG(1);
}

void clock_done(void) { TIMER_PWOFF(1); }

int clock_init(long time)
{
	clock_done(); // Disable all
	sys_time = time; sys_pph = 0; // Set time, pph = 0

	// Init Timer1 (Fcy/16 = 2 MHz) whith 10 ms period
	TIMER_INIT(1, T_MODE_INT|T_PS_8, PR1_START, SYSCLK_IPL);
	TIMER_ON(1); // Run Timer1

	return 0;
}
