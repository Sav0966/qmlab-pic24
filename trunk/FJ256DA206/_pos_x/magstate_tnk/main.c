#include <p24Fxxxx.h>
#include <tnkernel.h>
#include <config.h>
#include <buffer.h>
#include <timers.h>
#include <reset.h>
#include <osc.h>

#include "systime.h"
#include "sysuart.h"
#include "main.h"

#if (DISP_IPL != TN_INTERRUPT_LEVEL)
#error "DISP_IPL and TN_INTERRUPT_LEVEL must be equal"
#endif

TN_TCB tcb_Task_Main TN_DATA;
TN_TCB tcb_Task_UART TN_DATA;

TN_UWORD stk_Tmr_Task[TMR_TASK_STACK_SIZE] TN_DATA;
TN_UWORD stk_Idl_Task[IDL_TASK_STACK_SIZE] TN_DATA;
TN_UWORD stk_Task_Main[TASK_MAIN_STACK_SIZE] TN_DATA;
TN_UWORD stk_Task_UART[TASK_UART_STACK_SIZE] TN_DATA;

static LIST(IFUNC);		// Interrupt functions list
#define GET_IFUNC(pos)	LIST_DATA(IFUNC, LIST_GET(pos))
#define IFUNC_ITEM		LIST_ITEM(IFUNC)

static TN_EVENT sysu_event; // UART events

static void sysu_handler(void)
{ // System interrupt handler
	int mask = 0; // No events
	static int last_txevt;

	if (sysu_error())
	{ sysu_rxpurge() ; mask |= EV_ERR; }
	else if (sysu_rxcount()) mask |= EV_RXCHAR;

	if (last_txevt != sysu_txevt())
	{ last_txevt = sysu_txevt(); mask |= EV_TXEMPTY; }

	if (mask) tn_event_iset(&sysu_event, mask);
}

static struct _commbuf {
	char* pbuf;
	int	  size;
	volatile int offset;
} _rxb;

static void fTask_UART(void *param)
{
	for(;;)
	{
		TN_UWORD event;
		if (TERR_NO_ERR == tn_event_wait(
			(TN_EVENT*)param, EV_ERR | EV_RXCHAR |
			EV_TXEMPTY, TN_EVENT_WCOND_OR, &event,
			TN_WAIT_INFINITE))
		{
			tn_event_clear((TN_EVENT*)param, 0);

			if (event & EV_ERR) {
			}
			if (event & EV_RXCHAR) {
			}
			if (event & EV_TXEMPTY) {
			}
			if (event & EV_RXFLAG) {
			}
		} else break; // Error or exit
	}
}

static void uart_init(void)
{
	IFUNC sysu_ifunc = { 0, sysu_handler, SYS_UART_IPL };
	tn_event_create(&sysu_event, TN_EVENT_ATTR_MULTI, 0);

	if (set_interrupt(&sysu_ifunc)) { // Set handler
		tn_task_create( // Create UART task
			&tcb_Task_UART, // and run it
			fTask_UART, TASK_UART_PRIORITY,
			stk_Task_UART, TASK_UART_STACK_SIZE,
			&sysu_event, TN_TASK_START_ON_CREATION);
		sysu_init(); // Initialize UART module
	}
}

/* Reset all persistent and config data */
#define isPOWER_ON(i) (((i) & EXT_RESET) != 0)

static void power_on_init(void)
{
#ifdef __DEBUG
	// JTAG must be off to use RB8-11,12,13
	if (MCU_CONFIG1 & ~JTAGEN_OFF) while(1);
	// Check IESO bit in CONFIG2 (must be off)
	if (MCU_CONFIG2 & ~IESO_OFF) while(1);
#endif

#ifdef __DEBUG
	pins_init(); // Initialize pins at startup
	clr_reset_state();		// Needed for MCLR
#else
#warning "Check loader code in the flash"
#endif
}

void app_conf (void)
{
	while (!IS_MCU_PROGRAMMED()); // Stay here
	// SIM doesn't clear SRbits.IPL, ICD2 clears it
	SET_CPU_IPL(MAIN_IPL); // Set default by hands

	//	For POR, BOR and MCLR
	if (isPOWER_ON(get_reset_state())) power_on_init(); 

	osc_mode(__OSC__); // Select oscillator mode
	// Then disable all modules for energy saving
	PMD1 = PMD2 = PMD3 = PMD4 = PMD5 = PMD6 = -1;

	LIST_INIT(IFUNC);

	systime_init(INITIAL_TIME); // 10 ms period
}

void int_conf(void)
{
	uart_init();

	// Init dispatcher of events
	INT_INIT(DISP_INT, 0, DISP_IPL);
	TIMER_ON(SYS_TIMER); // Run system timer
}

void idle_handler (void)
{
	__asm__ volatile ("pwrsav	#1"); // Idle mode
}

SYS_INTERRUPT(DISP_VECTOR)
{
	static int last_tick;
	register void* pos;

	INT_CLR_FLAG(DISP_INT);

	// Check system_time
	pos = (void*)_sys_tick_;
	if ((int)pos != last_tick)
	{ // Switch context
		last_tick = (int)pos;
		tn_tick_int_processing();
		INT_SET_FLAG(DISP_INT);
		return;   // IF is set
	} // No RTOS services more

	pos = LIST_FIRST(IFUNC);
	while (pos) { // Check all sys events
		GET_IFUNC(pos)->hfunc();
		pos = LIST_NEXT(IFUNC, pos);
	}
}

int main(void)
{
    tn_start_system(stk_Tmr_Task, TMR_TASK_STACK_SIZE,
                    stk_Idl_Task, IDL_TASK_STACK_SIZE,
                    app_conf, int_conf, idle_handler);

	return( 0 ); // Never return
}

void *sys_malloc(size_t size)
{
	register void *ret;
	tn_sys_enter_critical();
		ret = malloc(size);
	tn_sys_exit_critical();
	return( ret );
}

void sys_free(void *ptr)
{
	tn_sys_enter_critical();
		free(ptr); // Locked
	tn_sys_exit_critical();
}

PIFUNC	set_interrupt(PIFUNC ifunc)
{
	PIFUNC data = sys_malloc(sizeof(IFUNC_ITEM));

	if (data) {
		void* pos;
		*data = *ifunc;
		{ tn_sys_enter_critical(); // SYS_Lock
			pos = LIST_FIRST(IFUNC);
			while (pos) {
				if (LIST_DATA(IFUNC, pos)->ipl < ifunc->ipl) break;
				pos = LIST_NEXT(IFUNC, pos); } // while (pos)
		 	LIST_INSL(IFUNC, pos, LIST_NODE(IFUNC, data));
		  tn_sys_exit_critical(); } // SYS_lock
		return( data ); // if()
	} return( 0 );
}

void del_interrupt(PIFUNC ifunc)
{
	tn_sys_enter_critical(); // SYS_Lock
		LIST_DEL(IFUNC, LIST_NODE(IFUNC, ifunc));
	tn_sys_exit_critical(); // SYS_lock
	sys_free(ifunc);
}
