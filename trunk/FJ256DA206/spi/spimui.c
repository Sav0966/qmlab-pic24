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

#if (SPI_MASTER == 1) // Registers and vars
__asm__("	.equiv	_BUF, _SPI1BUF		\n"
		"	.equiv	_STAT, _SPI1STAT	\n"
		"	.equiv	_cnt, _spim_1_cnt	\n"
		"	.equiv	_len, _spim_1_len	\n"
		"	.equiv	_pRX, _spim_1_pRX	\n"
		"	.equiv	_pTX, _spim_1_pTX	");
#elif (SPI_MASTER == 2)
__asm__("	.equiv	_BUF, _SPI2BUF		\n"
		"	.equiv	_STAT, _SPI2STAT	\n"
		"	.equiv	_cnt, _spim_2_cnt	\n"
		"	.equiv	_len, _spim_2_len	\n"
		"	.equiv	_pRX, _spim_2_pRX	\n"
		"	.equiv	_pTX, _spim_2_pTX	");
#elif (SPI_MASTER == 3)
__asm__("	.equiv	_BUF, _SPI3BUF		\n"
		"	.equiv	_STAT, _SPI3STAT	\n"
		"	.equiv	_cnt, _spim_3_cnt	\n"
		"	.equiv	_len, _spim_3_len	\n"
		"	.equiv	_pRX, _spim_3_pRX	\n"
		"	.equiv	_pTX, _spim_3_pTX	");
#endif // Registers and vars

volatile int _SPIM_(SPI_MASTER, err);
volatile char* _SPIM_(SPI_MASTER, pRX);
volatile char* _SPIM_(SPI_MASTER, pTX);
volatile int _SPIM_(SPI_MASTER, len) __attribute((near));
//volatile int _SPIM_(SPI_MASTER, cnt) __attribute((near));

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Needs to decrease speed
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);

//	SPI_DISABLE(SPI_MASTER); // Reset
//	SPI_ENABLE(SPI_MASTER); // SPI FIFO and buffer
//	_SPIM_(SPI_MASTER, pRX) = _SPIM_(SPI_MASTER, pTX);
//	_SPIM_(SPI_MASTER, len) = 0;
	
	++_SPIM_(SPI_MASTER, err);
}
/*
*	SPI ISR (Enhanced Master mode)
*
*	No overtime
*	Free CPU time spim_shift/_load(4K packet):
*	88% 1MHz, 77% @ 2MHz, 59% @ 4MHz, 28% @ 8MHz
*/
void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	__asm__ volatile(
	"	mov 	_pRX, W2	\n" // W2 = pRXbuf
	"	mov 	_pTX, W3	\n" // W3 = pTXbuf
	::: "w0", "w2", "w3" );

	__asm__ volatile( // Set TXI_END
	"	bclr	_STAT, #SISEL1		\n"
	"	bset	_STAT, #SISEL0		\n"
	"	bset	_STAT, #SISEL2		\n"
	); SPI_CLR_FLAG(SPI_MASTER); // Clear IF

	__asm__ volatile(
	"	cp0		_len			\n" // if (len==0)
	"	bra		z, _zero_		\n" // goto _zero_
	);

		//_nzero_: -----WRITE LOOP ---------- (len != 0) --
		__asm__ volatile(
		"	mov 	_BUF, W0			\n" // Must read to
		"	mov.b 	W0, [W2++]			\n" //  prevent ROV
		"	sub		W3, W2, W1			\n" //
		"	sub		W1, #SFIFO, W1		\n" // W1 = ((pTX-pRX)
		"	bra		_can_wr_			\n" //    - FIFO_SIZE)
		"_wr_:							\n" // while (W1 <= 0)
		"	mov.b	[W3++], W0			\n" // {
		"	mov		W0, _BUF			\n" //  WRITE(*pTX++)
		"	dec		_len				\n" //
		"	bra		z, _zero_			\n" //  if (--len !=0)
		"_can_wr_:						\n" //   goto _zero_
		"	inc		W1, W1				\n" // }
		"	bra		le, _wr_			\n" //_____ 7 clk
		::: "w0", "w1", "w2", "w3");

