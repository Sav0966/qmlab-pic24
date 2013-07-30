/*
*	User interface functions for SPI1-3 (master mode)
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef SPI_MASTER  // Only for used SPI

#include <spimui.h>
#include <_tools.h>

#ifndef SPI_DUMMY
#define SPI_DUMMY	0
#endif // Dummy byte

volatile unsigned const char *pTX;
volatile unsigned char *pRX;
volatile int len = 0;
volatile int err = 0;

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);

#ifdef __DEBUG
__asm__ volatile ("nop\nnop"); // breakpoint
#endif

	++err; // Needs to decrease speed
}

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on TXI_READY event (ready to write)

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag
	__asm__ volatile(
		"	mov		#_SPI2BUF, W1	\n"

		"	mov		_pRX, W0		\n"
//		"	btss	_SPI2STAT, #5	\n"
		"	mov.b	[W1], [W0++]	\n"
		"	mov		W0, _pRX		\n"

		"	mov		_pTX, W0		\n"

//		"_write:					\n"
		"	cp0		_len			\n"
		"	bra		z, _exit		\n"
		"	mov.b	[W0++], [W1]	\n"
		"	dec		_len			\n"
//		"	bra		_write			\n"

		"_exit:						\n"
		"	mov		W0, _pTX		"

//		"	mov		_pTX, W0		\n"
//		"_loop:						\n"
//		"	cp0		_len			\n"
//		"	bra		z, _exit		\n"
//		"	mov.b	[W1], [W0++]	\n"
//		"	mov.b	[W0], [W1]		\n"
//		"	dec		_len			\n"
//		"	btss	_SPI2STAT, #5	\n"
//		"	bra		_loop			\n"
		: : : "w0", "w1");
		
} // 27,9% free time @ 4MHz SPI

#ifdef _not_compil_
void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_ANY event (ready to read)
	if (len) // If can read, then can write
	{ SPI_WRITE(SPI_MASTER, *pTX++); --len; }

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
	*pRX++ = SPI_READ8(SPI_MASTER); // read byte
} // 27,9% free time @ 4MHz SPI

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_ANY event (ready to read)
	if (len) // If can read, then can write
	{ SPI_WRITE(SPI_MASTER, *pRX); --len; }

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
	*pRX++ = SPI_READ8(SPI_MASTER); // read byte
} // 32,6% free time @ 4MHz SPI

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_ANY event (ready to read)
	if (len) // If can read, then can write
	{ SPI_WRITE(SPI_MASTER, SPI_DUMMY); --len; }

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
	*pRX++ = SPI_READ8(SPI_MASTER); // read byte
} // 40,7% free time @ 4MHz SPI
#endif

int spi_send(unsigned char* buf, int n)
{
	if (len > 0) n = -1; // Can't transmit
	else {
		SPI_DISABLE_INT(SPI_MASTER);
//		SPI_SET_RTXI(SPI_MASTER, S_TXI_READY);

		pTX = pRX = buf; len = (n-8);
		if (n > 0) 
//			SPI_SET_FLAG(SPI_MASTER);
			SPI_WRITE(SPI_MASTER, *pTX++);

SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
		SPI_ENABLE_INT(SPI_MASTER);
	}

	return n;
}

#endif // SPI_MASTER
