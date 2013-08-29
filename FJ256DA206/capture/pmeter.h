/*
*	Period meter with Input Capture module
*/
#ifndef _PMETER_INCL_
#define _PMETER_INCL_

#include <caps.h>
#include <eds.h>

// Use this definition to declare period meter buffer
#define PM_BUFFER(n, size) __eds__ int _IC_(n, buf)[size]\
			__attribute__((page, space(eds), noload))
#define PM_BUF(n, i) _IC_(n, buf)[i]

#define DECL_PMETER_UI(n)\
	extern volatile PEINT _IC_(n, pcur) __attribute__((near));\
	extern volatile int *_IC_(n, pend) __attribute__((near));\
	extern volatile int _IC_(n, err) __attribute__((near))

#define PM_GET_PAGE(n)	(_IC_(n, pcur).p.page)

#define PM_IS_OBUF(n)	(_IC_(n, pcur).p.addr >= _IC_(n, pend))
#define PM_IS_OERR(n)	(_IC_(n, err)) /* Overrun IC FIFO */

#define PM_INIT(n, rxi, sync, ipl)\
	if (!IC_IS_INIT(n)) /* Init PM with IC @ FCY2 clock */\
		IC_INIT(n, ICT_FCY2|ICM_DISABLE|(rxi), sync, ipl);\
	_IC_(n, err) = 0 /* Clear Error */

#define PM_TMAX		(65536 / FCY2) /* 4,096 ms @ 16 MHz */

#define PM_IS_INIT(n)	IC_IS_INIT(n)
#define PM_DONE(n)		IC_PWOFF(n)

#define PM_START(n, size, mode)\
	_IC_(n, pcur).peds = _IC_(n, buf);\
	/* buf[0] - buf[size-2], last cell is not used */\
	_IC_(n, pend) =_IC_(n, pcur).p.addr + ((size) - 1);\
	IC_ENABLE(n, mode) /* Enable IC with icm = mode */

#define PM_STOP(n)		IC_RESET(n)

#define PM_PAUSE(n)			IC_DISABLE(n)
#define PM_RESUME(n, mode)	IC_ENABLE(n, mode)

#endif //_PMETER_INCL_
