/*
*	Generate some traps
*/
#include <p24Fxxxx.h> /* PIC24F only */
#include <mcu_id.h>
#include <reset.h>
#include <pm.h>
#include "main.h"

#define OSC_TRAP	0x2000	/* Unused RCON bit 13 */
#define MEM_TRAP	0x1000	/* Unused RCON bit 12 */
#define STACK_TRAP	0x0800	/* Unused RCON bit 11 */
#define MATH_TRAP	0x0400	/* Unused RCON bit 10 */

void __attribute__((section(".init")))
_move_reset_to_second_flash_page(void)
{
	__asm__ volatile(".space (256-2)*2");
	/* We want to erase first page - */
	/* move '_reset' label to 0x400 */
	for(;;); // Deed stop
}

static void mem_access_trap(void)
{
	__asm__ volatile ("mov #0x1001, w0");
	__asm__ volatile ("mov [w0], w0");
}

static void stack_ovf_trap(void)
{
	__asm__ volatile ("mov SPLIM, w1"); /* Save SPLIM */

	__asm__ volatile ("disi #2"); /* A write to the SPLIM register should */
	__asm__ volatile ("mov	w15, SPLIM"); /* not be immediately followed */
	__asm__ volatile ("nop"); /* by an indirect read operation using W15 */

	__asm__ volatile ("push	w0"); /* Compare SP and SPLIM only */
	__asm__ volatile ("push	w0"); /* Trap will be taken here */
	__asm__ volatile ("pop	w0"); /* Never be here*/
	__asm__ volatile ("pop	w0");

	__asm__ volatile ( /* Really, it's not be called ever */
	"disi #2\n mov w1, SPLIM\n nop"); /* Restore SPLIM */
}

static void math_error_trap(void) { int i = 0; i /= i; }

void illegal_opcode_func(void); /* trap_err_.s */
static void illegal_op_reset(void) { illegal_opcode_func(); }

static char __attribute__((persistent)) buf[PM_PAGE_SIZE];

static void unhandled_trap_reset(void)
{
	char tmp[3];
	/* _reset label must be at 0x400 */
	if (pm_read_pword(0) != 0x40400) return;

	pm_read_page(0L, buf); /* Read first flash page */
	/* Store _AddressError() interrupt vector in tmp[] */
	tmp[0] = buf[12]; tmp[1] = buf[13]; tmp[2] = buf[14];
	/* Then erase _AddressError() interrupt vector */
	buf[12] = 255; buf[13] = 255; buf[14] = 255;
	_erase_flash(0L); /* Can't use pm_esarse_ */
	pm_write_page(0L, buf); /* Save new page */
	/* Restore _AddressError() in buf[] from tmp[] */
	buf[12] = tmp[0]; buf[13] = tmp[1]; buf[14] = tmp[2];

	if (mcu_id != 0) /* For hardware only */
	{ mem_access_trap(); } /* - call trap */
	else /* Simulate trap for SIM debugger */
	{ RCONbits.TRAPR = 1; __asm__ ("reset"); }
}

static int get_cpu_ipl(void)
{
	int iRet = SRbits.IPL;
	if (CORCONbits.IPL3 != 0) iRet |= 8;
	return(iRet); /* Current CPU IPL */
}

static int cur_ipl; /* Current CPU IPL */
static int __attribute__((persistent)) osc_ipl;
static int __attribute__((persistent)) addr_ipl;
static int __attribute__((persistent)) stack_ipl;
static int __attribute__((persistent)) math_ipl;

void check_traps(void)
{
	cur_ipl = get_cpu_ipl();
	if ((rst_events & RCON_UNUSED) == 0)
	{ /* Is called first time */
		osc_ipl = 0; addr_ipl = 0;
		stack_ipl = 0; math_ipl = 0;
	}

	/* Call it first to prevent init '_ipl' twice */
	if (!(rst_state & MATH_TRAP)) math_error_trap();
	if (!(rst_state & STACK_TRAP)) stack_ovf_trap();
	if (!(rst_state & MEM_TRAP)) mem_access_trap();
//	if (!(rst_state & OSC_TRAP)) osc_fail_trap();

//	if (!(rst_events & RCON_CM)) config_mismatch_reset();
	if (!(rst_events & RCON_IOPUWR)) illegal_op_reset();
	if (!(rst_events & RCON_TRAPR)) unhandled_trap_reset();
	else /* Restore trap vector and view IPLs in debbuger */
	{
		if (pm_read_pword(8) < 0)
		{
			/* Restore value of erased trap vector */
			if (mcu_id != 0) /* For hardware only */
			{ pm_write_pword(8, *((long*)&buf[12])); }
			else /* For SIM debugger rewrite all page */
			{ _erase_flash(0L); pm_write_page(0L, buf); }
		}

		/* SIM doesn't'n clear SRbits.IPL after do_reset() */
		/* ICD2 clear it, releas -> ?. Set IPL manually */
		if (cur_ipl != 0) cur_ipl = 0; /* in main() */

//		if (osc_ipl != 0xE) osc_ipl = 0xE; // Untested
		if (addr_ipl != 0xD) addr_ipl = 0xD; 
		if (stack_ipl != 0xC) stack_ipl = 0xC; 
		if (math_ipl != 0xB) math_ipl = 0xB; 
	}
}

/* Trap handlers, reset INTCON1 bits by hand for example */
void __attribute__((__interrupt__, auto_psv))
_OscillatorFail(void) { osc_ipl = get_cpu_ipl(); INTCON1bits.OSCFAIL = 0;
		do_reset(get_reset_state() | OSC_TRAP); } // Reset OSCFAIL bit

void __attribute__((__interrupt__, auto_psv))
_AddressError(void) { addr_ipl = get_cpu_ipl(); INTCON1bits.ADDRERR = 0;
		do_reset(get_reset_state() | MEM_TRAP); } // Reset ADDRERR bit

void __attribute__((__interrupt__, auto_psv))
_StackError(void) { stack_ipl = get_cpu_ipl(); INTCON1bits.STKERR = 0;
		do_reset(get_reset_state() | STACK_TRAP); } // Reset STKERR bit

void __attribute__((__interrupt__, auto_psv))
_MathError(void) { math_ipl = get_cpu_ipl(); INTCON1bits.MATHERR = 0;
		do_reset(get_reset_state() | MATH_TRAP); } // Reset MATHERR bit
