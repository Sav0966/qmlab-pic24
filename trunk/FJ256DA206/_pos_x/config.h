/*
*	MCU Configuration Words
*/
#ifndef _CONFIG_INCL_
#define _CONFIG_INCL_
#include <p24Fxxxx.h> /* PIC24F only */
/*
* The value of oscillator mode
*/
#ifndef __OSC__
#define __OSC__		PRIPLL
#endif
#include <oscdef.h>
#include <fcy.h>
/*
* Pin definition
*/
#include <pinscfg.h>
/*
* Interrupt priority levels (IPL)
*/
#define MAIN_IPL	0	// Main loop
#define SYSCLK_IPL	1	// System timer
#define RTC_IPL		2	// RTC interface
#define UART_IPL	3	// System COM
#define GPS_IPL		3	// GPS module COM
#define SD_IPL		4	// SD card SPI
#define FL_IPL		4	// Flash SPI
#define PPS_IPL		6	// PPS signal
#define PWG_IPL		7	// Power Good
/*
* System modules
*/
#define SYS_TIMER	1	// System timer
#define RTC_I2C		3	// RTC interface
#define SD_SPI		2	// SD card SPI
#define FL_SPI		1	// Flash SPI
#define DBG_UART	1	// Debug COM
#define SYS_UART	2	// System COM
#define GPS_UART	3	// GPS module COM
#define PW_INT		1	// INT1 to PW_GOOG
#define PPS_INT		2	// INT2 to PPS signal
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
	JTAGEN_OFF	/* Disabled */\
/*	JTAGEN_ON    (def) Enabled */\
) /* #define CONFIG1_INIT */

// Reset IESO bit (Errata DS80505)
// Use OSCO/RC15 pin as digital IO to turn on/off
//  external ocsillator (low -> on), wich is connected to OSCI
// Enable primary oscillator and select External-Clock mode
// Clock switching is enabled, fail-safe monitor - enabled
// Manage PLL96MHZ by software (PLLEN bit), reset state -> off

#define CONFIG2_INIT (\
/* OSCO Pin Configuration: */\
	OSCIOFNC_ON  &	/* OSCO pin has digital I/O function (RA3) */\
/*	OSCIOFNC_OFF &	 (def) OSCO pin has clock out function (CLKO) */\
/* Primary Oscillator Select: */\
	POSCMOD_EC   &	/* External-Clock Mode Enabled */\
/*	POSCMOD_XT   &	 Oscillator Enabled */\
/*	POSCMOD_HS   &	 HS Oscillator Enabled */\
/*	POSCMOD_NONE &	 (def) Primary Oscillator Disabled */\
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
	PLL96MHZ_OFF &	/* PLL Startup is enabled by user in software (PLLEN bit) */\
/*	PLL96MHZ_ON  &	 (def) PLL Startup is enabled automatically always */\
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
	FCKSM_CSECME &	/* Sw Enabled, Mon Enabled */\
/*	FCKSM_CSECMD &	 Sw Enabled, Mon Disabled */\
/*	FCKSM_CSDCMD &	 (def) Sw Disabled, Mon Disabled (= 10b) */\
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
#include <mcu_id.h>
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
