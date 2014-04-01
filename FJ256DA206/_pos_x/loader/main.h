#ifndef _MAIN_INCL_
#define _MAIN_INCL_

#if (__OSC__ != FRCDIV)
#error "Internal oscillator must be used"
#endif

#define UART_USED	SYS_UART
#define RXBUF_SIZE	128
#define TXBUF_SIZE	16

typedef struct tagUARTBUF {
	unsigned char nrx;	// received bytes
	unsigned char ntx;	// transmitted bites
	unsigned char err;	// error counter
	unsigned char pos;	// current position
	unsigned char rxd[RXBUF_SIZE]; // RX buf
	unsigned char txd[TXBUF_SIZE]; // TX buf
} UARTBUF, *PUARTBUF;

extern volatile UARTBUF __attribute__((persistent)) buf;

void uart_init(void); // uartui.c
void uart_rx(void);
void uart_er(void);
void uart_tx(void);

void trm_command(void); // trmcmd.c

void hex_command(void); // hexcmd.c

#endif /*_MAIN_INCL_*/
