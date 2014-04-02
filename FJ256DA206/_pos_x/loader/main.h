#ifndef _MAIN_INCL_
#define _MAIN_INCL_

#if (__OSC__ != FRCDIV)
#error "Internal oscillator must be used"
#endif

#define UART_USED	SYS_UART
#define DATA_SIZE	32 /* Maximum data length */
#define RXBUF_SIZE	(2*DATA_SIZE + sizeof(":00000001FF\r\n"))
#define TXBUF_SIZE	16

typedef struct tagPROG {
	unsigned char _dummy;
	unsigned char nb;  // number of bytes
	unsigned int  addr; // start address
	unsigned char type; // record type
	unsigned char data[2*((DATA_SIZE+1)/2)];
} PROG, *PPROG;

typedef struct tagUARTBUF {
	union {
		PROG p; // programming data
		unsigned char b[sizeof(PROG)];
	} prog;

	unsigned char nrx;	// received bytes
	unsigned char ntx;	// transmitted bites
	unsigned char err;	// error counter
	unsigned char pos;	// current position

	unsigned char rxd[RXBUF_SIZE]; // RX buf
	unsigned char txd[TXBUF_SIZE]; // TX buf
} UARTBUF, *PUARTBUF;

void uart_init(PUARTBUF buf); // uartui.c
void uart_rx(PUARTBUF buf);
void uart_er(PUARTBUF buf);
void uart_tx(PUARTBUF buf);

void trm_command(PUARTBUF buf); // trmcmd.c

void hex_init(PUARTBUF buf);
void hex_command(PUARTBUF buf); // hexcmd.c

#endif /*_MAIN_INCL_*/
