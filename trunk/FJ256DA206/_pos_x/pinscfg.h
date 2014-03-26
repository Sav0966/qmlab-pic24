/*
* Redefinition of default MCU pins configuration
*/
#ifndef _PINSCFG_INCL_
#define _PINSCFG_INCL_

#include <pins.h>
/*
*	PORT E
*/
#define RE0_LOW		// Serial interface to external
#define RE1_LOW		// hardware (SCK and SD1-4 lines)
#define RE2_LOW
#define RE3_LOW					// SD1-SD4 output pins
#define RE4_LOW					// SCK output pin
#define _SD1		_LATE0		// C definitions
#define _SD2		_LATE1
#define _SD3		_LATE2
#define _SD4		_LATE3
#define _SCK		_LATE4

#define RE6_IN					// I2C3 SCL pin
#define RE7_IN					// I2C3 SDA pin

#define RE5_HIGH				// SPI2 ~CS pin (see RG6-8)
#define _CS2		_LATE5		// C definition
/*
*	PORT G
*/
#define RG2_IN		// D+/D- lines (USB module)
#define RG3_IN

#define RG6_IN		// SPI2 - interface to external SD card
#define RG7_HIGH	
#define RG8_HIGH
#define RP_SDI2		RP21_IN 	// Map SDI2 to RG6/RP21 pin
#define RP26_OUT	RP_SCK2OUT	// Map SCK2 to RG7/RP26 pin
#define RP19_OUT	RP_SDO2		// Map SDO2 to RG8/RP19 pin

#define RG9_LOW		// Common control line (polarization)
#define _POL		_LATG9		// C definition
/*
*	PORT B
*/
#define RB0_IN		// PGEC1 programming pin
#define RB1_IN		// PGED1 programming pin

#define RB3_AN		// Input power voltage (PW_EN wire)

#define RB2_LOW		// Separate control lines (synchronization)
#define RB4_LOW
#define RB5_LOW
#define RB6_LOW
#define _SYNC3		_LATB2		// C definitions
#define _SYNC2		_LATB4
#define _SYNC1		_LATB5
#define _SYNC4		_LATB6

#define RB7_IN		// Input validation lines
#define RB8_IN
#define RB9_IN
#define RB14_IN
#define _AS1		_RB7	// C definitions
#define _AS2		_RB8
#define _AS3		_RB9
#define _AS4		_RB14

#define RB10_AN		// Input analog signals
#define RB11_AN
#define RB12_AN
#define RB13_AN

#define RB15_IN		// REFO line (test line)
#define _REFO		_RB15	// C definition
/*
*	PORT F
*/
#define RF3_IN		// Power Good signal (INT)
#define _PWGOOD		_RF3	// C definition

#define RF4_IN		// Input frequency signals
#define RF5_IN		//		Input capture modules:
#define RP_IC4		RP10_IN // IC4 - RF4/RP10 pin
#define RP_IC3		RP17_IN // IC3 - RF5/RP17 pin

#define RF7_IN		// VBUS signal (USB module)
//#define _VBUS		_RF7	// C definition
/*
*	PORT C
*/

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