//__asm__ volatile("nop");

		//_rdwr_: ----- READ - WRITE LOOP -- (len != 0) ---
		// If can read, then can write. Try to fill TX FIFO
		__asm__ volatile("_rdwr_:		\n" // do {
		"	btsc	_STAT, #SRXMPT	\n" //  if (!CAN_RD)
		"	bra		_chck_				\n" //   goto _chck_
		"	mov		_BUF, W0			\n" //
		"	mov.b	W0, [W2++]			\n" //  *pRX++=READ()
		"	mov.b	[W3++], W0			\n" //
		"	mov		W0, _BUF			\n" //  WRITE(*pTX++)
		"	dec		_len				\n" //  --len
		"	bra		nz, _rdwr_			\n" // } while (len)
		"	bra		_ret_				\n" // return()
		::: "w0", "w2", "w3"); //__________________ 9 clk

		__asm__ volatile("_chck_:		\n" //_chck_: -----
		"	disi	#((_ch_-_chck_)/2-1)\n" // INTERLOCKED(
		"	btsc	_STAT, #SPITBF		\n" //  if (TX FIFO
		"	bra		_ret_				\n" //      is full)
		"	bclr	_STAT, #SISEL2		\n" //  { SISEL =
		"	bclr	_STAT, #SISEL0		\n" //    RXI_6DATA
		"	bset	_STAT, #SISEL1		\n" //    Clear IF }
		); 	SPI_CLR_FLAG(SPI_MASTER);	    // )/* Locked */
		__asm__ volatile("_ch_:	bra	_ret_");// return()

	//_zero_: --------- READ LOOP ---------- (len == 0) ---
	__asm__ volatile("_zero_:			\n" //
	"	bra		_can_rd_				\n" // while (CAN_RD)
	"_rd_:								\n" // {
	"	mov		_BUF, W0				\n" //
	"	mov.b	W0, [W2++]				\n" //  *pRX++=READ()
	"_can_rd_:							\n" //
	"	btss	_STAT, #SRXMPT			\n" // }
	"	bra		_rd_					\n" //_____ 6 clk
	::: "w0", "w2", "w3");

	__asm__ volatile(
	"	sub		W3, W2, [W15]	\n" // if (pRX !=
	"	bra		nz, _ret_		\n" // pTX) return
	::: "w0", "w2", "w3");

	__asm__ volatile("nop\nnop"); // pRX == pTX

	__asm__ volatile("_ret_:	\n"
	"	mov 	W3, _pTX		\n" // Restore pTXbuf
	"	mov 	W2, _pRX		\n" // Restore pRXbuf
	::: "w2", "w3");
}

IMPL_SPIM_SHIFT(SPI_MASTER)
{
	if (_SPIM_(SPI_MASTER, pRX) != // Busy
		_SPIM_(SPI_MASTER, pTX)) return(-1);

	SPI_DISABLE(SPI_MASTER);
	SPICON1bits(SPI_MASTER).DISSDO = 0;
	SPI_ENABLE(SPI_MASTER);

	if (len) {
		SPI_DISABLE_INT(SPI_MASTER);
		_SPIM_(SPI_MASTER, pRX) = buf;
		_SPIM_(SPI_MASTER, pTX) = buf+1;
		_SPIM_(SPI_MASTER, len) = len-1;
		SPI_ENABLE_INT(SPI_MASTER);

		SPI_WRITE(SPI_MASTER, *buf); // Run
	}

	return(len);
}

IMPL_SPIM_LOAD(SPI_MASTER)
{
	if (_SPIM_(SPI_MASTER, pRX) != // Busy
		_SPIM_(SPI_MASTER, pTX)) return(-1);

	SPI_DISABLE(SPI_MASTER);
	SPICON1bits(SPI_MASTER).DISSDO = 1;
	SPI_ENABLE(SPI_MASTER);

	if (len) {
		SPI_DISABLE_INT(SPI_MASTER);
		_SPIM_(SPI_MASTER, pRX) = buf;
		_SPIM_(SPI_MASTER, pTX) = buf+1;
		_SPIM_(SPI_MASTER, len) = len-1;
		SPI_ENABLE_INT(SPI_MASTER);

		SPI_WRITE(SPI_MASTER, *buf); // Run
	}

	return(len);
}

#endif // SPI_MASTER

