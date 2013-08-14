/*
*	SPI 1-3  definitions
*/
#ifndef _SPI_INCL_
#define _SPI_INCL_
/*
* SPI Transmit and Receive Buffer
*/
#define _SPIBUF(n)		SPI##n##BUF
#define SPIBUF(n)		_SPIBUF(n)

#define SPI_WRITE(n, i)		SPIBUF(n) = i
#define SPI_READ16(n)		((unsigned int)SPIBUF(n))
#define SPI_READ8(n)		((unsigned char)SPIBUF(n))
#define SPI_FIFO_SIZE		8 /* Enhanced Buffer mode */
/*
* SPI Control Register 1
*/
// SPI Control Register 1
#define _SPICON1(n)			SPI##n##CON1
#define _SPICON1bits(n)		SPI##n##CON1bits
#define SPICON1bits(n)		_SPICON1bits(n)
#define SPICON1(n)			_SPICON1(n)

#define S_MODE16	(1 << 10) // Word/Byte Communication Select bit

#define S_SMP		(1 << 9) // SPI Data Input Sample Phase bit
// Master mode: 1 = Input data sampled at the end of data output time
//              0 = Input data sampled at the middle of data output time
// Slave mode:  SMP must be cleared when SPIx is used in Slave mode.
#define SPI_CLR_SMP(n)		SPICON1bits(n).SMP = 0

#define S_CKE		(1 << 8) // SPI Clock Edge Select bit
// 1 = Serial output data changes on transition from Active to Idle clock state
// 0 = Serial output data changes on transition from Idle to Active clock state
// The user should program this bit to '0' for the Framed SPI modes

#define S_CKP		(1 << 6) // Clock Polarity Select bit
// 1 = Idle state for the clock is a high level; active state is a low level
// 0 = Idle state for the clock is a low level; active state is a high level

#define S_MSTEN		(1 << 5) // 1 = Master mode, 0 = Slave mode

// SS, SCK and SDO pins using:
#define S_SSEN	(1 << 7) // Slave Select Enable (Slave mode) bit
// If S_SSEN = 1, SS must be configured to an available RPn pin
#define S_DISSDO	(1 << 11) // Disable SDO Pin bit
#define S_DISSCK	(1 << 12) // Disable SCK Pin bit (SPI Master modes only)
// If enabled (=0), SDO or/and SCK must be configured to an available RPn pin

#define S_PPRE_64		0 // Primary Prescale bits (Master mode)
#define S_PPRE_16		1
#define S_PPRE_4		2
#define S_PPRE_NONE		3

#define S_SPRE_8		(0 << 2) //Secondary Prescale bits (Master mode)
#define S_SPRE_7		(1 << 2)
#define S_SPRE_6		(2 << 2)
#define S_SPRE_5		(3 << 2)
#define S_SPRE_4		(4 << 2)
#define S_SPRE_3		(5 << 2)
#define S_SPRE_2		(6 << 2)
#define S_SPRE_NONE		(7 << 2)

// Sample SCK Frequencies for FCY2 = 16 MHz
#define S_8000		(S_PPRE_NONE | S_SPRE_2)
#define S_4000		(S_PPRE_4 | S_SPRE_NONE)
#define S_2000		(S_PPRE_4 | S_SPRE_2)
#define S_1000		(S_PPRE_16 | S_SPRE_NONE)
#define S_500		(S_PPRE_16 | S_SPRE_2)
#define S_250		(S_PPRE_64 | S_SPRE_NONE)
#define S_125		(S_PPRE_64 | S_SPRE_2)
/*
* SPI Control Register 2
*/
// SPI Control Register 2
#define _SPICON2(n)			SPI##n##CON2
#define _SPICON2bits(n)		SPI##n##CON2bits
#define SPICON2bits(n)		_SPICON2bits(n)
#define SPICON2(n)			_SPICON2(n)

// Enhanced Buffer Enable bit (1 = enabled)
#define SPI_BUF_ENABLE(n)	SPICON2bits(n).SPIBEN = 1

