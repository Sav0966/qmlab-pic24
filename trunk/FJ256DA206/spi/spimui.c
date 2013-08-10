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
		"	.equiv	_cnt, __spim_1_cnt	\n"
		"	.equiv	_len, __spim_1_len	\n"
		"	.equiv	_pRX, __spim_1_pRX	\n"
		"	.equiv	_pTX, __spim_1_pTX	");
#elif (SPI_MASTER == 2)
__asm__("	.equiv	_BUF, _SPI2BUF		\n"
		"	.equiv	_STAT, _SPI2STAT	\n"
		"	.equiv	_cnt, __spim_2_cnt	\n"
		"	.equiv	_len, __spim_2_len	\n"
		"	.equiv	_pRX, __spim_2_pRX	\n"
		"	.equiv	_pTX, __spim_2_pTX	");
#elif (SPI_MASTER == 3)
__asm__("	.equiv	_BUF, _SPI3BUF		\n"
		"	.equiv	_STAT, _SPI3STAT	\n"
		"	.equiv	_cnt, __spim_3_cnt	\n"
		"	.equiv	_len, __spim_3_len	\n"
		"	.equiv	_pRX, __spim_3_pRX	\n"
		"	.equiv	_pTX, __spim_3_pTX	");
#endif // Registers and vars

