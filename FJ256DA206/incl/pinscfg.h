/*
* Redefinition of default MCU pins configuration
*/
#ifndef _PINSCFG_INCL_
#define _PINSCFG_INCL_

#include <pins.h>

// PORT B
// UART2 TxD, RxD, ~INVALID and ~SHDN pins (RB8-RB11 pins)
// To use RB8-11,12,13 JTAG must be off, check CONFIG1 word
#define RB8_HIGH	// TxD output (To RS-232 driver Tin pin)
#define RP8_OUT		RP_U2TX // Map UART1 TxD output to RB8/RP8 pin
#define RB9_IN		// RxD input (From RS-232 driver Rout pin)
#define RP_U2RX		RP9_IN // Map UART1 TxD input to RB9/RP9 pin
#define RB10_IN		// ~INVALID input signal from RS-232 driver
#define U2_VALID	_RB10 // C definitions of ~INVALID signal
#define RB11_LOW	// ~SHDN output signal to RS-232 driver
#define U2_SHDN		_LATB11 // C definitions of ~SHDN signal

#define RB15_HIGH	// Test pin (and REFO output)

// PORT D
// SPI1 Master Mode (SDO - RD0, SDI - RD2, SCK - RD11)
#define RD0_LOW		// SDO output (to DataFlash SI input)
#define RP11_OUT	RP_SDO1 // Map SDO1 to RD0/RP11 pin
#define RD2_IN		// SDI input (from DataFlash SO output)
#define RP_SDI1		RP23_IN // Map SDI1 to RD2/RP23 pin
#define RD11_HIGH	// SCK output (to DataFlash SCK input)
#define RP12_OUT	RP_SCK1OUT // Map SCK1 to RD11/RP12 pin

#endif /*_PINSCFG_INCL_*/
