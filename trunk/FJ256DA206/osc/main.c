#include "main.h"
#include "mcu_id.h"
#include "reset.h"
#include "osc.h"

/* Reinit all persistent and config data */
#define isPOWER_ON(i) (((i) & EXT_RESET) != 0)

unsigned int mcu_id;
unsigned int rst_state; /* Current reset state */
unsigned int __attribute__((persistent)) rst_events;
unsigned int __attribute__((persistent)) rst_num;

static void power_on_init(void)
{
	clr_reset_state(); // Needed for MCLR
 	rst_events = 0; rst_num = 0;
}

int main(void)
{
	if (!IS_MCU_PROGRAMMED()) /* Stay in programming */
	__delay32((unsigned long)((1000)*(FCY_UP)/1000ULL));

	/* SIM doesn't clear SRbits.IPL, ICD2 clears it */
	SET_CPU_IPL(MAIN_IPL); /* Set default by hands */

	mcu_id = MCU_ID;
	if (isPOWER_ON(rst_state = get_reset_state()))
		power_on_init(); /*	For POR, BOR and MCLR */

	rst_events |= rst_state; /* As RCON register */
	++rst_num; /* Calculate session reset number */

	do { // Main loop
		int cfg = MCU_CONFIG2; // Check CONFIG2 word
		if (cfg  & ~IESO_OFF) break; // Must be off

		/* Select reference clock = FCY/1024 and */
		REFOCON = 0x0200; /* disable it in sleep */
		/* Select AN15/RP29/REFO/RB15 as output */
		__asm__ volatile (/* pin in HIGHT state */\
		"	bclr	ANSB, #15	; Disable analog\n"
		"	bset	LATB, #15	; Set latch RB15\n"
// LOW	"	bclr	LATB, #15	; Clear latch RB15\n"
		"	bclr	TRISB, #15	; Enables output\n"
		); // It's not needed, but we set it by hand

		REFOCONbits.ROON = 1; // Enable REFO

		/* Turn on external oscillator FOX924B: */
		__asm__ volatile (/* OSCO/CLKO/RC15 pin */\
		"	bclr	LATC, #15	; Clear latch RC15\n"
		"	bclr	TRISC, #15	; Enables output\n"
		); // Set OSCO pin as output in low state
		__delay32(// Wait FOX924 startup time 5ms
			(unsigned long)((5)*(FCY_UP)/1000ULL));

		/* Set ECPLL oscillator mode, FCY = 32 MHZ */
		CLKDIVbits.PLLEN = 1; // Enable PLL96MHZ
		osc_switch(FNOSC_PRIPLL); // ECPLL mode
		/*
		* Do something, that is't clock-sensitive
		*/
		while (OSCCONbits.OSWEN != 0); // Wait



		/* Set FRCDIV oscillator mode, FCY = 4 MHZ */
		osc_switch(FNOSC_FRCDIV); // FRCDIV mode
		/*
		* Do something, that is't clock-sensitive
		*/
		while (OSCCONbits.OSWEN != 0); // Wait
		CLKDIVbits.PLLEN = 0; // Disable PLL96MHZ

		/* Turn off external oscillator FOX924B: */
		__asm__ volatile (/* OSCO/CLKO/RC15 pin */\
		"	bset	LATC, #15	; Set latch RC15\n"
		"	bclr	TRISC, #15	; Enables output\n"
		); // Set OSCO pin as output in hight state

		REFOCONbits.ROON = 0; // Disable REFO

		__delay32(// Wait 5ms and run loop again
			(unsigned long)((5)*(FCY_UP)/1000ULL));
	} while (1); // Main loop

	clr_reset_state(); // Clear uParam and RCON

	for(;;);
	return(0); /* Never return */
}
