/*
*	Store RCON in user_init function and clear all it status
*	bits. Unimplemented RCON bits is used to save perameter.
*/
#include <reset.h>

static unsigned int __attribute__((persistent)) reset_state;

void do_reset(unsigned int uParam)
{
	reset_state = uParam; /* Save unused RCON bits */
	__asm__ volatile ("reset");
}
/*
*	The status bits in the RCON register should be cleared
*	after they are read so that the next RCON register
*	value after a device Reset will be meaningful.
*/
void __attribute__((user_init)) reset_init(void)
{
#ifdef __DEBUG /*	--- ERROR ---
	You must always reset IESO in _CONFIG2(), insert in code
	_CONFIG2(IESO_OFF); // Reset IESO (Errata sheet DS80505) */
	if ((MCU_CONFIG2 & ~IESO_OFF) != 0) while(1); // Stop here
#endif

	/* May be we fail to take POR into account */
	if (RCON == (RCON_POR|RCON_BOR | RCON_EXTR))
		RCONbits.EXTR = 0; /* Then skip MCLR */
		 /* That is valid after programming */

	/* At power-on RCON reset value =3, in other case */
	if (RCON != (RCON_POR|RCON_BOR)) /* it's not POR */
		RCONbits.POR = 0; /* POR may be set on BOR */
		/* and MCLR reset, clear POR bit manually */
	else RCONbits.BOR = 0; /* BOR bit is also set*/
	/* at power-on reset. Clear it as minor bit */

	/* Initialize reset_state value only at POR or BOR */
	if ((RCON & (RCON_POR|RCON_BOR)) != 0) reset_state = 0;

	reset_state &= RCON_UNUSED; /* Clear all except uParam */
	reset_state |= RCON & RCON_BITS; /* Save all used bits */
	RCON &= ~RCON_STATUS; /* Then clear all status bits */
}

unsigned int get_reset_state(void)
{
	/* Function may be called from user_init section */
	/* before reset_init() has been called, or after */
	/* Idle or Sleep mode has been selected - reinit */
	if ((RCON & RCON_STATUS) != 0) reset_init();
	return(reset_state);
}
