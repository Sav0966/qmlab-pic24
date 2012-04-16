/*
*	UART 1-4 management functions
*/
#ifndef _UART_INCL_
#define _UART_INCL_
/*
* ~SHDN and ~INVALID lines of RS-232 Driver (if present)
*/
#define UART_IS_VALID(n)	!(U##n##_INVALID == 0)
#define UART_IS_SHDN(n)		(U##n##_SHDN == 0)
#define UART_WAKEUP(n)		U##n##_SHDN = 1
#define UART_SHDN(n)		U##n##_SHDN = 0
/*
* UART Transmit and Receive Registers
*/
#define UTXREG(n)	U##n##TXREG
#define URXREG(n)	U##n##RXREG
#define UART_WRITE(n, ch)	UTXREG(n) = ch
#define UART_READ(n)		((char)URXREG(n))
#define UART_READ9(n)		((int)URXREG(n))
/*
* UART Baud Rate Generator Prescaler Register
*/
#define UBRG(n)		U##n##BRG
// Computation of the baud rate whith RBGH = 0
#define FCY2BRG(fcy, rate) ((int)((fcy/(16L*rate))-1))
// Computation of the baud rate whith RBGH = 1
#define FCY2BRGH(fcy, rate) ((int)((fcy/(4L*rate))-1))
/*
* UART Mode Register and appropriate mode settings
*/
// UART Mode Register and its bits
#define UMODE(n)		U##n##MODE
#define UMODEbits(n)	U##n##MODEbits

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

// Loopback Mode Rx<->Tx, RTS<->CTS (if used)
// LPBACK = 1 should be set only after enabling
// the other bits associated with the UART module
#define UART_SET_LPBACK(n)	UMODEbits(n).LPBACK = 1
#define UART_CLR_LPBACK(n)	UMODEbits(n).LPBACK = 0
#define UART_IS_LPBACK(n)	(UMODEbits(n).LPBACK == 1)
#define U_LPBACK	0x0040 // Loopback Mode Select bit

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
#define U_RIDL			0x0000 // (def) Operate during idle
#define U_SIDL			0x2000 // Stop operation during idle
// If Wake-up on Start Bit Detect During Sleep Mode is enabled
// UART will continue to sample the RX pin; interrupt is generated on the
// falling edge, bit is cleared in hardware on the following rising edge
#define U_WAKE			0x0080 // UART Wake-up enable bit

// Enable baud rate measurement on the next character Ц requires reception
// of a Sync field (55h); cleared in hardware upon completion
#define U_ABAUD			0x0020 // Auto-Baud Enable bit

// Receive Polarity Inversion bit (Idle is '1' by default)
#define U_RXINV			0x0010 // RX Idle state is '0'
/*
* UART Status and Control Register
*/
// UART Status and Control Register
#define USTA(n)			U##n##STA
#define USTAbits(n)		U##n##STAbits

// By default Transmit is disabled, any pending transmission is
// aborted and the buffer is reset; TX pin is controlled by port
// If Transmit is enabled, TX pin controlled by UART
#define U_TXEN			0x0400 // Transmit Enable bit
#define UART_ENABLE_TX(n)		USTAbits(n).UTXEN = 1
#define UART_DISABLE_TX(n)		USTAbits(n).UTXEN = 0

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
#define UART_IS_ERR(n)	((USTA(n) & (U_PERR|U_FERR|U_OERR)) != 0)
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
#define UART_IS_RXIDLE(n)	(USTAbits(n).RIDLE !=0)

// Send Sync Break on next transmission Ц Start bit, followed by twelve
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
// IPL register definitions
#define IPCU1_Rbits		IPC2bits
#define IPCU1_Tbits		IPC3bits
#define IPCU1_ERbits	IPC16bits
#define IPCU2_Rbits		IPC7bits
#define IPCU2_Tbits		IPC7bits
#define IPCU2_ERbits	IPC16bits
#define IPCU3_Rbits		IPC20bits
#define IPCU3_Tbits		IPC20bits
#define IPCU3_ERbits	IPC20bits
#define IPCU4_Rbits		IPC22bits
#define IPCU4_Tbits		IPC22bits
#define IPCU4_ERbits	IPC21bits