#ifdef _not_compil_
	__asm__ volatile(
	"	mov		_pTX, W1	\n" // Store pTX

	"	bclr	_STAT, #SISEL1		\n"
	"	bset	_STAT, #SISEL0		\n"
	"	bset	_STAT, #SISEL2		\n"
	); // SISEL = TXI_END

	SPI_CLR_FLAG(SPI_MASTER); // Clear IF

	__asm__ volatile(
	"	cp0		_len				\n" // if (len==0)
	"	bra		z, _zero_			\n" // goto _zero_
//	"	bra		lt, _dummy \n"  // if (len < 0) _dummy
	);

		__asm__ volatile( //------- WRITE LOOP ----------
		"_nzero_:						\n" // len != 0
		"	mov		_pRX,W0				\n" // W1 == pTX
		"	sub		W0, W1, W0			\n" // cnt =
		"	add		W0, #SFIFO, W0	\n" //  (SFIFO+1) +
		"	mov		W0, _cnt			\n" //  (pRX - pTX)

		"	bra		$+10			\n" // while
		"	ze		[w1++], W0		\n" // (--cnt > 0) {
		"	mov		W0, _BUF		\n" //   WRITE(*pTX++)
		"	dec		_len			\n" //   if (--len == 0)
		"	bra		z, _zero_		\n" //      goto _zero_
		"	dec		_cnt			\n" // }
		"	bra		ge, $-10		\n" //__ 7n + 4 clk _

!!!		"	nop			\n"	// _cnt > 0

		// If can read - write, len != 0, pRX = pTX-SFIFO
		//------------------------- READ - WRITE LOOP ---
		"	btsc	_STAT, #SRXMPT	\n" // while (CAN_RD)
		"	bra		_chck_			\n" // {
		"	mov		_BUF, W0		\n" // 
		"	mov.b	W0, [W1-SFIFO]	\n" //   *pRX++=READ()
		"	mov.b	[W1++], W0		\n" //   WRITE(*pTX++)
		"	mov		W0, _BUF		\n" //
		"	dec		_len			\n" //   if (--len == 0)
		"	bra		nz, $-14		\n" //     goto _break_
//		"	btss	_STAT, #SRXMPT	\n" // }
//		"	sub		W1, #SFIFO, W0	\n" // W0 = pRX
//		"	mov		W0, _pRX		\n" // Restore pRX
!!!		"	bra		_break_			\n" //__ 9n + 4 clk _

		"_chck_:						\n" //_chck_: ---
		"	disi	#(_break_- $)/2-1	\n" // INTERLOCKED
		"	btsc	_STAT, #SPITBF		\n" // ( if (TX FIFO
		"	bra		_break_				\n" //    is full) {
		"	bclr	_STAT, #SISEL2		\n" //    SISEL =
		"	bclr	_STAT, #SISEL0		\n" //    RXI_6DATA
		"	bset	_STAT, #SISEL1		\n" //    Clear IF }
		::: "w0", "w1"); SPI_CLR_FLAG(SPI_MASTER); // )

			__asm__ volatile( //=========================
			"_break_:						\n" //_break_:
			"	sub		W1, #SFIFO, W0		\n" // W0 = pRX
			"	mov		W1, _pTX			\n" // Restore
			"	mov		W0, _pRX			\n" // pTX, pRX
			"	pop.d	W0	\n	retfie		\n" // return()
			::: "w0", "w1"); //========== RETURN ========

		//================== DUMMY READ =================
		//::: "w0", "w1"); //===== DUMMY READ =============

	__asm__ volatile( //-------- READ LOOP ----------
	"_zero_:					\n" // (len == 0) ---
	"	mov		W1, _pTX		\n" // Restore pTX
	"	mov		_pRX, W1		\n" // Store pRX

	"	btsc	_STAT, #SRXMPT	\n" // while (CAN_RD)
	"	bra		_end_			\n" // {
	"	mov		_BUF, W0		\n" //    *pRX++ = 
	"	mov.b	W0, [W1++]		\n" //      READ()
	"	btss	_STAT, #SRXMPT	\n" // } goto _end_
	"	bra		$-6				\n" //__ 5n + 3 clk _

		"_end_:						\n"
		"	mov		W1, _pRX		\n" // Restore pRX
		"	mov		_pTX, W0		\n" //
		"	sub		W0, W1, [W15]	\n" // if (pRX !=
		"	bra		nz, _ret_		\n" // pTX) return
		::: "w0", "w1");

	__asm__ volatile("_ret_:\n");
#endif
