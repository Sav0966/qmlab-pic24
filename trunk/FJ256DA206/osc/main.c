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
unsigned long nOscFail; // Trap error counter

static void power_on_init(void)
{
	clr_reset_state(); // Needed for MCLR
 	rst_events = 0; rst_num = 0;
}

/* Oscillator fail trap handler */
void __attribute__((__interrupt__, auto_psv))
_OscillatorFail(void)
{
	// Now we have FRC oscillator mode
	osc_mode(OSCCON >> 8); // Try to restore NOSC mode
	INTCON1bits.OSCFAIL = 0; // Clear interrupt flag
	++nOscFail;
}

int main(void)
{
	if (!IS_MCU_PROGRAMMED()) /* Stay in programming */
	__delay32((unsigned long)((1000)*(FCY_UP2)/1000UL));

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
		REFOCON = 0x0A00; /* disable it in sleep */
		/* Select AN15/RP29/REFO/RB15 as output */
		__asm__ volatile (/* pin in HIGHT state */\
		"	bclr	ANSB, #15	; Disable analog\n"
		"	bset	LATB, #15	; Set latch RB15\n"
// LOW	"	bclr	LATB, #15	; Clear latch RB15\n"
		"	bclr	TRISB, #15	; Enables output\n"
		); // It's not needed, but we set it by hand

		REFOCONbits.ROON = 1; // Enable REFO

		// Set different oscillator modes, save FCY and Ip
		osc_mode(FRCPLL);	// FRCPLL	FCY = 10,66 MHz	9.5 mA
		osc_mode(FRCDIV);	// FRCDIV	FCY = 4 MHz		4.9 mA
		osc_mode(FRC16);	// FRC16	FCY = 0.5 MHz	3.1 mA
		osc_mode(FRC);		// FRC  	FCY = 8 MHz		6.9 mA
//		osc_mode(LPRC);		// LPRC		FCY = 31857 Hz	2.6 mA
//		osc_mode(SOSC);
		osc_mode(PRIPLL);	// ECPLL	FCY = 32 MHZ	24.6 mA
		osc_mode(PRI);		// EC		FCY = 12 MHz	11.0 mA

		/* Turn off external oscillator FOX924B */
//		if (osc_ec_on(0)) break; // break in error

		REFOCONbits.ROON = 0; // Disable REFO

		delay_ms(5); // Wait 5ms and run loop again
	} while (1); // Main loop

	clr_reset_state(); // Clear uParam and RCON

	for(;;);
	return(0); /* Never return */
}