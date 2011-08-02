/*
*	MCU Configuration Words
*/
#ifndef _CONFIG_INCL_
#define _CONFIG_INCL_
#include <p24Fxxxx.h> /* PIC24F only */

#ifndef FOSC
#include "fosc.h"	/* Main oscillator frequency FOSC */
#endif

#define FCY_UP2		2000000L /* Default unprogrammed clock */
#define MAIN_IPL	0	/* The value of IPL for main loop */
/*
* Definitions of FCY value in different oscillator modes
*/
#define FCY_PRI	((unsigned long)FOSC)
/*
* Define FCY_PRIPLL for CPDIV = 0
* and select PLLDIV value from FOSC
*/
#if (FOSC > 47990000L) // From 48 to 32 MHz
 #define FCY_RPIPLL		((2*FCY_PRI)/3UL)
 #define PLLDIV_MASK	PLLDIV_DIV12
 #define PLLDIV_N		12
#elif (FOSC > 31990000L)  // 32 to 32 MHz
 #define FCY_PRIPLL		FCY_PRI
 #define PLLDIV_MASK	PLLDIV_DIV8
 #define PLLDIV_N		8
#elif (FOSC > 23990000L)  // 24 to 32 MHz
 #define FCY_PRIPLL		((4*FCY_PRI)/3UL)
 #define PLLDIV_MASK	PLLDIV_DIV6
 #define PLLDIV_N		6
#elif (FOSC > 19990000L)  // 20 to 32 MHz
 #define FCY_PRIPLL		((8*FCY_PRI)/5UL)
 #define PLLDIV_MASK	PLLDIV_DIV5
 #define PLLDIV_N		5
#elif (FOSC > 15990000L)  // 16 to 32 MHz
 #define FCY_PRIPLL		(2UL*FCY_PRI)
 #define PLLDIV_MASK	PLLDIV_DIV4
 #define PLLDIV_N		4
#elif (FOSC > 11990000L)  // 12 to 32 MHz
 #define FCY_PRIPLL		((8*FCY_PRI)/3UL)
 #define PLLDIV_MASK	PLLDIV_DIV3
 #define PLLDIV_N		3
#elif (FOSC > 7990000L)   //  8 to 32 MHz
 #define FCY_PRIPLL		(4UL*FCY_PRI)
 #define PLLDIV_MASK	PLLDIV_DIV2
 #define PLLDIV_N		2
#else // For 4 MHz PLL96MHZ clock input
 #define FCY_PRIPLL		(8UL*FCY_PRI)
 #define PLLDIV_MASK	PLLDIV_NODIV
 #define PLLDIV_N		1
#endif

#define FCY_FRC		8000000UL
#define FCY_FRC16	(FCY_FRC/16UL)
/* Define it for RCDIV = 2 only */
#define FCY_FRCDIV	(FCY_FRC/2UL)
/* Use {RCDIV, PLLDIV} = {2, 1} or {1, 2} only */
#define FCY_FRCPLL	((8UL*FCY_FRCDIV)/PLLDIV_N)

#define FCY_LPRC	31000UL
#define FCY_SOSC	32768UL
/*
* Definitions of Config Words
*/
#define CONFIG1_INIT (\
/* Watchdog Timer: */\
/*	FWDTEN_OFF	&	 Disable */\
	FWDTEN_ON	&	/* (def) Enable */\
/* Windowed WDT: */\
/*	WINDIS_ON	&	Window mode */\
	WINDIS_OFF	&	/* (def) Non-Window mode */\
\
/* WDT Prescaler: */\
/*	FWPSA_PR32	&	 1:32 */\
	FWPSA_PR128	&	/* (def) 1:128 */\
/* Watchdog Timer Postscaler: */\
/*	WDTPS_PS1   	&	 1:1 */\
/*	WDTPS_PS2   	&	 1:2 */\
/*	WDTPS_PS4   	&	 1:4 */\
/*	WDTPS_PS8   	&	 1:8 */\
/*	WDTPS_PS16  	&	 1:16 */\
/*	WDTPS_PS32  	&	 1:32 */\
/*	WDTPS_PS64  	&	 1:64 */\
/*	WDTPS_PS128 	&	 1:128 */\
/*	WDTPS_PS256 	&	 1:256 */\
/*	WDTPS_PS512 	&	 1:512 */\
/*	WDTPS_PS1024	&	 1:1,024 */\
/*	WDTPS_PS2048	&	 1:2,048 */\
/*	WDTPS_PS4096	&	 1:4,096 */\
/*	WDTPS_PS8192	&	 1:8,192 */\
/*	WDTPS_PS16384	&	 1:16,384 */\
	WDTPS_PS32768	&	/* (def) 1:32,768 */\
\
/* Emulator Pin Placement Select bits: */\
/*	ICS_PGx3	&	 Emulator functions are shared with PGEC3/PGED3 */\
/*	ICS_PGx2	&	 Emulator functions are shared with PGEC2/PGED2 */\
	ICS_PGx1	&	/* (def) Emulator functions are shared with PGEC1/PGED1 */\
\
/* General Segment Write Protect: */\
/*	GWRP_ON 	&	 Enabled */\
	GWRP_OFF	&	/* (def) Disabled */\
/* General Segment Code Protect: */\
/*	GCP_ON  	&	 Code protection is enabled */\
	GCP_OFF 	&	/* (def) Code protection is disabled */\
\
/* JTAG Port Enable: */\
/*	JTAGEN_OFF	 Disabled */\
	JTAGEN_ON   /* (def) Enabled */\
) /* #define CONFIG1_INIT */

