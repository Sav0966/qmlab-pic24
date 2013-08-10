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
#define ___SPIM_(n, name)	_spim_##n##_##name
#define __SPIM_(n, name)	spim_##n##_##name
#define _SPIM_(n, name)		___SPIM_(n, name)
#define SPIM_(n, name)		__SPIM_(n, name)

#define _SPIM_SHIFT(n)		_SPIM_(n, shift)
#define IMPL_SPIM_SHIFT(n)	int _SPIM_SHIFT(n)(char* buf, int rlen, int tlen)
#define DECL_SPIM_SHIFT(n)	extern IMPL_SPIM_SHIFT(n)

#define SPIM_SEND(n)		SPIM_(n, send)
#define IMPL_SPIM_SEND(n)	int SPIM_SEND(n)(char* buf, int len)
#define DECL_SPIM_SEND(n)	extern IMPL_SPIM_SEND(n)

#define DECL_SPIM_UI(n)\
	extern volatile int _SPIM_(n, err);\
	extern volatile char* _SPIM_(n, pRX) __attribute__((near));;\
	extern volatile char* _SPIM_(n, pTX) __attribute__((near));;\
	extern volatile int _SPIM_(n, len) __attribute__((near));\
	DECL_SPIM_SHIFT(n); DECL_SPIM_SEND(n)

#define SPIM_READY(n)	(_SPIM_(n, pRX) == _SPIM_(n, pTX))
#define SPIM_GETERR(n)	(_SPIM_(n, err))

#define SPIM_INIT(n, mode, ipl)\
	SPI_DISABLE_INT(n); SPI_DISABLE(n);\
	_SPIM_(n, pTX) = _SPIM_(n, pTX);\
	_SPIM_(n, len) = 0; _SPIM_(n, err) = 0;\
	SPI_EMASTER_INIT(n, mode, SPI_EN | S_TXI_END, ipl)

#define spim_done(n)				SPI_DONE(n)
#define spim_pwoff(n)				SPI_PWOFF(n)
#define spim_isinit(n)				SPI_IS_INIT(n)
#define spim_init(n, mode, ipl)		SPIM_INIT(n, mode, ipl)

#define spim_ready(n)				SPIM_READY(n)
#define spim_iserr(n)				SPIM_GETERR(n)
#define spim_shift(n, buf, len)		_SPIM_SHIFT(n)(buf, len, len)
#define spim_load(n, buf, rlen)		_SPIM_SHIFT(n)(buf, rlen, 0)
#define spim_send(n, buf, tlen)		_SPIM_SHIFT(n)(buf, 0, tlen)

#endif /*_SPIMUI_INCL_*/
