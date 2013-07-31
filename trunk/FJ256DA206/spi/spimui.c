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

#ifndef SPI_SET_CS
#define SPI_SET_CS()
#define SPI_CLR_CS()
#endif

volatile unsigned char *pSPIbuf;
volatile int len = 0;
volatile int err = 0;

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);
	++err; // Needs to decrease speed
}

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag
	__asm__ volatile(
	"	mov		#_SPI2BUF, W1	\n"
	"	mov		_pSPIbuf, W0	"::: "w0", "w1");

	__asm__ volatile("_loop:	\n"
	"	cp0		_len			\n"
	"	bra		z, _zero_len	\n"
	"	bra		le, _negative	"); // Select len

	__asm__ volatile("_positive:\n"
	// Called on
	"	mov		W0, _pSPIbuf	"::: "w0", "w1");
	__asm__ volatile("pop.d W0 \n retfie"); // return

	__asm__ volatile("_negative:\n"
	// Called on TXI_END event (transmit is complete)
//	"	mov.b	[W1], [W15]		\n" Overrun RX buffer
	"	mov.b	[W0++], [W1]	\n"
	"	inc		_len			\n"
	"	btss	_SPI2STAT, #1	\n"
	"	bra		_loop			\n"
	"	mov		W0, _pSPIbuf	"::: "w0", "w1");
	__asm__ volatile("pop.d W0 \n retfie"); // return

	__asm__ volatile(
	"_zero_len:				");
//	SPI_DISABLE_INT(SPI_MASTER);
//	SPI_CLR_ERFLAG(SPI_MASTER);
//	SPI_CLR_OERR(SPI_MASTER);

//	SPI_SET_CS(); // Unselect device
}

int spi_send(unsigned char* buf, int n)
{
	if (len > 0) n = -1; // Can't transmit
	else /* if (n > 0) */ {
		SPI_DISABLE_INT(SPI_MASTER);
		SPI_DISABLE_ERINT(SPI_MASTER);
		SPI_SET_RTXI(SPI_MASTER, S_TXI_EMPTY);

		pSPIbuf = buf; len = -n;

		SPI_SET_CS(); // Select device
		SPI_SET_FLAG(SPI_MASTER);
		SPI_ENABLE_INT(SPI_MASTER);
	}

	return n;
}

#ifdef __not_compil__
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
#endif //__not_compil__

#endif // SPI_MASTER
