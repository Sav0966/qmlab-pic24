/*
*	MCU pins configuration functions
*/
#ifndef _PINS_INCL_
#define _PINS_INCL_
/*
* Use next definitions to redefine default configuration
* Rx - ports RA, RB, RC, RD, RE, RF, RG; n - pin number
*/
// #define Rxn_IN		/* Pin Rxn is input */
// #define Rxn_LOW		/* Pin Rxn is low output */
// #define Rxn_HIGH		/* Pin Rxn is high output */
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

#endif /*_PINS_INCL_*/
