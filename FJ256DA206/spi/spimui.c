/*
*	User interface functions for SPI1-3 (master mode)
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef SPI_MASTER  // Only for used SPI

#include <spimui.h>

// SPI Status register bits (PIC24F family)
__asm__("	.equiv	SPITBF, 1	\n" // TX FIFO is full
		"	.equiv	SISEL0, 2	\n" // Interrupt mode bit 0
		"	.equiv	SISEL1, 3	\n" // Interrupt mode bit 1
		"	.equiv	SISEL2, 4	\n" // Interrupt mode bit 2
		"	.equiv	SRXMPT, 5	\n" // RX FIFO is empty
		"	.equiv	SFIFO,	8	"); // Size of FIFO

#if (SPI_MASTER == 1) // SPI registers & CS
__asm__("	.equiv	_SPIBUF, _SPI1BUF	\n"
		"	.equiv	_STAT, _SPI1STAT	");
#ifndef	_SPI1_CS // No ~CS pin attached
volatile int _SPI1_CS __attribute__ ((near)) = 1;
#endif // No ~CS pin attached

#elif (SPI_MASTER == 2)
__asm__("	.equiv	_SPIBUF, _SPI2BUF	\n"
		"	.equiv	_STAT, _SPI2STAT	");
#ifndef	_SPI2_CS // No ~CS pin attached
volatile int _SPI2_CS __attribute__ ((near)) = 1;
#endif // No ~CS pin attached

#elif (SPI_MASTER == 3)
__asm__("	.equiv	_SPIBUF, _SPI3BUF	\n"
		"	.equiv	_STAT, _SPI3STAT	");
#ifndef	_SPI3_CS // No ~CS pin attached
volatile int _SPI3_CS __attribute__ ((near)) = 1;
#endif // No ~CS pin attached
#endif // SPI registers & CS

#ifndef SPI_SET_CS
#define SPI_SET_CS(n)	(_SPI_CS(n) = 1)
#define SPI_CLR_CS(n)	(_SPI_CS(n) = 0)
#endif
#define SPI_UNSELECT(n)	SPI_SET_CS(n)
#define SPI_SELECT(n)	SPI_CLR_CS(n)

volatile int _cnt __attribute__ ((near))/* = 0 */;
static volatile int _len __attribute__ ((near))/* = 0 */;
static volatile int err /* = 0 */;
static volatile char *pTX, *pRX;

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);
	SPI_DISABLE(SPI_MASTER); // Reset
	SPI_ENABLE(SPI_MASTER); // SPI FIFO
	_len = 0; SPI_UNSELECT(SPI_MASTER);
	++err; // Needs to decrease speed
}
/*
*	SPI ISR (Enhanced Master mode)
*
*	Zero over and 88% free time @ 1MHz SPI (4K)
*	Zero over and 78% free time @ 2MHz SPI (4K)
*	Zero over and 58% free time @ 4MHz SPI (4K)
*	Zero over and 27% free time @ 8MHz SPI (4K)
*/
void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	__asm__ volatile(
	"	mov		_pTX,W1	\n" // Store pTX

	"	bclr	_STAT, #SISEL1		\n"
	"	bset	_STAT, #SISEL0		\n"
	"	bset	_STAT, #SISEL2		\n"
	); // SISEL = TXI_END

	SPI_CLR_FLAG(SPI_MASTER); // Clear IF

	__asm__ volatile(
	"	cp0		__len				\n" // if (len==0)
	"	bra		z, _zero_			\n" // goto _zero_
	);

		__asm__ volatile( //------- WRITE LOOP ----------
		"_nzero_:					\n" // (len != 0) ---
		"	mov		_pRX,W0			\n" //  W1 == pTX ---
		"	sub		W0, W1, W0		\n"
		"	add		W0, #SFIFO, W0	\n" // cnt = SFIFO +
		"	mov		W0, __cnt		\n" //    (pRX - pTX)

		"	bra		$+10			\n" // while
		"	ze		[w1++], W0		\n" // (--cnt >= 0) {
		"	mov		W0, _SPIBUF		\n" //   WRITE(*pTX++)
		"	dec		__len			\n" //   if (--len == 0)
		"	bra		z, _zero_		\n" //      goto _zero_
		"	dec		__cnt			\n" // }
		"	bra		ge, $-10		\n" //__ 7n + 4 clk _

		// If can read - write, len != 0, pRX = pTX-SFIFO
		//------------------------- READ - WRITE LOOP ---
		"	btsc	_STAT, #SRXMPT	\n" // while (CAN_RD)
		"	bra		$+18			\n" // {
		"	mov		_SPIBUF, W0		\n" // 
		"	mov.b	W0, [W1-SFIFO]	\n" //   *pRX++=READ()
		"	mov.b	[W1++], W0		\n" //   WRITE(*pTX++)
		"	mov		W0, _SPIBUF		\n" //
		"	dec		__len			\n" //   if (--len == 0)
		"	bra		z, _break_		\n" //     goto _break_
		"	btss	_STAT, #SRXMPT	\n" // }
		"	bra		$-14			\n" //__ 9n + 4 clk _

		"_chck_:						\n" //_chck_: ---
		"	disi	#(_break_- $)/2-1	\n" // INTERLOCKED
		"	btsc	_STAT, #SPITBF		\n" // ( if (TX FIFO
		"	bra		_break_				\n" //    is full) {
		"	bclr	_STAT, #SISEL2		\n" //    SISEL =
		"	bclr	_STAT, #SISEL0		\n" //    RXI_6DATA
		"	bset	_STAT, #SISEL1		\n" //    Clear IF }
		::: "w0", "w1"); SPI_CLR_FLAG(SPI_MASTER); // )

			__asm__ volatile( //===========================
			"_break_:						\n" //_break_:
			"	sub		W1, #SFIFO, W0		\n" // W0 = pRX
			"	mov		W1, _pTX			\n" // Restore
			"	mov		W0, _pRX			\n" // pTX, pRX
			"	pop.d	W0	\n	retfie		\n" // return()
			::: "w0", "w1"); //========== RETURN ==========

	__asm__ volatile( //-------- READ LOOP ----------
	"_zero_:					\n" // (len == 0) ---
	"	mov		W1, _pTX		\n" // Restore pTX
	"	mov		_pRX, W1		\n" // Store pRX

	"	btsc	_STAT, #SRXMPT	\n" // while (CAN_RD)
	"	bra		$+10			\n" // {
	"	mov		_SPIBUF, W0		\n" //    *pRX++ = 
	"	mov.b	W0, [W1++]		\n" //      READ()
	"	btss	_STAT, #SRXMPT	\n" // }
	"	bra		$-6				\n" //__ 5n + 3 clk _
	"	mov		W1, _pRX		\n" //   Restore pRX

	"	mov		_pTX, W0		\n" //
	"	sub		W0, W1, [W15]	\n" // if (pRX != pTX)
	"	bra		nz, _ret_		\n" //        return()
	::: "w0", "w1");

//	//_end_: Can not read, len == 0
//	__asm__ volatile("_end_:	");		

	if (SPI_SR_EMPTY(SPI_MASTER)) {// Unselect
		// device at the end of transmition of
		SPI_UNSELECT(SPI_MASTER); // packet
	}

	__asm__ volatile("_ret_:\n");
}

IMPL_SPIM_SHIFT(SPI_MASTER)
{
	if (!SPI_READY(SPI_MASTER)) return(-1); // Busy

	if (len) {
		SPI_DISABLE_INT(SPI_MASTER);
		pTX = buf; pRX = buf; _len = len;
		SPI_SELECT(SPI_MASTER);	// Select device
		SPI_SET_FLAG(SPI_MASTER); // and run SPI
		SPI_ENABLE_INT(SPI_MASTER); }

	return(len);
}

#endif // SPI_MASTER