volatile int _SPIM_(SPI_MASTER, err);
volatile char* _SPIM_(SPI_MASTER, pRX) __attribute__((near));
volatile char* _SPIM_(SPI_MASTER, pTX) __attribute__((near));
volatile int _SPIM_(SPI_MASTER, len) __attribute__((near));
volatile int _SPIM_(SPI_MASTER, cnt) __attribute__((near));

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
*	87% 1MHz, 77% @ 2MHz, 58% @ 4MHz, 28% @ 8MHz
*/
void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	__asm__ volatile( // Set TXI_END
	"	bclr	_STAT, #SISEL1		\n"
	"	bset	_STAT, #SISEL0		\n"
	"	bset	_STAT, #SISEL2		\n"
	); SPI_CLR_FLAG(SPI_MASTER); // Clear IF

	__asm__ volatile(
	"	cp0		_len				\n" // if (len==0)
	"	bra		z, _zero_			\n" // goto _zero_
//	"	bra		lt, _dummy_\n" // if (len < 0) _dummy_
	);

		__asm__ volatile( //------- WRITE LOOP ----------|
		"_nzero_:						\n" // len != 0  |
		"	mov		_pRX,W0				\n" // *pRX++=READ
		"	mov 	_BUF, W1			\n" // Must read to
		"	mov.b 	W1, [W0++]			\n" //  prevent ROV
		"	mov		W0, _pRX			\n" // Restore pRX

		"	mov		_pTX, W1			\n" // Store pTX
		"	sub		W0, W1, W0			\n" // cnt =
		"	add		W0, #(SFIFO+1), W0	\n" //  (pRX - pTX)
		"	mov		W0, _cnt			\n" //  + (SFIFO+1)

		"	bra		$+10			\n" // while
		"	ze		[w1++], W0		\n" // (--cnt > 0) {
		"	mov		W0, _BUF		\n" //   WRITE(*pTX++)
		"	dec		_len			\n" //   if (--len == 0)
		"	bra		z, _read_		\n" //      goto _read_
		"	dec		_cnt			\n" // }
		"	bra		gt, $-10		\n" //__ 7n + 4 clk _|

//		"	inc		_cnt			\n" // _cnt always > 0
		::: "w0", "w1");

		__asm__ volatile(//-------- READ - WRITE LOOP ---|
		// If can read - write, len != 0, pRX = pTX-SFIFO
		"	bra		$+14			\n" // while (CAN_RD)
		"	mov		_BUF, W0		\n" // {
		"	mov.b	W0, [W1-SFIFO]	\n" //   *pRX++=READ()
		"	mov.b	[W1++], W0		\n" //   WRITE(*pTX++)
		"	mov		W0, _BUF		\n" //
		"	dec		_len			\n" //   if (--len == 0)
		"	bra		z, _break_		\n" //     goto _break_
		"	btss	_STAT, #SRXMPT	\n" // }
		"	bra		$-14			\n" //__ 9n + 4 clk _|
		::: "w0", "w1");

		__asm__ volatile(// Try to set RXI_6DATA mode
		"_chck_:						\n" //_chck_:
		"	disi	#(_ch_- $)/2-1		\n" // INTERLOCKED
		"	btsc	_STAT, #SPITBF		\n" // ( if (TX FIFO
		"	bra		_break_				\n" //    is full) {
		"	bclr	_STAT, #SISEL2		\n" //    SIEL =
		"	bclr	_STAT, #SISEL0		\n" //    RXI_6DATA
		"	bset	_STAT, #SISEL1		\n" //    Clear IF }
		::: "w0", "w1"); SPI_CLR_FLAG(SPI_MASTER); // )
		__asm__ volatile("_ch_:");

			__asm__ volatile( //========= RETURN ========|
			"_break_:						\n" //_break_:
			"	sub		W1, #SFIFO, W0		\n" // W0 = pRX
			"	mov		W1, _pTX			\n" // Restore
			"	mov		W0, _pRX			\n" // pTX, pRX
			"	pop.d	W0	\n	retfie		\n" // return()
			::: "w0", "w1"); //========== RETURN ========|

		//================== DUMMY READ =================|
		//::: "w0", "w1"); //===== DUMMY READ ===========|

	__asm__ volatile( //-------- READ LOOP ----------|
	"_read_:					\n" // (len == 0) ---|
	"	mov		W1, _pTX		\n" // Restore pTX
	"_zero_:					\n" //
	"	mov		_pRX, W1		\n" // Store pRX
	"	btsc	_STAT, #SRXMPT	\n" // while (CAN_RD)
	"	bra		_end_			\n" // {
	"	mov		_BUF, W0		\n" //    *pRX++ = 
	"	mov.b	W0, [W1++]		\n" //      READ()
	"	btss	_STAT, #SRXMPT	\n" // } goto _end_
	"	bra		$-6				\n" //__ 5n + 4 clk _|
	::: "w0", "w1");

		__asm__ volatile(
		"_end_:						\n" //
		"	mov		W1, _pRX		\n" // Restore pRX
		"	mov		_pTX, W0		\n" //
		"	sub		W0, W1, [W15]	\n" // if (pRX !=
		"	bra		nz, _ret_		\n" // pTX) return
		::: "w0", "w1");

		__asm__ volatile("_packet_end_: nop\n");

	__asm__ volatile("_ret_:\n");
}

IMPL_SPIM_SHIFT(SPI_MASTER)
// (char* buf, int rlen, int tlen)
{
	SPI_DISABLE_INT(SPI_MASTER);

	if (_SPIM_(SPI_MASTER, pRX) !=
		_SPIM_(SPI_MASTER, pTX)) // Busy
		_SPIM_(SPI_MASTER, len) = -1;
	else {
		SPI_DISABLE(SPI_MASTER); // Use SDO or IO
		if (tlen) SPICON1bits(SPI_MASTER).DISSDO = 0;
		else  SPICON1bits(SPI_MASTER).DISSDO = 1;
		SPI_ENABLE(SPI_MASTER);

		if (rlen) {
			_SPIM_(SPI_MASTER, pRX) = buf;
			_SPIM_(SPI_MASTER, pTX) = buf+1;
			_SPIM_(SPI_MASTER, len) = rlen-1;
			SPI_WRITE(SPI_MASTER, *buf); // Run
		}
	}

	SPI_ENABLE_INT(SPI_MASTER);
	return(rlen);
}

#endif // SPI_MASTER
