/*
* Redefinition of default MCU pins configuration
*/
#ifndef _PINSCFG_INCL_
#define _PINSCFG_INCL_

#include <pins.h>
/*
*	PORT B
*/
// UART2 TxD, RxD, ~INVALID and ~SHDN pins (RB8-RB11 pins)
// To use RB8-11,12,13 JTAG must be off, check CONFIG1 word
#define RB8_HIGH	// TxD output (To RS-232 driver Tin pin)
#define RP8_OUT		RP_U2TX // Map UART1 TxD output to RB8/RP8 pin
//
#define RB9_IN		// RxD input (From RS-232 driver Rout pin)
#define RP_U2RX		RP9_IN // Map UART1 TxD input to RB9/RP9 pin
//
#define RB10_IN		// ~INVALID input signal from RS-232 driver
#define _U2_VALID	_RB10 // C definitions of ~INVALID signal
//
#define RB11_LOW	// ~SHDN output signal to RS-232 driver
#define _U2_SHDN	_LATB11 // C definitions of ~SHDN signal
// UART2

#define RB15_HIGH			// Test pin (and REFO output)
#define _REFO		_RB15	// C definitions of REFO pin
/*
*	PORT D
*/
// SPI1 Master Mode (SDO - RD0, SDI - RD2, SCK - RD11)
#define RD0_LOW		// SDO output (to DataFlash SI input)
#define RP11_OUT	RP_SDO1 // Map SDO1 to RD0/RP11 pin
//
#define RD2_IN		// SDI input (from DataFlash SO output)
#define RP_SDI1		RP23_IN // Map SDI1 to RD2/RP23 pin
//
#define RD11_HIGH	// SCK output (to DataFlash SCK input)
#define RP12_OUT	RP_SCK1OUT // Map SCK1 to RD11/RP12 pin
// SPI1
/*
*	PORT E
*/
#define RE0_HIGH	// ~CS0 (Chip Select 0) output pin
#define _CS0		_LATE0 // C definition of ~CS0 pin

//------------- End of pin redefinition -------------
/*
* Used pins (defined as input, output or analog pin)
*/
#define PIN_USED(port, n)\
	(defined(R##port##n##_LOW) || defined(R##port##n##_HIGHT)\
	|| defined(R##port##n##_IN) || defined(R##port##n##_AN))
/*
* Used SPI modules
*/
#if defined(RP_SDI1)
#define SPI1_USED
#endif
#if defined(RP_SDI2)
#define SPI2_USED
#endif
#if defined(RP_SDI3)
#define SPI3_USED
#endif
/*
* Used UART modules, ~SHDN and ~INVALID pins
*/
#if defined(RP_U1RX)
#define UART1_USED		// UART1 is used
#ifndef	_U1_VALID		// No ~INVALID pin
#define	_U1_VALID	1	// Always valid
#endif
#ifndef	_U1_SHDN		// No ~SHDN pin
#define	_U1_SHDN	1	// Always wakeup
#define	U1_SHDN_SET()	((void)0)
#define	U1_SHDN_CLR()	((void)0)
#else
#define	U1_SHDN_SET()	(_U1_SHDN = 1)
#define	U1_SHDN_CLR()	(_U1_SHDN = 0)
#endif // U1_SHDN
#endif // RP_U1RX

#if defined(RP_U2RX)
#define UART2_USED		// UART2 is used
#ifndef	_U2_VALID		// No ~INVALID pin
#define	_U2_VALID	1	// Always valid
#endif
#ifndef	_U2_SHDN		// No ~SHDN pin
#define	_U2_SHDN	1	// Always wakeup
#define	U2_SHDN_SET()	((void)0)
#define	U2_SHDN_CLR()	((void)0)
#else
#define	U2_SHDN_SET()	(_U2_SHDN = 1)
#define	U2_SHDN_CLR()	(_U2_SHDN = 0)
#endif // U2_SHDN
#endif // RP_U2RX

#if defined(RP_U3RX)
#define UART3_USED		// UART3 is used
#ifndef	_U3_VALID		// No ~INVALID pin
#define	_U3_VALID	1	// Always valid
#endif
#ifndef	_U3_SHDN		// No ~SHDN pin
#define	_U3_SHDN	1	// Always wakeup
#define	U3_SHDN_SET()	((void)0)
#define	U3_SHDN_CLR()	((void)0)
#else
#define U3_SHDN_SET()	(_U3_SHDN = 1)
#define U3_SHDN_CLR()	(_U3_SHDN = 0)
#endif // U3_SHDN
#endif // RP_U3RX

#if defined(RP_U4RX)
#define UART4_USED		// UART4 is used
#ifndef	_U4_VALID		// No ~INVALID pin
#define	_U4_VALID	1	// Always valid
#endif
#ifndef	_U4_SHDN		// No ~SHDN pin
#define	_U4_SHDN	1	// Always wakeup
#define	U4_SHDN_SET()	((void)0)
#define	U4_SHDN_CLR()	((void)0)
#else
#define	U4_SHDN_SET()	(_U4_SHDN = 1)
#define	U4_SHDN_CLR()	(_U4_SHDN = 0)
#endif // U4_SHDN
#endif // RP_U4RX

#endif /*_PINSCFG_INCL_*/
