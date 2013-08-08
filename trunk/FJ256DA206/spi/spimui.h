/*
*	SPI 1-3 user interface functions (master mode)
*/
#ifndef _SPIMUI_INCL_
#define _SPIMUI_INCL_

#include <spis.h>
/*
*	Initialize the SPI module for the Standard Master mode of operation
*/
#define SPI_MASTER_INIT(n, con, sta, ipl) {\
	SPI_DISABLE_INT(n); SPI_DISABLE_ERINT(n);\
	_SPIMD(n) = 0; /* Power on SPI module */\
\
	SPISTAT(n) = (sta) & ~SPI_EN; /* Setup RTXI and disable module */\
	SPICON1(n) = (con) | S_MSTEN; SPICON2(n) = 0; /* Master mode */\
	SPI_CLR_OERR(n); /* Clear overrun error */\
\
	SPI_CLR_FLAG(n); SPI_CLR_ERFLAG(n); /* Clear interrupt flags */	\
\
	if (ipl > 0) { /* Enable interrupts, if used */\
		SPI_SET_IPL(n, ipl); SPI_SET_ERIPL(n, ipl);\
		SPI_ENABLE_INT(n); SPI_ENABLE_ERINT(n);\
	}\
\
	if ((sta) & SPI_EN) SPI_ENABLE(n);\
} ((void)0)
/*
*	Initialize the SPI module for the Enhanced Master mode of operation
*/
#define SPI_EMASTER_INIT(n, con, sta, ipl) {\
	SPI_DISABLE_INT(n); SPI_DISABLE_ERINT(n);\
	_SPIMD(n) = 0; /* Power on SPI module */\
\
	SPISTAT(n) = (sta) & ~SPI_EN; /* Setup RTXI and disable module */\
	SPICON1(n) = (con) | S_MSTEN; SPICON2(n) = 0; /* Master mode */\
	SPI_CLR_OERR(n); SPI_BUF_ENABLE(n); /* Enable FIFO buffer */\
\
	SPI_CLR_FLAG(n); SPI_CLR_ERFLAG(n); /* Clear interrupt flags */	\
\
	if (ipl > 0) { /* Enable interrupts, if used */\
		SPI_SET_IPL(n, ipl); SPI_SET_ERIPL(n, ipl);\
		SPI_ENABLE_INT(n); SPI_ENABLE_ERINT(n);\
	}\
\
	if ((sta) & SPI_EN) SPI_ENABLE(n);\
} ((void)0)
/*
*	SPI master mode interface (n - SPI module number)
*/
#define __SPIM_(n, name)	spim_##n##_##name
#define _SPIM_(n, name)		__SPIM_(n, name)

#define _SPIM_SHIFT(n)		_SPIM_(n, shift)
#define IMPL_SPIM_SHIFT(n)	int _SPIM_SHIFT(n)(char* buf, int len)
#define DECL_SPIM_SHIFT(n)	extern IMPL_SPIM_SHIFT(n)

#define _spim_init(n, mode, ipl) /* mode = SPICON1 */\
	SPI_EMASTER_INIT(n, mode, SPI_EN | S_TXI_END, ipl)

#define DECL_SPIM_UI(n)\
extern volatile int _SPIM_(n, err);\
extern volatile char* _SPIM_(n, pRX);\
extern volatile char* _SPIM_(n, pTX);\
extern volatile int _SPIM_(n, len) __attribute((near));\
DECL_SPIM_SHIFT(n)

#define _SPIM_READY(n)	(_SPIM_(n, pRX) == _SPIM_(n, pTX))
#define _SPIM_ISERR(n)	(_SPIM_(n, err) != 0)

#define _SPIM_INIT(n, mode, ipl)\
	SPI_DISABLE_INT(n); SPI_DISABLE(n);\
	_SPIM_(n, pTX) = _SPIM_(n, pTX);\
	_SPIM_(n, len) = 0; _SPIM_(n, err) = 0;\
	SPI_EMASTER_INIT(n, mode, SPI_EN | S_TXI_END, ipl)

#define spim_done(n)				SPI_DONE(n)
#define spim_pwoff(n)				SPI_PWOFF(n)
#define spim_isinit(n)				SPI_IS_INIT(n)
#define spim_init(n, mode, ipl)		_SPIM_INIT(n, mode, ipl)

#define spim_ready(n)				_SPIM_READY(n)
#define spim_iserr(n)				_SPIM_ISERR(n)
#define spim_shift(n, buf, len)		_SPIM_SHIFT(n)(buf, len)

#endif /*_SPIMUI_INCL_*/
