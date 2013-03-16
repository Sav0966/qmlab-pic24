/*
*	MCU pin configuration functions
*
* In <config.h> :
* 1) Use macroses to redefine pin configuration
*	#define Rxn_IN		// Pin Rxn is input
*	#define Rxn_LOW		// Pin Rxn is output in low state
*	#define Rxn_HIGH	// Pin Rxn is output in high state
*	#define Rxn_AN		// Pin Rxn is analog input
* Rx - ports RA, RB, RC, RD, RE, RF, RG; n - pin number
*
* 2) Use macros to define remapable output function
*	#define RPn_OUT	RP_source	// Pin RPn is <source> output
* RPn - remapable pins RP0-RP31 (RP5,15,30,31 for 100-pin devices)
*
* 3) Use macroses to define remapable input function
*	#define RP_source	RPn_IN	// Pin RPn is <source> input
*	#define RP_source	RPIn	// Pin RPIn is <source> input
* RPn - remapable pins RP0-RP31 (RP5,15,30,31 for 100-pin devices)
* RPIn - remapable pins RPI32-RPI43 (for 64-pin devices only RPI37)
*
* Sources:
*	CxOUT	- Comparator 1-3 Output
*
*	UxTX	- UART 1-4 Transmit Output
*	UxRTS	- UART 1-4 Request To Send Output
*	UxCTS	- UART 1-4 Clear To Send Input
*	UxRX	- UART 1-4 Receive Input
*
*	SDOx	- SPI 1-3 Data Output
*	SCKxOUT	- SPI 1-3 Clock Output
*	SSxOUT	- SPI 1-3 Slave Select Output
*	SSxIN	- SPI 1-3 Slave Select Input
*	SCKxIN	- SPI 1-3 Clock Input
*	SDIx	- SPI 1-3 Data Input
*
*	OCx		- Output Compare 1-9 Ouput
*	OCFx	- Output Compare Fault A-B Input
*
*	ICx		- Input Capture 1-9 Input
*
*	INTx	- External Interrupt 1-4 Input
*
*	TxCK	- Timer 2-5 External Clock
*
*	NULL	- No output function or no input pin
*/
#ifndef _PINS_INCL_
#define _PINS_INCL_
#include <p24Fxxxx.h> /* PIC24F only */
/*
* MCU pins configuration function
*/
void pins_init(void);
/*
* Default MCU pins configuration
*/
/*
* PORTA (100-pins only)
*/
// #define RA0_IN
// #define RA1_IN
// #define RA2_IN
// #define RA3_IN
// #define RA4_IN
// #define RA5_IN
// #define RA6_AN
// #define RA7_AN
// #define RA9_AN
// #define RA10_AN
// #define RA14_IN
// #define RA15_IN
/*
* PORTB
*/
//#define RB0_AN
//#define RB1_AN
//#define RB2_AN
//#define RB3_AN
//#define RB4_AN
//#define RB5_AN
//#define RB6_AN
//#define RB7_AN
//#define RB8_AN
//#define RB9_AN
//#define RB10_AN
//#define RB11_AN
//#define RB12_AN
//#define RB13_AN
//#define RB14_AN
//#define RB15_AN
/*
* PORTC
*/
// 100-pins only
// #define RC1_IN
// #define RC2_IN
// #define RC3_IN
// #define RC4_AN

//#define RC12_IN
//#define RC13_AN
//#define RC14_AN
//#define RC15_IN
/*
* PORTD
*/
//#define RD0_IN
//#define RD1_IN
//#define RD2_IN
//#define RD3_IN
//#define RD4_IN
//#define RD5_IN
//#define RD6_AN
//#define RD7_AN
//#define RD8_IN
//#define RD9_IN
//#define RD10_IN
//#define RD11_IN

// 100-pins only
// #define RD12_IN
// #define RD13_IN
// #define RD14_IN
// #define RD15_IN
/*
* PORTE
*/
//#define RE0_IN
//#define RE1_IN
//#define RE2_IN
//#define RE3_IN
//#define RE4_IN
//#define RE5_IN
//#define RE6_IN
//#define RE7_IN

