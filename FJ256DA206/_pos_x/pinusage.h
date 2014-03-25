/*
* Used pins (defined as input, output or analog pin)
*/
#ifndef _PINUSAGE_INCL_
#define _PINUSAGE_INCL_

#include <pinscfg.h>

#define PIN_USED(port, n)\
	(defined(R##port##n##_LOW) || defined(R##port##n##_HIGHT)\
	|| defined(R##port##n##_IN) || defined(R##port##n##_AN))
/*
* Used Input Captur modules
*/
#if defined(RP_IC1)
#define IC1_USED
#endif
#if defined(RP_IC2)
#define IC2_USED
#endif
#if defined(RP_IC3)
#define IC3_USED
#endif
#if defined(RP_IC4)
#define IC4_USED
#endif
#if defined(RP_IC5)
#define IC5_USED
#endif
#if defined(RP_IC6)
#define IC6_USED
#endif
#if defined(RP_IC7)
#define IC7_USED
#endif
#if defined(RP_IC8)
#define IC8_USED
#endif
#if defined(RP_IC9)
#define IC9_USED
#endif
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
* Used UART modules
*/
#if defined(RP_U1RX)
#define UART1_USED		// UART1 is used
//#ifndef	_U1_VALID		// No ~INVALID pin
//#define	_U1_VALID	1	// Always valid
//#endif
//#ifndef	_U1_SHDN		// No ~SHDN pin
//#define	_U1_SHDN	1	// Always wakeup
//#define	U1_SHDN_SET()	((void)0)
//#define	U1_SHDN_CLR()	((void)0)
//#else
//#define	U1_SHDN_SET()	(_U1_SHDN = 1)
//#define	U1_SHDN_CLR()	(_U1_SHDN = 0)
//#endif // U1_SHDN
#endif // RP_U1RX

#if defined(RP_U2RX)
#define UART2_USED		// UART2 is used
//#ifndef	_U2_VALID		// No ~INVALID pin
//#define	_U2_VALID	1	// Always valid
//#endif
//#ifndef	_U2_SHDN		// No ~SHDN pin
//#define	_U2_SHDN	1	// Always wakeup
//#define	U2_SHDN_SET()	((void)0)
//#define	U2_SHDN_CLR()	((void)0)
//#else
//#define	U2_SHDN_SET()	(_U2_SHDN = 1)
//#define	U2_SHDN_CLR()	(_U2_SHDN = 0)
//#endif // U2_SHDN
#endif // RP_U2RX

#if defined(RP_U3RX)
#define UART3_USED		// UART3 is used
//#ifndef	_U3_VALID		// No ~INVALID pin
//#define	_U3_VALID	1	// Always valid
//#endif
//#ifndef	_U3_SHDN		// No ~SHDN pin
//#define	_U3_SHDN	1	// Always wakeup
//#define	U3_SHDN_SET()	((void)0)
//#define	U3_SHDN_CLR()	((void)0)
//#else
//#define U3_SHDN_SET()	(_U3_SHDN = 1)
//#define U3_SHDN_CLR()	(_U3_SHDN = 0)
//#endif // U3_SHDN
#endif // RP_U3RX

#if defined(RP_U4RX)
#define UART4_USED		// UART4 is used
//#ifndef	_U4_VALID		// No ~INVALID pin
//#define	_U4_VALID	1	// Always valid
//#endif
//#ifndef	_U4_SHDN		// No ~SHDN pin
//#define	_U4_SHDN	1	// Always wakeup
//#define	U4_SHDN_SET()	((void)0)
//#define	U4_SHDN_CLR()	((void)0)
//#else
//#define	U4_SHDN_SET()	(_U4_SHDN = 1)
//#define	U4_SHDN_CLR()	(_U4_SHDN = 0)
//#endif // U4_SHDN
#endif // RP_U4RX

#endif /*_PINSCFG_INCL_*/
