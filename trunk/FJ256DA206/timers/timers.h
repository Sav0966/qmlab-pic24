/*
*	Timers 1-5 management functions
*/
#ifndef _TIMERS_INCL_
#define _TIMERS_INCL_

#define T_ON		0x8000 /* Timer ON bit */

/* Timer gate time accumulation enable bit */
#define T_GATE_OFF	0x0000 /* Disabled (def) */
#define T_GATE_ON	0x0040 /* Enabled */

/* Timer input clock prescale select bits */
#define T_PS_8		0x0010 /* Prescaler 1:8 */
#define T_PS_64		0x0020 /* Prescaler 1:64 */
#define T_PS_256	0x0030 /* Prescaler 1:256 */ 
#define T_PS_NONE	0x0000 /* Prescaler 1:1 (def) */

#define T_SIDL		0x2000 /* Stop operation during idle */
#define T_RIDL		0x0000 /* Operate during idle (def) */

/* Timer 1 exterrnal clock input synchronization select bit */
#define T1_SYNC		0x0004 /* Synhronize external clock input */

#define T24_T32		0x0008 /* Timers 23(45) in 32-bit timer mode */

#define T_MODE_INT	0x0000 /* Internal clock source Fcy/2 (def) */
#define T_MODE_EXT	0x0002 /* External clock source (rising age) */
/* Gated time accumulation mode (internal clock source only) */
#define T_MODE_INT_GATE		(T_MODE_INT | T_GATE_ON)
/* Synchronous counter mode (external lock only) */
#define T1_MODE_EXT_SYNC	(T_MODE_EXT | T1_SYNC)

/* Timer control register */
#define TCON(timer)	T##timer##CON
#define TCONbits(timer) T##timer##CONbits

/* Start and stop timer counting */
#define TIMER_ON(timer)		TCONbits(timer).TON = 1
#define TIMER_OFF(timer)	TCONbits(timer).TON = 0
#define TIMER_IS_ON(timer)	(TCONbits(timer).TON == 1)

/* Read and write timer register */
#define TIMER_WRITE(timer, n)	TMR##timer = n
#define TIMER_READ(timer)		((int)TMR##timer)

/* Setup and obtain timer period register */
#define TIMER_SET_PR(timer, n)	PR##timer = n
#define TIMER_GET_PR(timer)		((int)PR##timer)

/* IPL register definitions */
#define IPCT1bits	IPC0bits
#define IPCT2bits	IPC1bits
#define IPCT3bits	IPC2bits
#define IPCT4bits	IPC6bits
#define IPCT5bits	IPC7bits

/* Setup and obtain timer interrupt pryority level */
#define TIMER_SET_IPL(timer, ipl)	IPCT##timer##bits.T##timer##IP = ipl
#define TIMER_GET_IPL(timer)		((int)IPCT##timer##bits.T##timer##IP)

/* IEC register definitions */
#define IECT1bits	IEC0bits
#define IECT2bits	IEC0bits
#define IECT3bits	IEC0bits
#define IECT4bits	IEC1bits
#define IECT5bits	IEC1bits

/* Enable and disable timer interrupt */
#define TIMER_ENABLE_INT(timer)		IECT##timer##bits.T##timer##IE = 1
#define TIMER_DISABLE_INT(timer)	IECT##timer##bits.T##timer##IE = 0

/* IFS register definitions */
#define IFST1bits	IFS0bits
#define IFST2bits	IFS0bits
#define IFST3bits	IFS0bits
#define IFST4bits	IFS1bits
#define IFST5bits	IFS1bits

/* Clear Interrupt Status bit */
#define TIMER_INTFLAG(timer) IFST##timer##bits.T##timer##IF
#define TIMER_CLR_INTFLAG(timer) TIMER_INTFLAG(timer) = 0
/*
* 16-bit timer initialization
*
* timer - timer number (1 - 5)
* mode - timer mode (T_MODE_x | [T_PS_x]|[T_SIDL] | [T_ON])
* ipl - interrupt priority level, if < 0 - no unterrupt
* n - the value of timer period register
*/
#define TIMER_INIT(timer, mode, n, ipl) {\
	PMD1bits.T##timer##MD = 0; /* Enable module to have access */\
	TCON(timer) = 0x00; /* Stop Timer and reset control register  */\
	TIMER_WRITE(timer, 0); /* Clear contents of the Timer register */\
	TIMER_SET_PR(timer, n); /* Load the Timer period register = n */\
	TIMER_CLR_INTFLAG(timer); /* Clear the interrupt status flag */\
\
	if (ipl >= 0) {\
		TIMER_SET_IPL(timer, ipl); /* Setup Timer IPL */\
		TIMER_ENABLE_INT(timer); /* Enable interrupt */\
	} else TIMER_DISABLE_INT(timer); /* or disable it */\
\
	TCON(timer) = mode; /* Setup Timer mode */\
}

#define TIMER_DONE(timer) {\
	TIMER_DISABLE_INT(timer);	/* Disable the interrupt */\
	TIMER_OFF(timer);			/* Disable timer */\
	TIMER_CLR_INTFLAG(timer);	/* Clear Timer interrupt flag */\
}

#define TIMER_PWOFF(timer) {\
	TIMER_DONE(timer);			/* Disable timer and interrupt */\
	PMD1bits.T##timer##MD = 1; 	/* Disable module to energy saving */\
}

#endif /*_TIMERS_INCL_*/
