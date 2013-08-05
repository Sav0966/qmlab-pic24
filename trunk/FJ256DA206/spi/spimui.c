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
//		"	.equiv	SISEL2, 4	\n" // Interrupt mode bit 2
		"	.equiv	SRXMPT, 5	\n" // RX FIFO is empty
		"	.equiv	SFIFO,	8	"); // Size of FIFO

#if (SPI_MASTER == 1) // SPI registers & CS
__asm__("	.equiv	_SPIBUF, _SPI1BUF	\n"
		"	.equiv	_SPISTAT, _SPI1STAT	");
#ifndef	_SPI1_CS // No ~CS pin attached
#define SPI_SET_CS(n)	((void)0)
#define SPI_CLR_CS(n)	((void)0)
#endif // No ~CS pin attached

#elif (SPI_MASTER == 2)
__asm__("	.equiv	_SPIBUF, _SPI2BUF	\n"
		"	.equiv	_SPISTAT, _SPI2STAT	");
#ifndef	_SPI2_CS // No ~CS pin attached
#define SPI_SET_CS(n)	((void)0)
#define SPI_CLR_CS(n)	((void)0)
#endif // No ~CS pin attached

#elif (SPI_MASTER == 3)
__asm__("	.equiv	_SPIBUF, _SPI3BUF	\n"
		"	.equiv	_SPISTAT, _SPI3STAT	");
#ifndef	_SPI3_CS // No ~CS pin attached
#define SPI_SET_CS(n)	((void)0)
#define SPI_CLR_CS(n)	((void)0)
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
*	0,0% over and 88% free time @ 1MHz SPI (4K)
*	0,0% over and 78% free time @ 2MHz SPI (4K)
*	1,1% over and 53% free time @ 4MHz SPI (4K)
*	6,5% over and 33% free time @ 8MHz SPI (4K)
*/
void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	__asm__ volatile(
	"	bclr	_SPISTAT, #SISEL1		\n"
	"	bset	_SPISTAT, #SISEL0		\n"
	); // Set TXI_END (or RXI_ANY) mode

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
		__asm__ volatile("_nzero_:		\n" // do {
		"	sub		W3, W2, W0			\n" //  if ((pTX-pRX)
		"	sub		W0, #SFIFO, W0		\n" //  >= FIFO_SIZE)
		"	bra		ge, _full_			\n" //   goto _full_
		"	ze		[W3++], W0			\n" //
		"	mov		W0, _SPIBUF			\n" //  WRITE(*pTX++)
		"	dec		W1, W1				\n" //  --len
		"	bra		nz, _nzero_			\n" // } while (len)
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

		//_full_: ----- READ - WRITE LOOP -- (len != 0) ---
		__asm__ volatile("_full_:		\n" // do {
		"	btsc	_SPISTAT, #SRXMPT	\n" //  if (!CAN_RD)
		"	bra		_chck_				\n" //   goto _chck_
		"	mov		_SPIBUF, W0			\n" //
		"	mov.b	W0, [W2++]			\n" //  *pRX++=READ()
		"	ze		[W3++], W0			\n" //
		"	mov		W0, _SPIBUF			\n" //  WRITE(*pTX++)
		"	dec		W1, W1				\n" //  --len
		"	bra		nz, _full_			\n" // } while (len)
		"	bra		_zero_				\n" // goto _zero_
		::: "w0", "w1", "w2", "w3"); //__ 9 clk in loop ___

		__asm__ volatile("_chck_:		\n" //_chck_: -----
		"	disi	#4					\n" // DISABLE_INT()
		"	btsc	_SPISTAT, #SPITBF	\n" // if (TX FIFO
		"	bra		$+6					\n" //     is full)
		"	bclr	_SPISTAT, #SISEL0	\n" // { SISEL =
		"	bset	_SPISTAT, #SISEL1	\n" //   TXI_EMPTY }
		); // Try to set TXI_EMPTY (or RXI_6DATA) mode

	__asm__ volatile("_ret_:\n"		// _ret_: -------------
	"	mov 	W1, _lbuf		\n" // Restore lbuf
	"	mov 	W2, _pRXbuf		\n" // Restore pRXbuf
	"	mov 	W3, _pTXbuf		\n" // Restore pTXbuf
	::: "w1", "w2", "w3" );
}

IMPL_SPIM_SEND(SPI_MASTER)
{
	if (lbuf != 0) return(-1); // Busy
	while (!SPI_SR_EMPTY(SPI_MASTER));

	if (len) {
		SPI_DISABLE_INT(SPI_MASTER);
		pTXbuf = buf; pRXbuf = buf; lbuf = len;
		SPI_SELECT(SPI_MASTER);	// Select device
		SPI_SET_FLAG(SPI_MASTER); // and run SPI
		SPI_ENABLE_INT(SPI_MASTER); }

	return(len);
}

#endif // SPI_MASTER
