#ifndef _SYS_IO_INCL_
#define _SYS_IO_INCL_

#include <tnkernel.h>

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

typedef struct _IOCB {
	void (*ifunc)(struct _IOCB *io); // System interrupt
	void (*efunc)(struct _IOCB *io); // Event handler (task)
	int			priority;	// Interrupt priority (0 - 127)

	int (*init)(void);		// Hardvare init
	void (*done)(void);		// Hardvare done
	int (*is_init)(void);	// Test function

	// Internal data
	TN_EVENT	event;		// Event object
	TN_UWORD	mask;		// Current event

} IOCB, *PIOCB;

PIOCB io_create(const PIOCB pcio);

#endif /*_SYS_IO_INCL_*/
