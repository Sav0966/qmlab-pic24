#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <refo.h>

#include "pmeter.h"

DECL_PMETER_UI(IC_USED);

static int i, stage = 0; // Test stage

void cap_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!IC_IS_INIT(IC_USED)) {
			IC_INIT(IC_USED, ICT_FCY2 | ICM_DISABLE,
					0, SYSCLK_IPL+1);

			stage = 0; // Start test
		}
	}

	if (!IC_IS_INIT(IC_USED)) return;

	switch(stage) {
		case 0: // REF frequency

			IC_DISABLE(IC_USED);
			refo_div(RODIV_8192);
			refo_on(); // ~3096 Hz
			// (== 256 us period)

			++stage; break; // Next test

		case 1: // Run sampling

			INIT_BUF(BUF_SIZE);
			IC_ENABLE(IC_USED, ICM_RAISE);

			++stage; break; // Next test

		case 2: // Wait while buffer is not full

			if (!IS_BUFERR()) break;

			++stage; break; // Next test

		case 3: // + 1 event (overwrite buffer)
			++stage; break;

		case 4: // Stop and nalyse buffer

			ASSERT(!IS_OERR()); // Not overrun FIFO
			IC_RESET(IC_USED); // Stop sampling
			refo_off(); // Turn off REF output

			for (i = 1; i < BUF_SIZE-1; ++i)
				ASSERT(((unsigned)buf[i] - // = 256 us
						(unsigned)buf[i-1]) == 0x1000);

			// Last buffer data must be overwriten
			ASSERT(((unsigned)buf[i] - // i = BUF_SIZ-1
						(unsigned)buf[i-1]) != 0x1000);

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: stage = 0;
			IC_PWOFF(IC_USED);
			break;
	} // switch(stage)
}
