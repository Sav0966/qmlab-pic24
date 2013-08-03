/*
*	User interface functions for SPI1-3 (master mode)
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef SPI_MASTER  // Only for used SPI

#include <spimui.h>

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

// SPI Status register bits (PIC24F family)
__asm__("	.equiv	SPITBF, 1	\n" // TX FIFO is full
		"	.equiv	SISEL0, 2	\n" // Interrupt mode bit
		"	.equiv	SISEL1, 3	\n" // Interrupt mode bit
		"	.equiv	SISEL2, 4	\n" // Interrupt mode bit
		"	.equiv	SRXMPT, 5	"); // RX FIFO is empty

#ifndef SPI_SELECT
#define __SPI_CS(n)	SPI##n##_CS
#define _SPI_CS(n)	__SPI_CS(n)

volatile int _SPI_CS(SPI_MASTER)
			__attribute__((near)) = 1;

#define SPI_UNSELECT(n)		(_SPI_CS(n) = 1)
#define SPI_SELECT(n)		(_SPI_CS(n) = 0)
#define SPI_READY(n)		(_SPI_CS(n))
#endif // SPI_SELECT

static volatile int len __attribute__((near)) /* = 0 */;
static volatile int err __attribute__((near)) /* = 0 */;
volatile unsigned char *pTXbuf, *pRXbuf;

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);
	++err; // Needs to decrease speed
}

//	__asm__ volatile( // TXI_END or RXI_ANY
//	"	bclr	_SPISTAT, #SISEL1		\n"
//	"	bset	_SPISTAT, #SISEL0		\n");

//	"	btss	_SPISTAT, #SRXMPT	\n" // if (CAN_RD)
//	"	bra		_rloop				\n" // goto _rloop

//	"	bclr	_SPISTAT, #SISEL0	\n" // FIFO is full
//	"	bset	_SPISTAT, #SISEL1	\n" //  - RXI_6DATA

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_FLAG(SPI_MASTER); // Clear IF

	__asm__ volatile(
//	"	bset	_SPISTAT, #SISEL2	\n" // = TXI_END
	"	btsc	_SPISTAT, #SRXMPT	\n" // if (!CAN_RD)
	"	bra		_write				\n" // goto _write
	);

	__asm__ volatile(
	"	mov 	_pRXbuf, W0			\n" // W0 = pRXbuf
	 //_________________________________ Read loop __
	"	_rloop:	\n"  // [W0++] = SPI_READ8(SPI_MASTER)
	"	mov		_SPIBUF, W1							\n"
	"	mov.b	W1, [W0++]							\n"
	"	btss	_SPISTAT, #SRXMPT	\n" // if (CAN_RD)
	"	bra		_rloop				\n" // goto _rloop
	"	mov		W0, _pRXbuf		 \n" // Restore pRXbuf
	::: "w0", "w1"); //______________ 5 clk in loop __

	__asm__ volatile("_write:		\n" //______write:
	"	mov 	_pTXbuf, W0			\n" // W0 = pTXbuf
	"	cp0		_len				\n" // if (len==0)
	"	bra	z, _zero				\n" //  goto _zero
	//SPI_WRITE(SPI_MASTER, [W0++]) ____ Write loop __
	"	ze		[W0++], W1							\n"
	"	mov		W1, _SPIBUF							\n"
	"	dec		_len				\n" // --len
	"	bra	z, _zero				\n" //  goto _zero

	"	btsc	_SPISTAT, #SPITBF	\n" // if (!CAN_WR)
	"	bra		_zero				\n" //  goto _zero

	//SPI_WRITE(SPI_MASTER, [W0++]) ____ Write loop __
	"	ze		[W0++], W1							\n"
	"	mov		W1, _SPIBUF							\n"
	"	dec		_len				\n" // --len

	::: "w0", "w1"); // _____________ 7 clk in loop __

	__asm__ volatile("_zero:\n"				//_zero:
	"	mov		W0, _pTXbuf		 \n" // Restore pTXbuf
	::: "w0");

	if (SPI_SR_EMPTY(SPI_MASTER))
		SPI_UNSELECT(SPI_MASTER);
}

int spi_msend(unsigned char* buf, int n)
{
	if (len != 0) return(-1); // Busy
	while (!SPI_SR_EMPTY(SPI_MASTER));

	SPI_DISABLE_INT(SPI_MASTER);

	SPI_DISABLE(SPI_MASTER); // Reset
	SPI_ENABLE(SPI_MASTER); // SPI FIFO
	SPI_SET_RTXI(SPI_MASTER, S_TXI_END);
	SPI_SET_FLAG(SPI_MASTER);

	pTXbuf = buf; pRXbuf = buf; len = n;
	SPI_SELECT(SPI_MASTER);	// Select device
	SPI_ENABLE_INT(SPI_MASTER); // and run

	return(n);
}

#endif // SPI_MASTER

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

#ifndef SPI_DUMMY
#define SPI_DUMMY	0
#endif // Dummy byte

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_ANY event (ready to read)
	if (len) // If can read, then can write
	{ SPI_WRITE(SPI_MASTER, SPI_DUMMY); --len; }

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
	*pRX++ = SPI_READ8(SPI_MASTER); // read byte
} // 40,7% free time @ 4MHz SPI
#endif //__not_compil__