#define S_FRMEN			(1 << 15) // Framed SPIx Support bit (1 = enabled)
// Frame Sync Pulse Direction Control on SSx Pin bit: 1 = SSx input (slave),
#define S_FSD			(1 << 14) // 0 = Frame sync pulse output (master)
// Frame Sync Pulse Polarity bit (Frame mode only): SSx is active-high,
#define S_FPOL			(1 << 13) // 0 = Frame sync pulse is active-low
// Frame Sync Pulse Edge Select bit: 1 = SSx coincides with the first bit clock
#define S_FE			(1 << 1) //  0 = SSx precedes the first bit clock
/*
* SPI Status and Control Register
*/
// SPI Status and Control Register
#define _SPISTAT(n)			SPI##n##STAT
#define _SPISTATbits(n)		SPI##n##STATbits
#define SPISTATbits(n)		_SPISTATbits(n)
#define SPISTAT(n)			_SPISTAT(n)

// SPI enable bit (these functions must be assigned to available
#define SPI_EN			(1 << 15)   // RPn/RPIn pins before use)
#define SPI_ENABLE(n)		SPISTATbits(n).SPIEN = 1
#define SPI_DISABLE(n)		SPISTATbits(n).SPIEN = 0
#define SPI_IS_ENABLE(n)	(SPISTATbits(n).SPIEN != 0)

#define S_SIDL			(1 << 13) // Stop operation in Idle mode

// Receive Overflow Flag bit
#define SPI_IS_OERR(n)		(SPISTATbits(n).SPIROV != 0)
#define SPI_CLR_OERR(n)		SPISTATbits(n).SPIROV = 0

// SPIx Buffer Element Count bits (valid in Enhanced Buffer mode)
// Master mode: Number of SPI transfers pending.
// Slave mode: Number of SPI transfers unread.
#define SPI_TX_COUNT(n)	((unsigned char)SPISTATbits(n).SPIBEC)

// Shift Register Empty bit (valid in Enhanced Buffer mode)
#define SPI_SR_EMPTY(n)		(SPISTATbits(n).SRMPT != 0)

// Receive FIFO Empty bit (valid in Enhanced Buffer mode)
#define SPI_RX_EMPTY(n)		(SPISTATbits(n).SRXMPT != 0)

// SPIx Buffer Interrupt Mode bits (valid in Enhanced Buffer mode):
// (def) Interrupt when the last data in the receive buffer is read; as a
#define S_RXI_EMPTY		0 // result, the buffer is empty (SRXMPT bit set)	
// Interrupt when data is available in the receive buffer (SRMPT bit is set)
#define S_RXI_ANY		(1 << 2)
// Interrupt when the SPIx receive buffer is 3/4 or more full
#define S_RXI_6DATA		(2 << 2)
// Interrupt when the SPIx receive buffer is full (SPIRBF bit set)
#define S_RXI_FULL		(3 << 2)
// Interrupt when one data is shifted into the SPIxSR; as a result,
#define S_TXI_READY		(4 << 2) // the TX FIFO has one open spot
// Interrupt when the last bit is shifted out of SPIxSR; now
#define S_TXI_END		(5 << 2) // the transmit is complete
// Interrupt when the last bit is shifted into SPIxSR; as a result,
#define S_TXI_EMPTY		(6 << 2) // the TX FIFO is empty
// Interrupt when the SPIx transmit buffer is full (SPITBF bit is set)
#define S_TXI_FULL		(7 << 2)

#define SPI_SET_RTXI(n, rtxi) /* Set new TXI or RXI value */\
	SPISTAT(n) = (SPISTAT(n) & ~(7 << 2)) | (rtxi & (7 << 2))

// SPIx Transmit Buffer Full Status bit:
// In Standard Buffer mode: Automatically set in hardware when the CPU writes
// to the SPIxBUF location, loading SPIxTXB. Automatically cleared in hardware
// when the SPIx module transfers data from SPIxTXB to SPIxSR.
// In Enhanced Buffer mode: Automatically set in hardware when the CPU writes
// to the SPIxBUF location, loading the last available buffer location.
// Automatically cleared in hardware when a buffer location is available
// for a CPU write.
#define SPI_CAN_WRITE(n)	(SPISTATbits(n).SPITBF == 0)

