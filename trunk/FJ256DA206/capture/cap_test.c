#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>

#include "caps.h"

#define IC_USED		9
#define BUF_SIZE	1024

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

#define DSR_PAGE(page)\
	{	register int _dsr_to_save = DSRPAG;\
		DSRPAG = page
		// Read data from 'page' in EDS memory
#define DSR_END() DSRPAG = _dsr_to_save; } ((void)0)

#define DSW_PAGE(page)\
	{	register int _dsw_to_save = DSWPAG;\
		DSWPAG = page
		// Write data to 'page' in EDS memory
#define DSW_END() DSWPAG = _dsw_to_save; } ((void)0)

#define EDS_PAGE(page) DSR_PAGE(page); DSW_PAGE(page)
	// Read-write data from 'page' in EDS memory
#define EDS_END() DSW_END(); DSR_END()

#define _EDS_PTR(type) union {\
	__eds__ typeof(type) *peds;\
	struct { typeof(type) *addr; int page; } p; }

typedef _EDS_PTR(char)	EDS_SPTR, *PEDS_SPTR;
typedef _EDS_PTR(long)	EDS_LPTR, *PEDS_LPTR;
typedef _EDS_PTR(int)	EDS_IPTR, *PEDS_IPTR;

#define ___IC_(n, name)		_ic_##n##_##name
#define __IC_(n, name)		ic_##n##_##name
#define _IC_(n, name)		___IC_(n, name)
#define IC_(n, name)		__IC_(n, name)

#define buf		_IC_(IC_USED, buf)
#define pbuf	_IC_(IC_USED, pbuf)
#define pend	_IC_(IC_USED, pend)
#define err		_IC_(IC_USED, err)

__eds__ int buf[BUF_SIZE] __attribute__((page, space(eds), noload));
/*volatile*/ EDS_IPTR pbuf __attribute__((near));
volatile int *pend __attribute__((near));
int err __attribute__((near)) /* = 0 */;

#define INIT_BUF() pbuf.peds = buf;\
	 pend = pbuf.p.addr + BUF_SIZE

#define IS_BUF_ERR	(pend == pbuf.p.addr)
#define IS_ERROR	(err || IS_BUF_ERR())

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

void IC_INTFUNC(IC_USED, no_auto_psv)(void)
{
	IC_CLR_FLAG(IC_USED);

	__asm__ volatile ("push _DSWPAG");
	DSWPAG = pbuf.p.page;

	if (IC_IS_OERR(IC_USED)) ++err;

	while (IC_CAN_READ(IC_USED)) {
		*pbuf.p.addr = IC_READ(IC_USED);
		if (pbuf.p.addr < pend) ++pbuf.p.addr;
	}

	__asm__ volatile ("pop _DSWPAG");
}

static int stage = 0; // Test stage

void cap_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!IC_IS_INIT(IC_USED)) {
			IC_INIT(IC_USED,
				ICT_FCY2 | ICM_RAISE, 0, SYSCLK_IPL+1);

			stage = 0; // Start test
		}
	}

	if (!IC_IS_INIT(IC_USED)) return;

	switch(stage) {
		case 0:
			INIT_BUF();
			START_PWM(5000);

			++stage; break; // Next test

		case 1:

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: stage = 0;
			IC_PWOFF(IC_USED);
			break;
	} // switch(stage)
}
