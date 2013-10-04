#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <refo.h>


#include "pmeter.h"

#define BUF_SIZE	3002
PM_BUFFER(IC_USED, BUF_SIZE+1); // +1 for checking
DECL_PMETER_UI(IC_USED); // Declare user interfase

static int i, tim, stage = 0; // Test stage

void cap_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if (!PM_IS_INIT(IC_USED)) {
		PM_INIT(IC_USED, IC_RXI_3DATA, 0, SYSCLK_IPL+1);
		PM_BUF(IC_USED, BUF_SIZE) = 0xABCD; // Check ptr
		stage = 0; // Start test
	}

	if (!PM_IS_INIT(IC_USED)) return;

	switch(stage) {
		case 0: // Start sampling

			refo_div(RODIV_8192);
			refo_on(); // == 256 us period on REFO

#ifndef __MPLAB_SIM  // Start IC (hardware only)
			PM_START(IC_USED, BUF_SIZE, ICM_RAISE);
			++stage; break; // Next test

#else
			{ // Fill buffer by software (SIM)
				static char __time__[] = __TIME__; // Rnd()

				_IC_(IC_USED, pcur).peds = _IC_(IC_USED, buf);
				_IC_(IC_USED, pend) =_IC_(IC_USED, pcur).p.addr +
											((BUF_SIZE) - 1);

				for (i = 0; __time__[i] != 0; ++i)
					PM_BUF(IC_USED, 0) += __time__[i];

				for (i = 1; i < BUF_SIZE-1; ++i) {
					PM_BUF(IC_USED, i) = PM_BUF(IC_USED, i-1)+0x1000; }
				// Last buffer data must be overwriten, i = BUF_SIZ-1
				PM_BUF(IC_USED, i) = PM_BUF(IC_USED, i-1) + 0x2000;

				_IC_(IC_USED, pcur).p.addr = _IC_(IC_USED, pend);
				stage = 3; break;
			}
#endif
		case 1: // Wait while buffer is not full

			if (!PM_IS_OBUF(IC_USED)) break;

			++stage; break; // Next test

		case 2: // + 1 event (overwrite last cell)
			++stage; break;

		case 3: // Stop and analyse buffer

			ASSERT(!PM_IS_OERR(IC_USED));
			PM_STOP(IC_USED); // Stop sampling
			refo_off(); // Turn off REF output

			for (i = 1; i < BUF_SIZE-1; ++i)
				ASSERT(((unsigned)PM_BUF(IC_USED, i) - // = 256 us
						(unsigned)PM_BUF(IC_USED, i-1)) == 0x1000);

			// Last buffer data must be overwriten
			ASSERT(((unsigned)PM_BUF(IC_USED, i) - // i = BUF_SIZ-1
						(unsigned)PM_BUF(IC_USED, i-1)) != 0x1000);

			// Extra buffer data must not be changed
			ASSERT(PM_BUF(IC_USED, BUF_SIZE) == 0xABCD);

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		case 4: // Methematics (statistical algorithm)

			pm_math_init(IC_USED);

			do { // Prepare statistics
				PROFILE_START(SYS_TIMER);
					pm_math23_task(IC_USED);
				PROFILE_END(SYS_TIMER, tim);
			} while PM_IS_RUN(IC_USED);
			// 3.31 us per one period

			__asm__ volatile ("nop\nnop");

			{ // Calculate average period (~T~)
				unsigned long long sum;
				unsigned long num;
				double period;

				// ~T~ = S / (2*_N1*_N1)
				PROFILE_START(SYS_TIMER);
					sum = pm_math23_sum(IC_USED);
					num = pm_math23_num(IC_USED); num *= num;
					if (num) period = (double)sum / num;
				PROFILE_END(SYS_TIMER, tim); // ~135 us

				ASSERT(period == (2.0 * 0x1000));

				__asm__ volatile ("nop\nnop");
			}

			++stage; break; // Next test

		default:
			PM_DONE(IC_USED);
			break;
	} // switch(stage)
}

// Check names
#undef IC_USED
#define IC_USED	1
//#include "pmeter.c"
