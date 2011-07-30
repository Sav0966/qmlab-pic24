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

static void test_mode(OSC_MODE mode)
{
	int i = 250; // 5 seccond loop time

	osc_mode(mode); // Set oscillator mode
	osc_pll_on(0); // Try to turn off PLL96MHz
	osc_ec_on(0); // Try to turn off FOX924

	REFOCONbits.ROON = 1; // Enable REFO
	delay_ms(5000);  // Check FCY = REFO
	REFOCONbits.ROON = 0; // Disable REFO
	delay_ms(5000); // Wait and check Ip

	while (i--) // Check delay in loop
	{
		REFOCONbits.ROON = 1; delay_ms(10);
		REFOCONbits.ROON = 0; delay_ms(10);
	}

	// In LPRC and SOSC modes delay 10 ms -> 17 ms
}

int main(void)
{
	int cfg;

	if (!IS_MCU_PROGRAMMED()) /* Stay in programming */
	__delay32((unsigned long)((1000)*(FCY_UP2)/1000UL));

	/* SIM doesn't clear SRbits.IPL, ICD2 clears it */
	SET_CPU_IPL(MAIN_IPL); /* Set default by hands */

	mcu_id = MCU_ID;
	if (isPOWER_ON(rst_state = get_reset_state()))
		power_on_init(); /*	For POR, BOR and MCLR */

	rst_events |= rst_state; /* As RCON register */
	++rst_num; /* Calculate session reset number */

	cfg = MCU_CONFIG2; /* Check IESO bit in CONFIG2 */
	if (cfg  & ~IESO_OFF) while(1); /* Must be off */
	osc_mode(PRIPLL); /* Set main oscillator mode */

	/* Select reference clock = FCY/1 and */
	REFOCON = 0x0; /* disable it in sleep */
	/* Select AN15/RP29/REFO/RB15 as output */
	__asm__ volatile (/* pin in HIGHT state */\
		"	bclr	ANSB, #15	; Disable analog\n"
		"	bset	LATB, #15	; Set latch RB15\n"
// LOW	"	bclr	LATB, #15	; Clear latch RB15\n"
		"	bclr	TRISB, #15	; Enables output\n"
	); // It's not needed, but we set it by hand

	do { // Main loop

		// Set different oscillator modes, save FCY and Ip
		test_mode(FRCPLL);	// FRCPLL	FCY = 10,66 MHz	8.4 mA
		test_mode(FRCDIV);	// FRCDIV	FCY = 4 MHz		4.3 mA
		test_mode(FRC16);	// FRC16	FCY = 0.5 MHz	2.7 mA
		test_mode(FRC);		// FRC  	FCY = 8 MHz		6.1 mA
		test_mode(LPRC);	// LPRC		FCY = 31857 Hz	2.2 mA
		test_mode(SOSC);	// SOSC		FCY = 32768 Hz	2.2 mA
		test_mode(PRIPLL);	// ECPLL	FCY = 32 MHZ	22.3 mA
		test_mode(PRI);		// EC		FCY = 12 MHz	9.8 mA

		// Frequency dependence of additional current on REFO pin
		// FCY (MHz)	0.5		4		8		10.66	12		32
		// dIprefo(mA)	0		0.1		0.3		0.45	0.55	1.5
		// REFOCONbits.ROON = 1 (REFO is enabled)

		// Additional current if oscilloscope probe is connected
		// to REFO pin. Probe GTP-250A-2 (x10: R = 10MOm, C ~ 17pF)
		// FCY (MHz)	0.5		4		8		10.66	12		32
		// dIpbrobe(mA)	0.05	0.2		0.4		0.6		0.6		1.75

		// Frequency dependence of additional current in breakpoint
		// FCY (MHz)	0.5		4		8		10.66	12		32
		// dIpicd2(mA)	0.7		1.0		1.2		1.3		1.3		2.5
		// ICD2 debugger stops in breakpoint

	} while (1); // Main loop

	clr_reset_state(); // Clear uParam and RCON

	for(;;);
	return(0); /* Never return */
}