#define CONFIG2_INIT (\
/* OSCO Pin Configuration: */\
/*	OSCIOFNC_ON  &	 OSCO pin has digital I/O function (RA3) */\
	OSCIOFNC_OFF &	/* (def) OSCO pin has clock out function (CLKO) */\
/* Primary Oscillator Select: */\
/*	POSCMOD_EC   &	 External-Clock Mode Enabled */\
/*	POSCMOD_XT   &	 Oscillator Enabled */\
/*	POSCMOD_HS   &	 HS Oscillator Enabled */\
	POSCMOD_NONE &	/* (def) Primary Oscillator Disabled */\
/* Initial Oscillator Select: */\
/*	FNOSC_FRC    & 	 Fast RC Oscillator (FRC) */\
/*	FNOSC_FRCPLL &	 Fast RC Oscillator with PLL module (FRCPLL) */\
/*	FNOSC_PRI    & 	 Primary Oscillator (XT, HS, EC) */\
/*	FNOSC_PRIPLL &	 Primary Oscillator with PLL module (XTPLL, HSPLL, ECPLL) */\
/*	FNOSC_SOSC   &	 Secondary Oscillator (SOSC) */\
/*	FNOSC_LPRC   &	 Low-Power RC Oscillator (LPRC) */\
	FNOSC_FRCDIV &	/* (def) Fast RC Oscillator with Postscaler (FRCDIV) */\
\
/* 96MHz PLL Startup Select: */\
/*	PLL96MHZ_OFF &	 PLL Startup is enabled by user in software (PLLEN bit) */\
	PLL96MHZ_ON  &	/* (def) PLL Startup is enabled automatically always */\
/* 96 MHz PLL Prescaler Select: */\
/*	PLLDIV_NODIV &	 Oscillator input used directly (4 MHz input) */\
/*	PLLDIV_DIV2  &	 Oscillator input divided by 2 (8 MHz input) */\
/*	PLLDIV_DIV3  &	 Oscillator input divided by 3 (12 MHz input) */\
/*	PLLDIV_DIV4  & 	 Oscillator input divided by 4 (16 MHz input) */\
/*	PLLDIV_DIV5  &	 Oscillator input divided by 5 (20 MHz input) */\
/*	PLLDIV_DIV6  &	 Oscillator input divided by 6 (24 MHz input) */\
/*	PLLDIV_DIV8  &	 Oscillator input divided by 8 (32 MHz input) */\
/*	PLLDIV_DIV12 &	 (def) Oscillator input divided by 12 (48 MHz) */\
	PLLDIV_MASK  &	/* Obtained from FOSC above */\
\
/* Clock Switching and Fail-Safe Clock Monitor: */\
/*	FCKSM_CSECME &	 Sw Enabled, Mon Enabled */\
/*	FCKSM_CSECMD &	 Sw Enabled, Mon Disabled */\
	FCKSM_CSDCMD &	/* (def) Sw Disabled, Mon Disabled (= 10b) */\
/* Unprogrammed value = 11b (can be used for IS_MCU_PROGRAMMED) */\
\
/* IOLOCK One-Way Set Enable: */\
/*	IOL1WAY_OFF  &	 Use the unlock sequence to change the IOLOCK bit */\
	IOL1WAY_ON   &	/* (def) Once set, the IOLOCK bit cannot be cleared */\
\
/* Internal External Switchover: */\
	IESO_OFF	/* Reset IESO (Errata sheet DS80505) */\
/*	IESO_ON 	Default. Don't use with FJDA210 series */\
) /* #define CONFIG2_INIT */

#ifndef IS_MCU_PROGRAMMED
#include "mcu_id.h"
/* The CONFIG2 (and other) is programmed the last */
#define IS_MCU_PROGRAMMED()	(MCU_CONFIG2 != 0xFFFF)
#endif

#define CONFIG3_INIT (\
/* Secondary Oscillator Power Mode Select: */\
/*	SOSCSEL_EC    	&	 External Clock Mode (SCLKI) */\
/*	SOSCSEL_LPSOSC	&	 Low-Power (low gain) Mode */\
	SOSCSEL_SOSC  	&	/* Default (high gain) Mode */\
\
/* Voltage Regulator Wake-up Time Select: */\
/*	WUTSEL_FST		&	 Fast Wake-up Timer */\
	WUTSEL_LEG		&	/* (def) Legacy Wake-up Timer */\
\
/* Segment Write Protection Disable: */\
/*	WPDIS_WPEN		&	 Enabled */\
	WPDIS_WPDIS		&	/* (def) Disabled */\
/* Write Protect Configuration Page Select: */\
/*	WPCFG_WPCFGEN	&	 Enabled */\
	WPCFG_WPCFGDIS	&	/* (def) Disabled */\
/* Segment Write Protection End Page Select: */\
/*	WPEND_WPSTARTMEM &	 Write Protect from page 0 to WPFP */\
	WPEND_WPENDMEM	 &	/* (def) From WPFP to the last page of memory */\
/* Write Protection Flash Page Segment Boundary: */\
/*	WPFP_WPFP0  	 Page 0 (0x00) */\
/*	WPFP_WPFPnnn	 Page nnn (nnn X 0x400) : */\
/*	WPFP_WPFPxxx	 Select needed page here */\
	WPFP_WPFP255	/* (def) Highest Page (same as page 170) */\
) /* #define CONFIG3_INIT */

#endif /*_CONFIG_INCL_*/
