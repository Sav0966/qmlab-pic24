/*
*	SPI 1-3 user interface functions (master mode)
*/
#ifndef _SPIMUI_INCL_
#define _SPIMUI_INCL_

#include <spis.h>
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

#ifdef __not_compile__ // Standard mode is not used
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
#endif //__not_compile__

#endif /*_SPIMUI_INCL_*/