// 100-pins only
// #define RE8_IN
// #define RE9_AN
/*
* PORTF
*/
//#define RF0_AN
//#define RF1_IN

// 100-pins only
// #define RF2_IN

//#define RF3_IN
//#define RF4_IN
//#define RF5_IN
//#define RF7_IN

// 100-pins only
// #define RF8_IN
// #define RF12_IN
// #define RF13_IN
/*
* PORTG
*/
// 100-pins only
// #define RG0_IN
// #define RG1_IN

//#define RG2_IN
//#define RG3_IN
//#define RG6_AN
//#define RG7_AN
//#define RG8_AN
//#define RG9_AN

// 100-pins only
// #define RG12_IN
// #define RG13_IN
// #define RG14_IN
// #define RG15_IN
/*
* Selectable output sources (maps function to output)
*/
#define RP_NULL			0	// NULL (defaul function)
#define RP_C1OUT		1	// Comparator 1 Output
#define RP_C2OUT		2	// Comparator 2 Output
#define RP_U1TX			3	// UART1 Transmit
#define RP_U1RTS		4	// UART1 Request To Send
#define RP_U2TX			5	// UART2 Transmit
#define RP_U2RTS		6	// UART2 Request To Send
#define RP_SDO1			7	// SPI1 Data Output
#define RP_SCK1OUT		8	// SPI1 Clock Output
#define RP_SS1OUT		9	// SPI1 Slave Select Output
#define RP_SDO2			10	// SPI2 Data Output
#define RP_SCK2OUT		11	// SPI2 Clock Output
#define RP_SS2OUT		12	// SPI2 Slave Select Output
#define RP_OC1			18	// Output Compare 1
#define RP_OC2			19	// Output Compare 2
#define RP_OC3			20	// Output Compare 3
#define RP_OC4			21	// Output Compare 4
#define RP_OC5			22	// Output Compare 5
#define RP_OC6			23	// Output Compare 6
#define RP_OC7			24	// Output Compare 7
#define RP_OC8			25	// Output Compare 8
#define RP_U3TX			28	// UART3 Transmit
#define RP_U3RTS		29	// UART3 Request To Send
#define RP_U4TX			30	// UART4 Transmit
#define RP_U4RTS		31	// UART4 Request To Send
#define RP_SDO3			32	// SPI3 Data Output
#define RP_SCK3OUT		33	// SPI3 Clock Output
#define RP_SS3OUT		34	// SPI3 Slave Select Output
#define RP_OC9			35	// Output Compare 9
#define RP_C3OUT		36	// Comparator 3 Output
/*
* Selectable input pins (pin numbers)
*/
#define RP0_IN		0
#define RP1_IN		1
#define RP2_IN		2
#define RP3_IN		3
#define RP4_IN		4
#define RP6_IN		6
#define RP7_IN		7
#define RP8_IN		8
#define RP9_IN		9
#define RP10_IN		10
#define RP11_IN		11
#define RP12_IN		12
#define RP13_IN		13
#define RP14_IN		14
#define RP16_IN		16
#define RP17_IN		17
#define RP18_IN		18
#define RP19_IN		19
#define RP20_IN		20
#define RP21_IN		21
#define RP22_IN		22
#define RP23_IN		23
#define RP24_IN		24
#define RP25_IN		25
#define RP26_IN		26
#define RP27_IN		27
#define RP28_IN		28
#define RP29_IN		29

#define RPI37		37

#ifdef _ANSA6
// (100-pin devices only)
 #define RP5_IN			5
 #define RP15_IN		15
 #define RP30_IN		30
 #define RP31_IN		31
 #define RPI32			32
 #define RPI33			33
 #define RPI34			34
 #define RPI35			35
 #define RPI36			36
 #define RPI38			38
 #define RPI39			39
 #define RPI40			40
 #define RPI41			41
 #define RPI42			42
 #define RPI43			43
#endif

#define RPI_NULL	0x3F	// No pin to input function

#endif /*_PINS_INCL_*/