// SPIx Receive Buffer Full Status bit:
// In Standard Buffer mode: Automatically set in hardware when SPIx transfers
// data from SPIxSR to SPIxRXB. Automatically cleared in hardware when the core
// reads the SPIxBUF location, reading SPIxRXB.
// In Enhanced Buffer mode: Automatically set in hardware when SPIx transfers
// data from the SPIxSR to the buffer, filling the last unread buffer location.
// Automatically cleared in hardware when a buffer location is available for
// a transfer from SPIxSR.
#define SPI_RX_FULL(n)		(SPISTATbits(n).SPIRBF != 0)

// SPI_CAN_READ() is different in Standard mode and Enhanced Buffer mode
#define SPI_CAN_READ(n, SPIBEN)	(SPIBEN)? !SPI_RX_EMPTY(n): SPI_RX_FULL(n)
/*
* Interrupt management of SPI module (spi_rx/tx and spi_fault)
*/
// Interrupt Priority levels
#define _SPIIP(n) _SPI##n##IP
#define _SPFIP(n) _SPF##n##IP
// Setup and obtain SPI IPL values
#define SPI_SET_IPL(n, ipl)		_SPIIP(n) = ipl
#define SPI_GET_IPL(n)			((int)_SPIIP(n))
#define SPI_SET_ERIPL(n, ipl)	_SPFIP(n) = ipl
#define SPI_GET_ERIPL(n)		((int)_SPFIP(n))

// Interrupt Enable bits
#define _SPIIE(n) _SPI##n##IE
#define _SPFIE(n) _SPF##n##IE
// Enable and disable SPI interrupts
#define SPI_ENABLE_INT(n)		_SPIIE(n) = 1
#define SPI_DISABLE_INT(n)		_SPIIE(n) = 0
#define SPI_IS_ENABLE_INT(n)	(_SPIIE(n) == 1)
#define SPI_ENABLE_ERINT(n)		_SPFIE(n) = 1
#define SPI_DISABLE_ERINT(n)		_SPFIE(n) = 0
#define SPI_IS_ENABLE_ERINT(n)	(_SPFIE(n) == 1)

// Interrupt Status bits
#define _SPIIF(n) _SPI##n##IF
#define _SPFIF(n) _SPF##n##IF
// Clear, Set and Check Interrupt Status
#define SPI_CLR_FLAG(n)		_SPIIF(n) = 0
#define SPI_SET_FLAG(n)		_SPIIF(n) = 1
#define SPI_IS_FLAG(n)		(_SPIIF(n) != 0)
#define SPI_CLR_ERFLAG(n)	_SPFIF(n) = 0
#define SPI_SET_ERFLAG(n)	_SPFIF(n) = 1
#define SPI_IS_ERFLAG(n)	(_SPFIF(n) != 0)
/*
* Power management of SPI module (PMDx.SPInMD bit)
*/
#define __SPIMD(n)			_SPI##n##MD
#define _SPIMD(n)			__SPIMD(n)
/*
*	Template of SPI Interrupt Service Routines
*/
#define _SPI_INTFUNC(n, attr)\
__attribute__((__interrupt__, attr)) _SPI##n##Interrupt
#define SPI_INTFUNC(n, attr) _SPI_INTFUNC(n, attr)

#define _SPI_ERR_INTFUNC(n, attr)\
__attribute__((__interrupt__, attr)) _SPI##n##ErrInterrupt
#define SPI_ERR_INTFUNC(n, attr) _SPI_ERR_INTFUNC(n, attr)
/*
*	Common initialization functions
*/
#define SPI_IS_INIT(n) /* Powered & Enabled */\
	((_SPIMD(n) == 0) && SPI_IS_ENABLE(n))

#define SPI_DONE(n)\
	SPI_DISABLE_INT(n); SPI_DISABLE_ERINT(n);\
	SPI_DISABLE(n); /* Disable module and interrupts */\
	/* Clear all interrupt status flags (SPI and Err */\
	SPI_CLR_FLAG(n); SPI_CLR_ERFLAG(n)

#define SPI_PWOFF(n)\
	SPI_DONE(n); /* Disable SPI and interrupt */\
	_SPIMD(n) = 1 /* Power off SPI module */

#endif /*_SPI_INCL_*/
