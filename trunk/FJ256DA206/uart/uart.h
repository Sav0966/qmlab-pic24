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
* Power management of UART module
*/
// PMD register definitions
#define PMDU1bits	PMD1bits
#define PMDU2bits	PMD1bits
#define PMDU3bits	PMD3bits
#define PMDU4bits	PMD4bits

#define UART_PMD(n)		PMDU##n##bits.U##n##MD
#define UART_PWON(n)	UART_PMD(n) = 0
#define UART_PWOFF(n)	UART_PMD(n) = 1
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
* UART Initialization
*/
#define UART_INIT(n, rx_ipl, tx_ipl, er_ipl) {\
	if (UART_IS_VALID(n)) { /* Valid input levels */\
		UART_WAKEUP(n); /* Wake-up RS-232 Driver */\
		UART_PWON(n);   /* Power on UART module */\
	} else { /* RS-232 por isn't connected  to PC*/\
		UART_SHDN(n); /* Shutdown RS-232 Driver */\
		UART_PWOFF(n); /* Power off UART module */\
	}\
\
	/* Clear all interrupt status flags (Rx, Tx and Error */\
	UART_CLR_RXFLAG(n); UART_CLR_TXFLAG(n); UART_CLR_ERFLAG(n); \
\
	if (rx_ipl >= 0) {\
		UART_SET_RX_IPL(n, rx_ipl); /* Receive IPL */\
		UART_ENABLE_RX_INT(n); /* Enable interrupt */\
	} else UART_DISABLE_RX_INT(n); /* or disable it */\
\
	if (tx_ipl >= 0) {\
		UART_SET_TX_IPL(n, tx_ipl); /* Transmit IPL */\
		UART_ENABLE_TX_INT(n); /* Enable interrupt */\
	} else UART_DISABLE_TX_INT(n); /* or disable it */\
\
	if (er_ipl >= 0) {\
		UART_SET_ER_IPL(n, er_ipl); /* Error IPL */\
		UART_ENABLE_ER_INT(n); /* Enable interrupt */\
	} else UART_DISABLE_ER_INT(n); /* or disable it */\
}

#endif /*_UART_INCL_*/
