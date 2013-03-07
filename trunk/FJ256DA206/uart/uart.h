/*
*	UART 1-4 definitions
*/
#ifndef _UART_INCL_
#define _UART_INCL_
/*
* UART Transmit and Receive Registers
*/
#define _UTXREG(n)	U##n##TXREG
#define _URXREG(n)	U##n##RXREG
#define UTXREG(n)	_UTXREG(n)
#define URXREG(n)	_URXREG(n)

#define UART_WRITE(n, ch)	UTXREG(n) = ch
#define UART_READ8(n)		((char)URXREG(n))
#define UART_READ9(n)		((int)URXREG(n))
#define UART_FIFO_SIZE		4
/*
* UART Baud Rate Generator Prescaler Register
*/
#define _UBRG(n)	U##n##BRG
#define UBRG(n)		_UBRG(n)
// Computation of the baud rate whith RBGH = 0
#define FCY2BRG(fcy, rate) ((int)((fcy/(16L*rate))-1))
// Computation of the baud rate whith RBGH = 1
#define FCY2BRGH(fcy, rate) ((int)((fcy/(4L*rate))-1))
/*
* UART Mode Register and appropriate mode settings
*/
// UART Mode Register and its bits
#define _UMODE(n)		U##n##MODE
#define _UMODEbits(n)	U##n##MODEbits
#define UMODEbits(n)	_UMODEbits(n)
#define UMODE(n)		_UMODE(n)
// If UART is disabled: all UART pins are controlled by
// 		port latches; UART power consumption is minimal
// If UART is enabled: all UARTx pins are controlled
// 		by UART as defined by UEN<1:0> (U_RTS_xxx)
#define U_EN			0x8000 // UART Enable bit

// Most usable modes of UART module
#define U_NOPARITY		0x0000 // (def) 8-bit data, no parity
#define U_EVEN			0x0002 // 8-bit data, even parity
#define U_ODD			0x0004 // 8-bit data, odd parity
#define U_9BIT			0x0006 // 9-bit data, no parity
#define U_1STOP			0x0000 // (def) One Stop bit
#define U_2STOP			0x0001 // Two Stop bits

// Loopback Mode Rx<->Tx, (RTS<->CTS, if used)
// LPBACK = 1 should be set only after enabling
// the other bits associated with the UART module
#define UART_SET_LPBACK(n)	UMODEbits(n).LPBACK = 1
#define UART_CLR_LPBACK(n)	UMODEbits(n).LPBACK = 0
#define UART_IS_LPBACK(n)	(UMODEbits(n).LPBACK == 1)

// Auto Baud Mode. Enable baud rate measurement on the
// next character – requires reception of a Sync field
// (55h); cleared in hardware upon completion
#define UART_SET_ABAUD(n)	UMODEbits(n).ABAUD = 1
#define UART_CLR_ABAUD(n)	UMODEbits(n).ABAUD = 0
#define UART_IS_ABAUD(n)	(UMODEbits(n).ABAUD == 1)

// High-Speed mode (4 BRG clock cycles per bit); x16 - default
#define U_BRGH			0x0008 // Not usable for IrDA support

#define U_IREN			0x1000 // IrDA Encoder and Decoder Enable bit

// RTS/BCLK and CTS pins configurations and control modes:
#define U_RTS_SIMPLEX	0x0800 // RTS pin is in Simplex mode
#define U_RTS_FLOW		0x0000 // (def) RTS pin is in Flow Control mode
// (def) TX and RX pins are enabled and used; CTS and RTS/BCLK pins
#define U_RTS_NONE		0x0000 // are controlled by port latches
// TX, RX and RTS pins are enabled and used; CTS pin is
#define U_RTS_ONLY		0x0100 // controlled by port latches
// TX, RX and BCLK pins are enabled and used; CTS pin is controlled by
#define U_RTS_BCLK		0x0300 // port latches (for external IrDA chip)
// TX, RX, CTS and RTS pins are enabled and used
#define U_RTS_CTS		0x0200

