#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <refo.h>
#include <eds.h>

#include "caps.h"

#define IC_USED		9
#define BUF_SIZE	1024

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

// Short names for this module
#define buf		_IC_(IC_USED, buf)
#define pbuf	_IC_(IC_USED, pbuf)
#define pend	_IC_(IC_USED, pend)
#define err		_IC_(IC_USED, err)

__eds__ int buf[BUF_SIZE] __attribute__((page, space(eds), noload));
volatile PEINT pbuf __attribute__((near));
volatile int *pend __attribute__((near));
int err __attribute__((near)) /* = 0 */;

#define INIT_BUF() /* &buf[0] - &buf[BUF_SIZE-2] */\
pbuf.peds = buf; pend = pbuf.p.addr + (BUF_SIZE - 1)

#define IS_BUFERR()	(pend == pbuf.p.addr)
#define IS_OERR()	(err) /* Overrun FIFO */

void IC_INTFUNC(IC_USED, no_auto_psv)(void)
{
	IC_CLR_FLAG(IC_USED);

	__asm__ volatile ("push _DSWPAG");
	DSWPAG = pbuf.p.page;

	if (IC_IS_OERR(IC_USED)) ++err;

	while (IC_CAN_READ(IC_USED)) {
		*pbuf.p.addr = IC_READ(IC_USED);
		// Never overrun buffer boundaries
		if (pbuf.p.addr < pend) ++pbuf.p.addr;
	} // This loop clears OERR flag (if needed)

	__asm__ volatile ("pop _DSWPAG");
}

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
			refo_on(); // 3096 Hz
			// (256 us period)

			++stage; break; // Next test

		case 1: // Run sampling

			INIT_BUF();
			IC_ENABLE(IC_USED, ICM_RAISE);

			++stage; break; // Next test

		case 2: // Wait while buffer is not full

			if (!IS_BUFERR()) break;

			++stage; break; // Next test

		case 3: ++stage; break; // + 1 event

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
