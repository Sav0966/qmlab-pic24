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
#define RF0_LOW		// RS-232 driver ~SHDN pin
#define _U2_SHDN	_LATF0	// C definition

#define RF1_LOW		// DC/DC module ~SHDN pin
#define _PW_SHDN	_LATF1	// C definition

#define RF3_IN		// Power Good signal (input and INT)
#define RP_INT1		RP16_IN	// Map INT1 to RF3/RP16 pin
#define _PWGOOD		_RF3	// C definition

#define RF4_IN		// Input frequency signals
#define RF5_IN		//		Input capture modules:
#define RP_IC4		RP10_IN	// IC4 - RF4/RP10 pin
#define RP_IC3		RP17_IN	// IC3 - RF5/RP17 pin

#define RF7_IN		// VBUS signal (USB module)
//#define _VBUS		_RF7	// C definition
/*
*	PORT C
*/
#define RC12_IN		// CLKI input (external main oscillator)
#define RC15_HIGH	// OSCO pin has digital output function (RC15)
#define _OSCOFF		_LATC15	// C definition (initially turned off)

#define RC13_IN		// SOSCI and SOSCO
#define RC14_IN
/*
*	PORT D
*/
#define RD1_IN		// RxD2 input (From RS-232 driver Rout pin)
#define RD2_HIGH	// TxD2 output (To RS-232 driver Tin pin)
#define RP23_OUT	RP_U2TX // Map TxD2 to RD2/RP23 pin
#define RP_U2RX		RP24_IN // Map RxD to RD1/RP24 pin

#define RD3_IN		// SPI1 - interface to external Flash
#define RD4_HIGH	
#define RD5_HIGH
#define RP_SDI1		RP22_IN 	// Map SDI1 to RD3/RP22 pin
#define RP25_OUT	RP_SDO1		// Map SDO1 to RD4/RP25 pin
#define RP20_OUT	RP_SCK1OUT	// Map SCK1 to RD5/RP20 pin

#define RD6_HIGH	// SPI1 ~CS pin (see RD3-5)
#define _CS1		_LATD6		// C definition

#define RD7_LOW		// ~RESET pin (GPS and Flash)
#define _RST		_LATD7		// C definition

#define RD8_IN		// Input frequency signals
#define RD9_IN		//		Input capture modules:
#define RP_IC2		RP2_IN	// IC2 - RD8/RP2 pin
#define RP_IC1		RP4_IN	// IC3 - RD9/RP4 pin

#define RD0_IN		// RxD3 input (From GPS module Rout pin)
#define RD10_HIGH	// TxD3 output (To GPS module Tin pin)
#define RP3_OUT		RP_U3TX // Map TxD3 to RD10/RP3 pin
#define RP_U3RX		RP11_IN // Map RxD3 to RD0/RP11 pin

#define RD11_IN		// PPS signal (from GPS module, INT)
#define RP_INT2		RP12_IN	// Map INT2 to RD11/RP12 pin

#endif /*_PINSCFG_INCL_*/