// Power saving and wake-up flags:
#define U_SIDL			0x2000 // Stop operation during idle
// If Wake-up on Start Bit Detect During Sleep Mode is enabled
// UART will continue to sample the RX pin; interrupt is generated on the
// falling edge, bit is cleared in hardware on the following rising edge
#define U_WAKE			0x0080 // UART Wake-up enable bit

// Receive Polarity Inversion bit (Idle is '1' by default)
#define U_RXINV			0x0010 // RX Idle state is '0'
/*
* UART Status and Control Register
*/
// UART Status and Control Register
#define _USTA(n)		U##n##STA
#define _USTAbits(n)	U##n##STAbits
#define USTAbits(n)		_USTAbits(n)
#define USTA(n)			_USTA(n)

// By default Transmit is disabled, any pending transmission is
// aborted and the buffer is reset; TX pin is controlled by port
// If Transmit is enabled, TX pin controlled by UART
#define U_TXEN			0x0400 // Transmit Enable bit
#define UART_ENABLE_TX(n)		USTAbits(n).UTXEN = 1
#define UART_DISABLE_TX(n)		USTAbits(n).UTXEN = 0
#define UART_IS_ENABLE_TX(n)	(USTAbits(n).UTXEN == 1)

// Transmission Interrupt Mode Selection bits:
// (def) Interrupt when a character is transferred to the Transmit
// Shift Register (this implies there is at least one character
#define U_TXI_READY		0x0000 // open in the transmit buffer)
// Interrupt when a character is transferred to the Transmit
// Shift Register (TSR) and as a result, the transmit buffer
#define U_TXI_EMPTY		0x8000 //  becomes empty
// Interrupt when the last character is shifted out of
// the Transmit Shift Register; all transmit operations
#define U_TXI_END		0x2000 // are completed

#define UART_SET_TXI(n, txi) /* Set new TXI value */\
USTA(n) = (USTA(n) & ~(U_TXI_END | U_TXI_EMPTY)) | txi

// Receive Interrupt Mode Selection bits
// (def) Interrupt is set when any character is received and
// transferred from the RSR to the receive buffer; receive buffer
#define U_RXI_ANY		0x0000 // has one or more characters
// Interrupt is set on an RSR transfer, making the receive buffer 3/4
#define U_RXI_3DATA		0x0080 // full (i.e., has 3 data characters)
// Interrupt is set on an RSR transfer, making the receive buffer
#define U_RXI_FULL		0x00C0 // full (i.e., has 4 data characters)

// 0 => Transmit buffer is not full, at least one more character can
#define UART_CAN_WRITE(n)	(USTAbits(n).UTXBF == 0) // be written
// 1 => Transmit Shift Register is empty and transmit
// buffer is empty (the last transmission has completed)
#define UART_IS_TXEND(n)	(USTAbits(n).TRMT != 0)

// Receive Buffer Data Available bit (read-only). Receive
// buffer has data, at least one more character can be read
#define UART_CAN_READ(n) (USTAbits(n).URXDA != 0)

// Receiver errors:
#define U_PERR			0x0008
#define U_FERR			0x0004
#define U_OERR			0x0002
#define UART_IS_RXERR(n)	((USTA(n) & (U_PERR|U_FERR|U_OERR)) != 0)
// Receive Buffer Overrun Error Status bit (clear/read-only). Must be
// cleared by software. Clearing a previously set OERR bit (1 -> 0
// transition) will reset the receiver buffer and the RSR to the
#define UART_IS_OERR(n)	(USTAbits(n).OERR != 0) //  empty state
#define UART_CLR_OERR(n)	USTAbits(n).OERR = 0
// Parity Error Status bit (read-only). Parity error has been detected for
// the current character (character at the top of the receive FIFO)
#define UART_IS_PERR(n)	(USTAbits(n).PERR != 0)
// Framing Error Status bit (read-only). Framing error has been detected
// for the current character (character at the top of the receive FIFO)
#define UART_IS_FERR(n)	(USTAbits(n).FERR != 0)

