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
		"	.equiv	_SPISTAT, _SPI1STAT	");
#ifndef	_SPI1_CS // No ~CS pin attached
volatile int _SPI1_CS __attribute__ ((near)) = 1;
#endif // No ~CS pin attached

#elif (SPI_MASTER == 2)
__asm__("	.equiv	_SPIBUF, _SPI2BUF	\n"
		"	.equiv	_SPISTAT, _SPI2STAT	");
#ifndef	_SPI2_CS // No ~CS pin attached
volatile int _SPI2_CS __attribute__ ((near)) = 1;
#endif // No ~CS pin attached

#elif (SPI_MASTER == 3)
__asm__("	.equiv	_SPIBUF, _SPI3BUF	\n"
		"	.equiv	_SPISTAT, _SPI3STAT	");
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

static volatile int err /* = 0 */;
static volatile int lbuf /* = 0 */;
static volatile char *pTXbuf, *pRXbuf;

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);
	SPI_DISABLE(SPI_MASTER); // Reset
	SPI_ENABLE(SPI_MASTER); // SPI FIFO
	lbuf = 0; SPI_UNSELECT(SPI_MASTER);
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
	"	bclr	_SPISTAT, #SISEL1		\n"
	"	bset	_SPISTAT, #SISEL0		\n"
	"	bset	_SPISTAT, #SISEL2		\n"
	); // Set TXI_END interrupt mode

	__asm__ volatile(
	"	mov 	_lbuf, W1				\n" // W1 = lbuf
	"	mov 	_pRXbuf, W2				\n" // W2 = pRXbuf
	"	mov 	_pTXbuf, W3				\n" // W3 = pTXbuf
	::: "w1", "w2", "w3" );

	SPI_CLR_FLAG(SPI_MASTER); // Clear IF

	__asm__ volatile(
	"	cp0		W1						\n" // if (len==0)
	"	bra		z, _zero_		"::: "w1"); // goto _zero_

		//_nzero_: -----WRITE LOOP ---------- (len != 0) --
		__asm__ volatile("_wloop_:		\n" // do {
		"	sub		W3, W2, W0			\n" //  if ((pTX-pRX)
		"	sub		W0, #SFIFO, W0		\n" //  >= FIFO_SIZE)
		"	bra		ge, _fill_			\n" //   goto _fill_
		"	mov.b	[W3++], W0			\n" //
		"	mov		W0, _SPIBUF			\n" //  WRITE(*pTX++)
		"	dec		W1, W1				\n" //  --len
		"	bra		nz, _wloop_			\n" // } while (len)
		::: "w0", "w1", "w2", "w3"); //__ 8 clk in loop ___

	//_zero_: --------- READ LOOP ---------- (len == 0) ---
	__asm__ volatile("_zero_:			\n" // for(;;) {
	"	btsc	_SPISTAT, #SRXMPT		\n" //  if (!CAN_RD)
	"	bra		$+8						\n" //        break
	"	mov		_SPIBUF, W0				\n" //
	"	mov.b	W0, [W2++]				\n" //  *pRX++=READ()
	"	bra		_zero_					\n" // }
	::: "w0", "w2"); //__________________ 6 clk in loop ___

	//  Unselect device at the end of transmition of packet
	if (SPI_SR_EMPTY(SPI_MASTER)) SPI_UNSELECT(SPI_MASTER);

	__asm__ volatile("bra _ret_"); // return()

		//_fill_: ----- READ - WRITE LOOP -- (len != 0) ---
		// If can read, then can write. Try to fill TX FIFO
		__asm__ volatile("_fill_:		\n" // do {
		"	btsc	_SPISTAT, #SRXMPT	\n" //  if (!CAN_RD)
		"	bra		_chck_				\n" //   goto _chck_
		"	mov		_SPIBUF, W0			\n" //
		"	mov.b	W0, [W2++]			\n" //  *pRX++=READ()
		"	mov.b	[W3++], W0			\n" //
		"	mov		W0, _SPIBUF			\n" //  WRITE(*pTX++)
		"	dec		W1, W1				\n" //  --len
		"	bra		nz, _fill_			\n" // } while (len)
		"	bra		_zero_	; or _ret_	\n" // goto _zero_
		::: "w0", "w1", "w2", "w3"); //__ 9 clk in loop ___

		__asm__ volatile("_chck_:		\n" //_chck_: -----
		"	disi	#((_ret_- $)/2-1)	\n" // INTERLOCKED(
		"	btsc	_SPISTAT, #SPITBF	\n" //  if (TX FIFO
		"	bra		_ret_				\n" //      is full)
		"	bclr	_SPISTAT, #SISEL2	\n" //  { SISEL =
		"	bclr	_SPISTAT, #SISEL0	\n" //    RXI_6DATA
		"	bset	_SPISTAT, #SISEL1	\n" //    Clear IF }
		); 	SPI_CLR_FLAG(SPI_MASTER);	    // )/* Locked */

	__asm__ volatile("_ret_:\n"		// _ret_: -------------
	"	mov 	W3, _pTXbuf		\n" // Restore pTXbuf
	"	mov 	W2, _pRXbuf		\n" // Restore pRXbuf
	"	mov 	W1, _lbuf		\n" // Restore lbuf
	::: "w1", "w2", "w3" );
}

IMPL_SPIM_SHIFT(SPI_MASTER)
{
	if (!SPI_READY(SPI_MASTER)) return(-1); // Busy

	if (len) {
		SPI_DISABLE_INT(SPI_MASTER);
		pTXbuf = buf; pRXbuf = buf; lbuf = len;
		SPI_SELECT(SPI_MASTER);	// Select device
		SPI_SET_FLAG(SPI_MASTER); // and run SPI
		SPI_ENABLE_INT(SPI_MASTER); }

	return(len);
}

#endif // SPI_MASTER
