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

#ifndef SPI_SELECT
#define SPI_SELECT()
#define SPI_UNSELECT()
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

#if (SPI_MASTER == 1) // SPI registers
__asm__("	.equiv	_SPIBUF, _SPI1BUF	\n"
		"	.equiv	_SPISTAT, _SPI1STAT	");
#elif (SPI_MASTER == 2)
__asm__("	.equiv	_SPIBUF, _SPI2BUF	\n"
		"	.equiv	_SPISTAT, _SPI2STAT	");
#elif (SPI_MASTER == 3)
__asm__("	.equiv	_SPIBUF, _SPI3BUF	\n"
		"	.equiv	_SPISTAT, _SPI3STAT	");
#endif

// SPI Status register bits
__asm__("	.equiv	SPITBF, 1	\n" // TX FIFO is full
		"	.equiv	SRXMPT, 5	"); // RX FIFO is empty

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_FLAG(SPI_MASTER); // Clear IF

	__asm__ volatile( // TXI_END or RXI_ANY
	"	bclr	_SPISTAT, #3			\n"
	"	bset	_SPISTAT, #2			");

	__asm__ volatile(	// Prepare W0, W1
	"	mov		#_SPIBUF, W1				\n"
	"	mov		_pSPIbuf, W0	"::: "w0", "w1");

	__asm__ volatile(
	"	cp0		_len							\n"
	//_loop:
	"	_loop:	; Flags after cp0, dec or inc	\n"
	"	bra		z, _zero_len					\n"
	"	bra		le, _negative	"); // Select len

	//_positive:
	__asm__ volatile("_positive:		");
	__asm__ volatile(
	"	mov		W0, _pSPIbuf		"::: "w0", "w1");
	__asm__ volatile("pop.d W0 \n retfie"); // return
	//_positive______________________________________

	//_negative: Called on TXI_END or TXI_READY
	__asm__ volatile("_negative:	; W0 = pSPIbuf ");
	__asm__ volatile( // Overrun RX FIFO for a speed
	//"	mov.b	[W1], [W15]			\n" // Don't read
	"	mov.b	[W0++], [W1]		\n" // WRITE(*p++)
	"	inc		_len				\n" // ++len
	"	btss	_SPISTAT, #SPITBF	\n" // if (CAN_WR)
	"	bra		_loop				\n" //  goto _loop
	"	bclr	_SPISTAT, #2	\n" // FIFO is full -
	"	bset	_SPISTAT, #3	\n" //  set TXI_READY
	"	mov		W0, _pSPIbuf		"::: "w0", "w1");
	__asm__ volatile("pop.d W0 \n retfie"); // return
	// 0,25% overrun and 64% free time @ 4MHz SPI ___
	// 2,35% overrun and 34% free time @ 8MHz SPI ___
	//_negative_____________________ 8 clk in loop __

	//_zero_len:
	__asm__ volatile("_zero_len:	");
	if (SPI_SR_EMPTY(SPI_MASTER)) {
		SPI_DISABLE_INT(SPI_MASTER);
		SPI_CLR_ERFLAG(SPI_MASTER);
		SPI_CLR_OERR(SPI_MASTER);
		SPI_UNSELECT(); }
}

int spi_send(unsigned char* buf, int n)
{
	if (len > 0) n = -1; // Can't transmit
	else /* if (n > 0) */ {
		SPI_DISABLE_INT(SPI_MASTER);
		SPI_DISABLE_ERINT(SPI_MASTER);
		SPI_SET_RTXI(SPI_MASTER, S_TXI_END);

		pSPIbuf = buf; len = -n;

		SPI_SELECT(); // Select device
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
