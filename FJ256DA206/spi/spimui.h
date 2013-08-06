/*
*	SPI 1-3 user interface functions (master mode)
*/
#ifndef _SPIMUI_INCL_
#define _SPIMUI_INCL_

#include <spis.h>
/*
*	Definition of Select Device pin (if used)
*
* Use macros:
* #define _SPIx_CS  // Use this definition to aviod selection
* #define _SPIx_CS _LATpn // Define port p pin n as CS output
*/
#define __SPI_CS(n)		_SPI##n##_CS
#define _SPI_CS(n)		__SPI_CS(n)

#ifndef	_SPI1_CS
extern volatile int _SPI1_CS __attribute__ ((near));
#endif
#ifndef	_SPI2_CS
extern volatile int _SPI2_CS __attribute__ ((near));
#endif
#ifndef	_SPI3_CS
extern volatile int _SPI1_CS __attribute__ ((near));
#endif

#define SPI_READY(n)	(_SPI_CS(n) != 0)
/*
*	SPI master mode interface (n - SPI module number)
*/
#define _spim_init(n, mode, ipl) /* mode = SPICON1 */\
	SPI_EMASTER_INIT(n, mode, SPI_EN | S_TXI_END, ipl)

#define __SPIM_SHIFT(n)		spim_##n##_shift
#define _SPIM_SHIFT(n)		__SPIM_SHIFT(n)
#define IMPL_SPIM_SHIFT(n)	int _SPIM_SHIFT(n)(char* buf, int len)
#define DECL_SPIM_SHIFT(n)	extern IMPL_SPIM_SHIFT(n)

#define DECL_SPIM_UI(n)		DECL_SPIM_SHIFT(n)

#define spim_isinit(n)				SPI_IS_INIT(n)
#define spim_init(n, mode, ipl)		_spim_init(n, mode, ipl)
#define spim_pwoff(n)				SPI_PWOFF(n)
#define spim_done(n)				SPI_DONE(n)

#define spim_shift(n, buf, len)		_SPIM_SHIFT(n)(buf, len)
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

#endif /*_SPIMUI_INCL_*/
