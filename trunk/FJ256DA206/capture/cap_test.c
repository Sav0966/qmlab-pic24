#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

static int stage = 0; // Test stage

void cap_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
//		if (!spim_isinit(SPI_MASTER)) {
//			spim_init(SPI_MASTER, SPI_MODE, SYSCLK_IPL+1);
//			stage = 0; // Start test
//		}
	}

//	if (!spim_isinit(SPI_MASTER)) return;

	switch(stage) {
		case 0:

			++stage; break; // Next test

		case 1:

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: stage = 0;
			break;
	} // switch(stage)
}
