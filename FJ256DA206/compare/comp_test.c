#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>

#include "comp.h"

#define OC_USED		9

#define START_PWM(period) _OC1MD = 0;\
	OC1CON1 = 0; /* It is a good practice to clear */\
	OC1CON2 = 0; /* off the control bits initially */\
	OC1CON1bits.OCTSEL = 0x07; /* This selects the */\
	/* peripheral clock as the clock input to the OC module */\
	OC1R = period/2; /* This is just a typical number, user */\
	/* must calculate based on the waveform requirements and */\
	/* the system clock */\
	OC1RS = period; /* Determines the Period */\
	OC1CON1bits.OCM = 6; /* This selects and */\
	/* starts the Edge Aligned PWM mode */ ((void)0)

static int stage = 0; // Test stage

void comp_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
//		if (!IC_IS_INIT(IC_USED)) {
//			IC_INIT(IC_USED,
//				ICT_FCY2 | ICM_RAISE, 0, SYSCLK_IPL+1);

//			stage = 0; // Start test
//		}
	}

//	if (!IC_IS_INIT(IC_USED)) return;

	switch(stage) {
		case 0:
//			START_PWM(5000);

			++stage; break; // Next test

		case 1:

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: stage = 0;
//			OC_PWOFF(OC_USED);
			break;
	} // switch(stage)
}
