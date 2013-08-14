#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>

#include "caps.h"

#define IC_USED	9

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

static int stage = 0; // Test stage

void IC_INTFUNC(IC_USED, no_auto_psv)(void)
{
	IC_CLR_FLAG(IC_USED);

}

void cap_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!IC_IS_INIT(IC_USED)) {
			IC_INIT(IC_USED,
				ICT_FCY2 | ICM_DISABLE, 0, SYSCLK_IPL+1);

			stage = 0; // Start test
		}
	}

	if (!IC_IS_INIT(IC_USED)) return;

	switch(stage) {
		case 0:

			++stage; break; // Next test

		case 1:

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: stage = 0;
			IC_PWOFF(IC_USED);
			break;
	} // switch(stage)
}
