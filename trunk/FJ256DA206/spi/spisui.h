/*
*	SPI 1-3 user interface functions (slave mode)
*/
#ifndef _SPISUI_INCL_
#define _SPISUI_INCL_

#include <spis.h>
/*
*	Initialize the SPI module for the Standard Slave mode of operation
*/
#define SPI_SLAVE_INIT(n, con, sta, ipl) {\
	SPI_DISABLE_INT(n); SPI_DISABLE_ERINT(n);\
	_SPIMD(n) = 0; /* Power on SPI module */\
\
	SPISTAT(n) = (sta) & ~SPI_EN; /* Setup RTXI and disable module */\
	SPI_WRITE(n, 0);				/* Clear the SPIBUF register */\
	SPICON1(n) = (((con)&S_CKE)? ((con)|S_SSEN): con) & ~S_MSTEN;\
	SPICON2(n) = 0; /* Slave mode, if CKE is set, set SSEN too  */\
	SPI_CLR_SMP(n); SPI_CLR_OERR(n); /* Clear the SMP and error */\
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
*	Initialize the SPI module for the Enhanced Slave mode of operation
*/
#define SPI_ESLAVE_INIT(n, con, sta, ipl) {\
	SPI_DISABLE_INT(n); SPI_DISABLE_ERINT(n);\
	_SPIMD(n) = 0; /* Power on SPI module */\
\
	SPISTAT(n) = (sta) & ~SPI_EN; /* Setup RTXI and disable module */\
	SPI_WRITE(n, 0);				/* Clear the SPIBUF register */\
	SPICON1(n) = (((con)&S_CKE)? ((con)|S_SSEN): con) & ~S_MSTEN;\
	SPICON2(n) = 0; /* Slave mode, if CKE is set, set SSEN too  */\
	SPI_CLR_SMP(n); SPI_CLR_OERR(n); /* Clear the SMP and error */\
	SPI_BUF_ENABLE(n); /* Enable FIFO buffer */\
\
	SPI_CLR_FLAG(n); SPI_CLR_ERFLAG(n); /* Clear interrupt flags */\
\
	if (ipl > 0) { /* Enable interrupts, if used */\
		SPI_SET_IPL(n, ipl); SPI_SET_ERIPL(n, ipl);\
		SPI_ENABLE_INT(n); SPI_ENABLE_ERINT(n);\
	}\
\
	if ((sta) & SPI_EN) SPI_ENABLE(n);\
} ((void)0)

#endif /*_SPISUI_INCL_*/
