/*
* Redefinition of default MCU pins configuration
*/
#ifndef _PINSCFG_INCL_
#define _PINSCFG_INCL_

#include <pins.h>
/*
*	PORT E
*/
#define RE0_LOW
#define RE1_LOW
#define RE2_LOW
#define RE3_LOW					// SD1-SD4 output pins
#define RE4_LOW					// SCK output pin
#define SD1			_LATE0		// C definitions
#define SD2			_LATE1
#define SD3			_LATE2
#define SD4			_LATE3
#define SCK			_LATE4

#define RE5_HIGH				// SPI2 ~CS pin
#define CS2			_LATE5		// C definition

#define RE6_IN					// I2C3 SCL pin
#define RE7_IN					// I2C3 SDA pin
/*
*	PORT G
*/
#define RG6_IN					// 
#define RG7_HIGH
#define RG8_HIGH
/*
*	PORT B
*/
// UART2 TxD, RxD, ~INVALID and ~SHDN pins (RB8-RB11 pins)
// To use RB8-11,12,13 JTAG must be off, check CONFIG1 word
//#define RB8_HIGH	// TxD output (To RS-232 driver Tin pin)
//#define RP8_OUT		RP_U2TX // Map UART1 TxD output to RB8/RP8 pin
//
//#define RB9_IN		// RxD input (From RS-232 driver Rout pin)
//#define RP_U2RX		RP9_IN // Map UART1 TxD input to RB9/RP9 pin
//
//#define RB10_IN		// ~INVALID input signal from RS-232 driver
//#define _U2_VALID	_RB10 // C definitions of ~INVALID signal
//
//#define RB11_LOW	// ~SHDN output signal to RS-232 driver
//#define _U2_SHDN	_LATB11 // C definitions of ~SHDN signal
// UART2

//#define RB15_HIGH			// Test pin (and REFO output)
//#define _REFO		_RB15	// C definitions of REFO pin
/*
*	PORT D
*/
// SPI1 Master Mode (SDO - RD0, SDI - RD2, SCK - RD11)
//#define RD0_LOW		// SDO output (to DataFlash SI input)
//#define RP11_OUT	RP_SDO1 // Map SDO1 to RD0/RP11 pin
//
//#define RD2_IN		// SDI input (from DataFlash SO output)
//#define RP_SDI1		RP23_IN // Map SDI1 to RD2/RP23 pin
//
//#define RD11_HIGH	// SCK output (to DataFlash SCK input)
//#define RP12_OUT	RP_SCK1OUT // Map SCK1 to RD11/RP12 pin
// SPI1
/*
*	PORT E
*/
//#define RE0_HIGH	// ~CS0 (Chip Select 0) output pin
//#define _CS0		_LATE0 // C definition of ~CS0 pin

#endif /*_PINSCFG_INCL_*/
