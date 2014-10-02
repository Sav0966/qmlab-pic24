#ifndef _TNKERNEL_CONF_H
#define _TNKERNEL_CONF_H

#define  TN_DEBUG
#define  TN_NO_ERROR_CHECKING

/* RAM allocation for tasks */

#define TMR_TASK_STACK_SIZE		256
#define IDL_TASK_STACK_SIZE 	256

#define TASK_MAIN_STACK_SIZE	256
#define TASK_MAIN_PRIORITY		8

#define TASK_UART_STACK_SIZE	256
#define TASK_UART_PRIORITY		4

/* Communication events */

#define EV_RXCHAR	0x0001 // A character was received
#define EV_RXFLAG	0x0002 // The event character was received
#define EV_TXEMPTY	0x0004 // The last character was sent
#define EV_CTS		0x0008 // The CTS signal changed state
#define EV_DSR		0x0010 // The DSR signal changed state
#define EV_RLSD		0x0020 // The VALID signal changed state
#define EV_BREAK	0x0040 // A break was detected on input
#define EV_ERR		0x0080 // A line-status error occurred
// Line-status errors are CE_FRAME, CE_OVERRUN, CE_RXPARITY
#define EV_RING		0x0100 // A ring indicator was detected

#endif
