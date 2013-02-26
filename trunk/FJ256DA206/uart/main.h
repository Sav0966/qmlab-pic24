#ifndef _MAIN_INCL_
#define _MAIN_INCL_

#define BIOS_START_TIME	853801200L	// 20 џэт 1997 15:00:00

#define UART_USED	1 			// Checked URAT module (1-4)
// You must enable UART1 IO in SIM Simulator Settings Dialog
// Check 'Enable UART1 IO', 'Rewind input', Output 'Window'
#define UART_TXBUF_SIZE	32 // Size of Transmitter queue
#define UART_RXBUF_SIZE	16 // Size of Receiver queue

extern unsigned int rst_state;
extern unsigned int mcu_id;

#endif /*_MAIN_INCL_*/
