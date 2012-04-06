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
//#define IFST1bits	IFS0bits
//#define IFST2bits	IFS0bits
//#define IFST3bits	IFS0bits
//#define IFST4bits	IFS1bits

// Clear Interrupt Status bit
//#define TIMER_INTFLAG(timer) IFST##timer##bits.T##timer##IF
//#define TIMER_CLR_INTFLAG(timer) IFST##timer##bits.T##timer##IF = 0
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
\
}

#endif /*_UART_INCL_*/
