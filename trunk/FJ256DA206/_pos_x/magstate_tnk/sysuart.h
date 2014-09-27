/*
*	System uart user interface functions
*/
#ifndef _SYSUART_INCL_
#define _SYSUART_INCL_

#define SYS_UART_RXBUF_SIZE		32
#define SYS_UART_TXBUF_SIZE		32

int sysu_init(void);
int sysu_is_init(void);
void sysu_done(void);

int sysu_error(void);
int sysu_txevt(void);

// Buffer functions

int sysu_rxsize(void);
int sysu_txsize(void);
int sysu_rxcount(void);
int sysu_txcount(void);
void sysu_rxpurge(void);
void sysu_txpurge(void);
#define sysu_rxfull() (sysu_rxcount() == sysu_rxsize())
#define sysu_txfull() (sysu_txcount() == sysu_txsize())

// Read-write functions

int sysu_getc(void);
int sysu_putc(const int c);
int sysu_write(const char* buf, int len);
// int sysu_read(char* buf, int len);

#endif /*_SYSUART_INCL_*/