// Setup and obtain UART interrupt pryority levels
#define UART_SET_RX_IPL(n, ipl)	IPCU##n##_Rbits.U##n##RXIP = ipl
#define UART_GET_RX_IPL(n)		((int)IPCU##n##_Rbits.U##n##RXIP)
#define UART_SET_TX_IPL(n, ipl)	IPCU##n##_Tbits.U##n##TXIP = ipl
#define UART_GET_TX_IPL(n)		((int)IPCU##n##_Tbits.U##n##TXIP)
#define UART_SET_ER_IPL(n, ipl)	IPCU##n##_ERbits.U##n##ERIP = ipl
#define UART_GET_ER_IPL(n)		((int)IPCU##n##_ERbits.U##n##ERIP)

// IEC register definitions
#define IECU1_Rbits		IEC0bits
#define IECU1_Tbits		IEC0bits
#define IECU1_ERbits	IEC4bits
#define IECU2_Rbits		IEC1bits
#define IECU2_Tbits		IEC1bits
#define IECU2_ERbits	IEC4bits
#define IECU3_Rbits		IEC5bits
#define IECU3_Tbits		IEC5bits
#define IECU3_ERbits	IEC5bits
#define IECU4_Rbits		IEC5bits
#define IECU4_Tbits		IEC5bits
#define IECU4_ERbits	IEC5bits

// Enable and disable UART interrupts
#define UART_ENABLE_RX_INT(n)	IECU##n##_Rbits.U##n##RXIE = 1
#define UART_DISABLE_RX_INT(n)	IECU##n##_Rbits.U##n##RXIE = 0
#define UART_ENABLE_TX_INT(n)	IECU##n##_Tbits.U##n##TXIE = 1
#define UART_DISABLE_TX_INT(n)	IECU##n##_Tbits.U##n##TXIE = 0
#define UART_ENABLE_ER_INT(n)	IECU##n##_ERbits.U##n##ERIE = 1
#define UART_DISABLE_ER_INT(n)	IECU##n##_ERbits.U##n##ERIE = 0

// IFS register definitions
#define IFSU1_Rbits		IFS0bits
#define IFSU1_Tbits		IFS0bits
#define IFSU1_ERbits	IFS4bits
#define IFSU2_Rbits		IFS1bits
#define IFSU2_Tbits		IFS1bits
#define IFSU2_ERbits	IFS4bits
#define IFSU3_Rbits		IFS5bits
#define IFSU3_Tbits		IFS5bits
#define IFSU3_ERbits	IFS5bits
#define IFSU4_Rbits		IFS5bits
#define IFSU4_Tbits		IFS5bits
#define IFSU4_ERbits	IFS5bits
#define UART_TX_INTFLAG(n) IFSU##n##_Tbits.U##n##TXIF
#define UART_RX_INTFLAG(n) IFSU##n##_Rbits.U##n##RXIF
#define UART_ER_INTFLAG(n) IFSU##n##_ERbits.U##n##ERIF

// —менить все определени€ на регистронезависимые =!=
// #define UART_TX_INTFLAG(n) _U##n##TXIF

// Clear, Set and check Interrupt Status bit
#define UART_CLR_RXFLAG(n)	UART_RX_INTFLAG(n) = 0
#define UART_SET_RXFLAG(n)	UART_RX_INTFLAG(n) = 1
#define UART_IS_RXFLAG(n)	UART_RX_INTFLAG(n)
#define UART_CLR_TXFLAG(n)	UART_TX_INTFLAG(n) = 0
#define UART_SET_TXFLAG(n)	UART_TX_INTFLAG(n) = 1
#define UART_IS_TXFLAG(n)	UART_TX_INTFLAG(n)
#define UART_CLR_ERFLAG(n)	UART_ER_INTFLAG(n) = 0
#define UART_SET_ERFLAG(n)	UART_ER_INTFLAG(n) = 1
#define UART_IS_ERFLAG(n)	UART_ER_INTFLAG(n)

