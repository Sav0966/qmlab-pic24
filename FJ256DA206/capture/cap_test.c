#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <refo.h>


#include "pmeter.h"

#define BUF_SIZE	1024
PM_BUFFER(IC_USED, BUF_SIZE+1); // Buffer
DECL_PMETER_UI(IC_USED); // User interfase

static int i, stage = 0; // Test stage

void cap_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test period meter
		PM_INIT(IC_USED, IC_RXI_4DATA, 0, SYSCLK_IPL+1);
		PM_BUF(IC_USED, BUF_SIZE) = 0xABCD; // Check ptr
		stage = 0; // Start test
	}

	if (!PM_IS_INIT(IC_USED)) return;

	switch(stage) {
		case 0: // REF frequency

			IC_DISABLE(IC_USED);
			refo_div(RODIV_8192);
			refo_on(); // ~3096 Hz
			// (== 256 us period)

			++stage; break; // Next test

		case 1: // Run sampling

			PM_START(IC_USED, BUF_SIZE, ICM_RAISE);

			++stage; break; // Next test

		case 2: // Wait while buffer is not full

			if (!PM_IS_OBUF(IC_USED)) break;

			++stage; break; // Next test

		case 3: // + 1 event (overwrite buffer)
			++stage; break;

		case 4: // Stop and nalyse buffer

			ASSERT(!PM_IS_OERR(IC_USED));
			PM_STOP(IC_USED); // Stop sampling
			refo_off(); // Turn off REF output

			for (i = 1; i < BUF_SIZE-1; ++i)
				ASSERT(((unsigned)_IC_(IC_USED, buf)[i] - // = 256 us
						(unsigned)_IC_(IC_USED, buf)[i-1]) == 0x1000);

			// Last buffer data must be overwriten
			ASSERT(((unsigned)_IC_(IC_USED, buf)[i] - // i = BUF_SIZ-1
						(unsigned)_IC_(IC_USED, buf)[i-1]) != 0x1000);

			// Extra buffer data must be not changed
			ASSERT(PM_BUF(IC_USED, BUF_SIZE) == 0xABCD);

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default:
			PM_DONE(IC_USED);
			break;
	} // switch(stage)
}
