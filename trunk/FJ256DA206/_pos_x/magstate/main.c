#include <p24Fxxxx.h>
#include <config.h>
#include <power.h>
#include <reset.h>
#include <clock.h>
#include <ints.h>
#include <osc.h>

#include "main.h"
#include "dispatch.h"
#include "sysuart.h"

// 20 џэт 1997 15:00:00
#define INITIAL_TIME	853801200L

/* Reinit all persistent and config data */
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

static void on_idle(void)
{

	__asm__ volatile ("pwrsav	#1"); // Idle mode
}

#define IO_IPL		1

#define ENTER_IO_LEVEL() {\
	__asm__ volatile ("push SR\n");\
	SR = (IO_IPL << 5)

#define LEAVE_IO_LEVEL()\
	__asm__ volatile ("pop SR\n"); } ((void)0)

static int uart_erhook(int);
static int uart_rxhook(int);
static int uart_txhook(int);

static HOOK _uart_erhook = { sysu_error, uart_erhook };
static HOOK _uart_rxhook = { sysu_rxcount, uart_rxhook };
static HOOK _uart_txhook = { sysu_txevt, uart_txhook };


static int rx_cnt; // = 0
static unsigned char rx_buf[25];
static unsigned char evt_char; // = 0
#define ARSIZE(ar) (sizeof(ar)/sizeof(ar[0]))

static int uart_erhook(int evt /* == sysu_error() */)
{ // DISPATCH_LEVEL
	sysu_rxpurge();
	return( 0 ); // No errors
}

static int uart_rxhook(int evt /* == sysu_txcount() */)
{
	int i;
	static char busy = 0;

	// DISPATCH_LEVEL
	if (busy) return( evt );
	busy = 1; // Prevent recursion
	ENTER_IO_LEVEL(); // IO_LEVEL

	for (; rx_cnt < ARSIZE(rx_buf); ++rx_cnt)
	{
		if ((rx_cnt == 0) && rx_buf[0] != 0) break;
		if ((i = sysu_getc()) < 0) break;

		rx_buf[rx_cnt] = (unsigned char)i;

		if (rx_buf[rx_cnt] == evt_char)
		{ // New command has been received
			MSG msg;
			msg.message = 1;
			msg.wParam = (int)rx_buf;
			msg.lParam = rx_cnt;
			msg.time = sys_clock();

			if (push_msg(&msg) == NULL)
				__asm__ volatile ("nop"); // !!!

			rx_cnt = 0; break;
		}
	}

	if (rx_cnt >= ARSIZE(rx_buf))
		rx_buf[rx_cnt = 0] = 0;

	LEAVE_IO_LEVEL(); // IO_LEVEL
	busy = 0; //  DISPATCH_LEVEL

	// We can skip event
	if (sysu_rxcount()) DISPATCH();

	// No bytes to read
	return( _uart_rxhook.prev->pfnhook(0) );
}

static int uart_txhook(int evt /* == sysu_txevt() */)
{
	static char busy = 0;

	// DISPATCH_LEVEL
	if (busy) return( evt );
	busy = 1; // Prevent recursion
	ENTER_IO_LEVEL(); // IO_LEVEL

	LEAVE_IO_LEVEL(); // IO_LEVEL
	busy = 0; //  DISPATCH_LEVEL

	return( evt ); // Wait for next event
}

int uart_init(void)
{
	int ok = 0;

	sysu_done();
	ENTER_DISP_LEVEL();
		if (disp_sethook(DISP_SYSUTX, &_uart_txhook))
		if (disp_sethook(DISP_SYSUER, &_uart_erhook))
		if (disp_sethook(DISP_SYSURX, &_uart_rxhook)) ok = 1;
	LEAVE_DISP_LEVEL();

	return( ok? sysu_init() : 0 );
}

int main(void)
{
	while (!IS_MCU_PROGRAMMED()); // Stay here
	// SIM doesn't clear SRbits.IPL, ICD2 clears it
	SET_CPU_IPL(MAIN_IPL); // Set default by hands

	//	For POR, BOR and MCLR
	if (isPOWER_ON(get_reset_state())) power_on_init(); 

	// Then disable all modules for energy saving
	PMD1 = PMD2 = PMD3 = PMD4 = PMD5 = PMD6 = -1;

	osc_mode(__OSC__); // Select oscillator mode

	disp_init(); // Init dispatcher

	// Run Timer1 (2 MHz) with 10 ms elapse period
	if (clock_init(INITIAL_TIME) < 0) while (1);
	// For empty loop and SYS_TIMER using only:
	// 12.6V -> 6.3 mA (PRIPLL) or 2.2mA (FRCPLL)

	power_init(); // Init power module
	uart_init(); // Init system UART

	for(;;) { // Main loop
		MSG msg;
		while (pop_msg(&msg) != NULL)
		{
			if (msg.message == 1)
			{
				unsigned char *buf = 
					(unsigned char*)msg.wParam;

				buf[0] = 0; // Complite
			}

		}

sysu_test();

		on_idle(); // Idle loop
	} // Main loop

	return(0); // Never return
}