// Receiver Idle bit (read-only)
#define UART_IS_RXIDLE(n)	(USTAbits(n).RIDLE != 0)

// Send Sync Break on next transmission – Start bit, followed by twelve
// '0' bits, followed by Stop bit; cleared by hardware upon completion
#define UART_SET_BREAK(n) USTAbits(n).UTXBRK = 1

// Address Character Detect bit (bit 8 of received data = 1). If 9-bit mode
#define U_ADDEN		0x0020 //  is not selected, this does not take effect
#define UART_SET_ADDEN(n)	USTAbits(n).ADDEN = 1 // Wait Address byte
#define UART_CLR_ADDEN(n)	USTAbits(n).ADDEN = 0 // Wait Data bytes

// IrDA Encoder Transmit Polarity Inversion bit
#define U_TXINV			0x4000 // TX is Idle 'IREN'
/*
* Interrupt management of UART module
*/
// Interrupt Priority levels
#define _URXIP(n) _U##n##RXIP
#define _UTXIP(n) _U##n##TXIP
#define _UERIP(n) _U##n##ERIP
// Setup and obtain UART IPL values
#define UART_SET_RX_IPL(n, ipl)	_URXIP(n) = ipl
#define UART_GET_RX_IPL(n)		((int)_URXIP(n))
#define UART_SET_TX_IPL(n, ipl)	_UTXIP(n) = ipl
#define UART_GET_TX_IPL(n)		((int)_UTXIP(n))
#define UART_SET_ER_IPL(n, ipl)	_UERIP(n) = ipl
#define UART_GET_ER_IPL(n)		((int)_UERIP(n))

// Interrupt Enable bits
#define _URXIE(n) _U##n##RXIE
#define _UTXIE(n) _U##n##TXIE
#define _UERIE(n) _U##n##ERIE
// Enable and disable UART interrupts
#define UART_ENABLE_RXINT(n)	_URXIE(n) = 1
#define UART_DISABLE_RXINT(n)	_URXIE(n) = 0
#define UART_IS_ENABLE_RXINT(n)	(_URXIE(n) == 1)
#define UART_ENABLE_TXINT(n)	_UTXIE(n) = 1
#define UART_DISABLE_TXINT(n)	_UTXIE(n) = 0
#define UART_IS_ENABLE_TXINT(n)	(_UTXIE(n) == 1)
#define UART_ENABLE_ERINT(n)	_UERIE(n) = 1
#define UART_DISABLE_ERINT(n)	_UERIE(n) = 0
#define UART_IS_ENABLE_ERINT(n)	(_UERIE(n) == 1)

// Interrupt Status bits
#define _URXIF(n) _U##n##RXIF
#define _UTXIF(n) _U##n##TXIF
#define _UERIF(n) _U##n##ERIF
// Clear, Set and Check Interrupt Status
#define UART_CLR_RXFLAG(n)	_URXIF(n) = 0
#define UART_SET_RXFLAG(n)	_URXIF(n) = 1
#define UART_IS_RXFLAG(n)	(_URXIF(n) != 0)
#define UART_CLR_TXFLAG(n)	_UTXIF(n) = 0
#define UART_SET_TXFLAG(n)	_UTXIF(n) = 1
#define UART_IS_TXFLAG(n)	(_UTXIF(n) != 0)
#define UART_CLR_ERFLAG(n)	_UERIF(n) = 0
#define UART_SET_ERFLAG(n)	_UERIF(n) = 1
#define UART_IS_ERFLAG(n)	(_UERIF(n) != 0)
/*
* Power management of UART module (PMDx.UnMD bit)
*/
#define _UMD(n)		_U##n##MD

#endif /*_UART_INCL_*/