// Most usable deinitoins of inntrrupt routine functions
#define _UART_RX_INTFUNC(n) void __attribute__((__interrupt__, no_auto_psv)) _U##n##RXInterrupt(void)
#define _UART_TX_INTFUNC(n) void __attribute__((__interrupt__, no_auto_psv)) _U##n##TXInterrupt(void)
#define _UART_ER_INTFUNC(n) void __attribute__((__interrupt__, no_auto_psv)) _U##n##ErrInterrupt(void)
#define UART_RX_INTFUNC(n) _UART_RX_INTFUNC(n)
#define UART_TX_INTFUNC(n) _UART_TX_INTFUNC(n)
#define UART_ER_INTFUNC(n) _UART_ER_INTFUNC(n)
/*
* Power management of UART module
*/
// PMD register definitions
#define PMDU1bits	PMD1bits
#define PMDU2bits	PMD1bits
#define PMDU3bits	PMD3bits
#define PMDU4bits	PMD4bits
#define UART_PMD(n)	PMDU##n##bits.U##n##MD
/*
* UART Initialization
*/
#define UART_IS_INIT(n) /* Powered & Enabled */\
(!UART_PMD(n) && UMODEbits(n).UARTEN)

#define UART_INIT(n, mode, sta, brg, rx_ipl, tx_ipl, er_ipl) {\
	UART_DISABLE_RX_INT(n); /* Disable all interrupts */\
	UART_DISABLE_TX_INT(n); UART_DISABLE_ER_INT(n);\
\
	if (UART_IS_VALID(n)) { /* Valid input levels */\
		UART_WAKEUP(n); /* Wake-up RS-232 Driver */\
		UART_PMD(n) = 0; /* Power on UART module */\
\
		/* Setup mode (UART disabled). Setup control */\
		/* bits and clear TX buffer and RX overflow error */\
		UMODE(n) = (mode) & ~U_EN; USTA(n) = (sta) & ~U_TXEN;\
		UBRG(n) = brg; /* Write appropriate baud rate value */\
		/* Clear all interrupt status flags (Rx, Tx and Error */\
		UART_CLR_RXFLAG(n); UART_CLR_TXFLAG(n); UART_CLR_ERFLAG(n); \
\
		if (rx_ipl >= 0) {\
			UART_SET_RX_IPL(n, rx_ipl); /* Receive IPL */\
			UART_ENABLE_RX_INT(n); /* Enable interrupt */\
		}\
\
		if (tx_ipl >= 0) {\
			UART_SET_TX_IPL(n, tx_ipl); /* Transmit IPL */\
			UART_ENABLE_TX_INT(n); /* Enable interrupt */\
		}\
\
		if (er_ipl >= 0) {\
			UART_SET_ER_IPL(n, er_ipl); /* Error IPL */\
			UART_ENABLE_ER_INT(n); /* Enable interrupt */\
		}\
\
		/* The UTXEN bit should not be set until the */\
		/* UARTEN bit has been set; otherwise, UART */\
		/* transmissions will  not be enabled */\
		/* Enable UART if it's needed in 'mode' */\
		if ((mode) & U_EN) UMODEbits(n).UARTEN = 1;\
		/* Enable Transmitter if it's needed */\
		if ((sta) & U_TXEN) UART_ENABLE_TX(n);\
\
	} else { /* RS-232 por isn't connected  to PC */\
		UART_SHDN(n); /* Shutdown RS-232 Driver */\
		UART_PMD(n) = 1; /* Power off UART module */\
	} /* UART_IA_VALID() */\
}

#endif /*_UART_INCL_*/
