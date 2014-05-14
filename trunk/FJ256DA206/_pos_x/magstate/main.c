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
	// Run Timer1 (2 MHz) with 10 ms elapse period
	if (clock_init(INITIAL_TIME) < 0) while (1);
	// For empty loop and SYS_TIMER using only:
	// 12.6V -> 6.3 mA (PRIPLL) or 2.2mA (FRCPLL)

	power_init(); // Init power module
	sysu_init(); // Init system UART

	disp_init(); // Init dispatcher

	for(;;) { // Main loop

		on_idle(); // Idle loop
	} // Main loop

	return(0); // Never return
}
