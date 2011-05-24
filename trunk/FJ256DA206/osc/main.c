#include "main.h"
#include "reset.h"

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
	SRbits.IPL = MAIN_IPL; /* Set default by hands */

	mcu_id = MCU_ID;
	if (isPOWER_ON(rst_state = get_reset_state()))
		power_on_init(); /*	For POR, BOR and MCLR */

	rst_events |= rst_state; /* As RCON register */
	++rst_num; /* Calculate session reset number */

	clr_reset_state(); // Clear uParam and RCON

	for(;;);
	return(0); /* Never return */
}
