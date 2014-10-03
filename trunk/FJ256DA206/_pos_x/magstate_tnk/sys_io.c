#include <p24Fxxxx.h>
#include <tnkernel.h>
#include <string.h>
#include <config.h>

#include "sysuart.h"
#include "sys_io.h"
#include "main.h"

static TN_TCB	*uart_task;  // UART task
static TN_UWORD	*uart_stack; // Task stack
static TN_EVENT	*uart_event; // UART events
static PIFUNC	uart_ifunc; // Sys handler

static void uart_handler(void)
{ // System interrupt handler
	int mask = 0; // No events
	static int last_txevt;

	if (sysu_error())
	{ sysu_rxpurge() ; mask |= EV_ERR; }
	else if (sysu_rxcount()) mask |= EV_RXCHAR;

	if (last_txevt != sysu_txevt())
	{ last_txevt = sysu_txevt(); mask |= EV_TXEMPTY; }

	if (mask) tn_event_iset(uart_event, mask);
}

static struct _commbuf {
	char* pbuf;
	int	  size;
	volatile int offset;
} _rxb;

void uart_efunc(TN_UWORD event)
{
	if (event & EV_ERR) {
		__asm volatile ("nop\nnop\n");
	}
	if (event & EV_RXCHAR) {
		__asm volatile ("nop\nnop\n");
	}
	if (event & EV_TXEMPTY) {
		__asm volatile ("nop\nnop\n");
	}
	if (event & EV_RXFLAG) {
		__asm volatile ("nop\nnop\n");
	}
}

static void uart_func(void *param)
{
	for(;;)
	{
		TN_UWORD event;
		if (TERR_NO_ERR == tn_event_wait(
			(TN_EVENT*)param, EV_ERR | EV_RXCHAR |
			EV_TXEMPTY, TN_EVENT_WCOND_OR, &event,
			TN_WAIT_INFINITE)) uart_efunc(event);
		else break; // Error or exit
	}

	tn_task_exit( 0 );
}

void uart_done(void)
{
	if (sysu_is_init()) sysu_done(); // Reset module
	if (uart_ifunc) del_interrupt(uart_ifunc);
	uart_ifunc = 0; // Remove handler

	if (uart_task) {
		TN_TASK_REF ref;
		if (TERR_NO_ERR ==
			tn_task_reference(uart_task, &ref)) {

			tn_event_delete(uart_event); // Resume
			while (ref.state != TSK_STATE_DORMANT)
				tn_task_reference(uart_task, &ref);
			tn_task_delete(uart_task); // Remove
		}	

		sys_free(uart_task); uart_task = 0;
	}

	if (uart_event) { sys_free(uart_event); uart_event = 0; }
	if (uart_stack) { sys_free(uart_stack); uart_task = 0; }
}

int uart_init(void)
{
	IFUNC _ifunc = { 0, uart_handler, SYS_UART_IPL };

	if (!sysu_is_init()) // Once
	if ((uart_stack = sys_malloc( // Allocate memory
		sizeof(TN_UWORD) * TASK_UART_STACK_SIZE)) != 0)
	if ((uart_event = sys_malloc(sizeof(TN_EVENT))) != 0)
	if ((uart_task = sys_malloc(sizeof(TN_TCB))) != 0) // Last
	{
		memset(uart_task, 0, sizeof(TN_TCB));
		memset(uart_event, 0, sizeof(TN_EVENT));
		tn_event_create(uart_event, TN_EVENT_ATTR_SINGLE |
		/* Only one task can wait */ TN_EVENT_ATTR_CLR, 0);

		if (0 != (uart_ifunc =
			set_interrupt(&_ifunc))) // Set interrupt handler
			if (TERR_NO_ERR == // Create UART task and call
				tn_task_create( // uart_func(uart_event)
					uart_task,
					uart_func, TASK_UART_PRIORITY,
					uart_stack, TASK_UART_STACK_SIZE,
					uart_event, TN_TASK_START_ON_CREATION))
					// Try to initialize UART module
					if (sysu_init()) return( 1 );
		
	}

	uart_done(); // Reset all resource
	return( 0 ); // Error
}

static void uart_hfunc(struct _IOCB *io)
{ // System interrupt handler
	int mask = 0; // No events
	static int last_txevt;

	if (sysu_error())
	{ sysu_rxpurge() ; mask |= EV_ERR; }
	else if (sysu_rxcount()) mask |= EV_RXCHAR;

	if (last_txevt != sysu_txevt())
	{ last_txevt = sysu_txevt(); mask |= EV_TXEMPTY; }

	if (mask) tn_event_iset(uart_event, mask);
}

const IOCB comm = {
	uart_ifunc, uart_efunc, SYS_UART_IPL,
	sysu_init, sysu_done, sysu_is_init };

PIOCB io_create(const PIOCB pcio)
{
	PIOCB io = sys_malloc(sizeof(IOCB));

	if (io) {
		*io = *pcio;
	} return( io );
}
