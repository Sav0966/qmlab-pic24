/*
*	Переидомер с внутрицикловой обработкой (POS-1)
*/
#ifndef _PMETER_INCL_
#define _PMERT_INCL_

#include <config.h>

#ifdef IC_USED // Only for used Cupture module

#include <pmeter.h>
#include <eds.h>

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

// Short names for this C module
#define buf		_IC_(IC_USED, buf)
#define pbuf	_IC_(IC_USED, pbuf)
#define pend	_IC_(IC_USED, pend)
#define err		_IC_(IC_USED, err)

__eds__ int buf[BUF_SIZE] // Sampling buffer
	__attribute__((page, space(eds), noload));

volatile PEINT pbuf __attribute__((near));
volatile int *pend __attribute__((near));
int err __attribute__((near)) /* = 0 */;

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

#endif // IC_USED
#endif //_PMETER_INCL_
