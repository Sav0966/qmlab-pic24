#include <p24Fxxxx.h>
#include <pinscfg.h>

#define SET_BIT(nbit) | (1 << nbit)

void pins_init(void)
{
#ifdef _ANSA6 // (100-pin devices only)
	ANSA = ~(0
 #if (defined(RA6_IN) || defined(RA6_LOW) || defined(RA6_HIGH))
 SET_BIT(6)
 #endif
 #if (defined(RA7_IN) || defined(RA7_LOW) || defined(RA7_HIGH))
 SET_BIT(7)
 #endif
 #if (defined(RA9_IN) || defined(RA9_LOW) || defined(RA9_HIGH))
 SET_BIT(9)
 #endif
 #if (defined(RA10_IN) || defined(RA10_LOW) || defined(RA10_HIGH))
 SET_BIT(10)
 #endif
 );
#endif // (100-pin devices only)

	ANSB = ~(0
#if (defined(RB0_IN) || defined(RB0_LOW) || defined(RB0_HIGH))
SET_BIT(0)
#endif
#if (defined(RB1_IN) || defined(RB1_LOW) || defined(RB1_HIGH))
SET_BIT(1)
#endif
#if (defined(RB2_IN) || defined(RB2_LOW) || defined(RB2_HIGH))
SET_BIT(2)
#endif
#if (defined(RB3_IN) || defined(RB3_LOW) || defined(RB3_HIGH))
SET_BIT(3)
#endif
#if (defined(RB4_IN) || defined(RB4_LOW) || defined(RB4_HIGH))
SET_BIT(4)
#endif
#if (defined(RB5_IN) || defined(RB5_LOW) || defined(RB5_HIGH))
SET_BIT(5)
#endif
#if (defined(RB6_IN) || defined(RB6_LOW) || defined(RB6_HIGH))
SET_BIT(6)
#endif
#if (defined(RB7_IN) || defined(RB7_LOW) || defined(RB7_HIGH))
SET_BIT(7)
#endif
#if (defined(RB8_IN) || defined(RB8_LOW) || defined(RB8_HIGH))
SET_BIT(8)
#endif
#if (defined(RB9_IN) || defined(RB9_LOW) || defined(RB9_HIGH))
SET_BIT(9)
#endif
#if (defined(RB10_IN) || defined(RB10_LOW) || defined(RB10_HIGH))
SET_BIT(10)
#endif
#if (defined(RB11_IN) || defined(RB11_LOW) || defined(RB11_HIGH))
SET_BIT(11)
#endif
#if (defined(RB12_IN) || defined(RB12_LOW) || defined(RB12_HIGH))
SET_BIT(12)
#endif
#if (defined(RB13_IN) || defined(RB13_LOW) || defined(RB13_HIGH))
SET_BIT(13)
#endif
#if (defined(RB14_IN) || defined(RB14_LOW) || defined(RB14_HIGH))
SET_BIT(14)
#endif
#if (defined(RB15_IN) || defined(RB15_LOW) || defined(RB15_HIGH))
SET_BIT(15)
#endif
);

	ANSC = ~(0
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RC4_IN) || defined(RC4_LOW) || defined(RC4_HIGH))
 SET_BIT(4)
 #endif
#endif // (100-pin devices only)
#if (defined(RC13_IN) || defined(RC13_LOW) || defined(RC13_HIGH))
SET_BIT(13)
#endif
#if (defined(RC14_IN) || defined(RC14_LOW) || defined(RC14_HIGH))
SET_BIT(14)
#endif
);

	ANSD = ~(0
#if (defined(RD6_IN) || defined(RD6_LOW) || defined(RD6_HIGH))
SET_BIT(6)
#endif
#if (defined(RD7_IN) || defined(RD7_LOW) || defined(RD7_HIGH))
SET_BIT(7)
#endif
);

#ifdef _ANSA6 // (100-pin devices only)
	ANSE = ~(0
 #if (defined(RE9_IN) || defined(RE9_LOW) || defined(RE9_HIGH))
 SET_BIT(9)
 #endif
 );
#endif //(100-pin devices only)

	ANSF = ~(0
#if (defined(RF0_IN) || defined(RF0_LOW) || defined(RF0_HIGH))
SET_BIT(0)
#endif
);

	ANSG = ~(0
#if (defined(RG6_IN) || defined(RG6_LOW) || defined(RG6_HIGH))
SET_BIT(6)
#endif
#if (defined(RG7_IN) || defined(RG7_LOW) || defined(RG7_HIGH))
SET_BIT(7)
#endif
#if (defined(RG8_IN) || defined(RG8_LOW) || defined(RG8_HIGH))
SET_BIT(8)
#endif
#if (defined(RG9_IN) || defined(RG9_LOW) || defined(RG9_HIGH))
SET_BIT(9)
#endif
);

#ifdef _ANSA6 // (100-pin devices only)
	LATA = (0
 #if (defined(RA0_HIGH))
 SET_BIT(0)
 #endif
 #if (defined(RA1_HIGH))
 SET_BIT(1)
 #endif
 #if (defined(RA2_HIGH))
 SET_BIT(2)
 #endif
 #if (defined(RA3_HIGH))
 SET_BIT(3)
 #endif
 #if (defined(RA4_HIGH))
 SET_BIT(4)
 #endif
 #if (defined(RA5_HIGH))
 SET_BIT(5)
 #endif
 #if (defined(RA6_HIGH))
 SET_BIT(6)
 #endif
 #if (defined(RA7_HIGH))
 SET_BIT(7)
 #endif
 #if (defined(RA9_HIGH))
 SET_BIT(9)
 #endif
 #if (defined(RA10_HIGH))
 SET_BIT(10)
 #endif
 #if (defined(RA14_HIGH))
 SET_BIT(14)
 #endif
 #if (defined(RA15_HIGH))
 SET_BIT(15)
 #endif
 );
#endif // (100-pin devices only)

	LATB = (0
#if (defined(RB0_HIGH))
SET_BIT(0)
#endif
#if (defined(RB1_HIGH))
SET_BIT(1)
#endif
#if (defined(RB2_HIGH))
SET_BIT(2)
#endif
#if (defined(RB3_HIGH))
SET_BIT(3)
#endif
#if (defined(RB4_HIGH))
SET_BIT(4)
#endif
#if (defined(RB5_HIGH))
SET_BIT(5)
#endif
#if (defined(RB6_HIGH))
SET_BIT(6)
#endif
#if (defined(RB7_HIGH))
SET_BIT(7)
#endif
#if (defined(RB8_HIGH))
SET_BIT(8)
#endif
#if (defined(RB9_HIGH))
SET_BIT(9)
#endif
#if (defined(RB10_HIGH))
SET_BIT(10)
#endif
#if (defined(RB11_HIGH))
SET_BIT(11)
#endif
#if (defined(RB12_HIGH))
SET_BIT(12)
#endif
#if (defined(RB13_HIGH))
SET_BIT(13)
#endif
#if (defined(RB14_HIGH))
SET_BIT(14)
#endif
#if (defined(RB15_HIGH))
SET_BIT(15)
#endif
);

	LATC = (0
#if (defined(RC1_HIGH))
SET_BIT(1)
#endif
#if (defined(RC2_HIGH))
SET_BIT(2)
#endif
#if (defined(RC3_HIGH))
SET_BIT(3)
#endif
#if (defined(RC4_HIGH))
SET_BIT(4)
#endif
#if (defined(RC12_HIGH))
SET_BIT(12)
#endif
#if (defined(RC13_HIGH))
SET_BIT(13)
#endif
#if (defined(RC14_HIGH))
SET_BIT(14)
#endif
#if (defined(RC15_HIGH))
SET_BIT(15)
#endif
);

	LATD = (0
#if (defined(RD0_HIGH))
SET_BIT(0)
#endif
#if (defined(RD1_HIGH))
SET_BIT(1)
#endif
#if (defined(RD2_HIGH))
SET_BIT(2)
#endif
#if (defined(RD3_HIGH))
SET_BIT(3)
#endif
#if (defined(RD4_HIGH))
SET_BIT(4)
#endif
#if (defined(RD5_HIGH))
SET_BIT(5)
#endif
#if (defined(RD6_HIGH))
SET_BIT(6)
#endif
#if (defined(RD7_HIGH))
SET_BIT(7)
#endif
#if (defined(RD8_HIGH))
SET_BIT(8)
#endif
#if (defined(RD9_HIGH))
SET_BIT(9)
#endif
#if (defined(RD10_HIGH))
SET_BIT(10)
#endif
#if (defined(RD11_HIGH))
SET_BIT(11)
#endif
#if (defined(RD12_HIGH))
SET_BIT(12)
#endif
#if (defined(RD13_HIGH))
SET_BIT(13)
#endif
#if (defined(RD14_HIGH))
SET_BIT(14)
#endif
#if (defined(RD15_HIGH))
SET_BIT(15)
#endif
);

	LATE = (0
#if (defined(RE0_HIGH))
SET_BIT(0)
#endif
#if (defined(RE1_HIGH))
SET_BIT(1)
#endif
#if (defined(RE2_HIGH))
SET_BIT(2)
#endif
#if (defined(RE3_HIGH))
SET_BIT(3)
#endif
#if (defined(RE4_HIGH))
SET_BIT(4)
#endif
#if (defined(RE5_HIGH))
SET_BIT(5)
#endif
#if (defined(RE6_HIGH))
SET_BIT(6)
#endif
#if (defined(RE7_HIGH))
SET_BIT(7)
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RE8_HIGH))
 SET_BIT(8)
 #endif
 #if (defined(RE9_HIGH))
 SET_BIT(9)
 #endif
#endif // (100-pin devices only)
);

	LATF = (0
#if (defined(RF0_HIGH))
SET_BIT(0)
#endif
#if (defined(RF1_HIGH))
SET_BIT(1)
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RF2_HIGH))
 SET_BIT(2)
 #endif
#endif // (100-pin devices only)
#if (defined(RF3_HIGH))
SET_BIT(3)
#endif
#if (defined(RF4_HIGH))
SET_BIT(4)
#endif
#if (defined(RF5_HIGH))
SET_BIT(5)
#endif
#if (defined(RF7_HIGH))
SET_BIT(7)
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RF8_HIGH))
 SET_BIT(8)
 #endif
 #if (defined(RF12_HIGH))
 SET_BIT(12)
 #endif
 #if (defined(RF13_HIGH))
 SET_BIT(13)
 #endif
#endif // (100-pin devices only)
);

	LATG = (0
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RG0_HIGH))
 SET_BIT(0)
 #endif
 #if (defined(RG1_HIGH))
 SET_BIT(1)
 #endif
#endif // (100-pin devices only)
#if (defined(RG2_HIGH))
SET_BIT(2)
#endif
#if (defined(RG3_HIGH))
SET_BIT(3)
#endif
#if (defined(RG6_HIGH))
SET_BIT(6)
#endif
#if (defined(RG7_HIGH))
SET_BIT(7)
#endif
#if (defined(RG8_HIGH))
SET_BIT(8)
#endif
#if (defined(RG9_HIGH))
SET_BIT(9)
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RG12_HIGH))
 SET_BIT(12)
 #endif
 #if (defined(RG13_HIGH))
 SET_BIT(13)
 #endif
 #if (defined(RG14_HIGH))
 SET_BIT(14)
 #endif
 #if (defined(RG15_HIGH))
 SET_BIT(15)
 #endif
#endif // (100-pin devices only)
);

#ifdef _ANSA6 // (100-pin devices only)
	TRISA = ~(0
 #if (defined(RA0_HIGH) || defined(RA0_LOW))
 SET_BIT(0)
 #endif
 #if (defined(RA1_HIGH) || defined(RA1_LOW))
 SET_BIT(1)
 #endif
 #if (defined(RA2_HIGH) || defined(RA2_LOW))
 SET_BIT(2)
 #endif
 #if (defined(RA3_HIGH) || defined(RA3_LOW))
 SET_BIT(3)
 #endif
 #if (defined(RA4_HIGH) || defined(RA4_LOW))
 SET_BIT(4)
 #endif
 #if (defined(RA5_HIGH) || defined(RA5_LOW))
 SET_BIT(5)
 #endif
 #if (defined(RA6_HIGH) || defined(RA6_LOW))
 SET_BIT(6)
 #endif
 #if (defined(RA7_HIGH) || defined(RA7_LOW))
 SET_BIT(7)
 #endif
 #if (defined(RA9_HIGH) || defined(RA9_LOW))
 SET_BIT(9)
 #endif
 #if (defined(RA10_HIGH) || defined(RA10_LOW))
 SET_BIT(10)
 #endif
 #if (defined(RA14_HIGH) || defined(RA14_LOW))
 SET_BIT(14)
 #endif
 #if (defined(RA15_HIGH) || defined(RA15_LOW))
 SET_BIT(15)
 #endif
 );
#endif // (100-pin devices only)

	TRISB = ~(0
#if (defined(RB0_HIGH) || defined(RB0_LOW))
SET_BIT(0)
#endif
#if (defined(RB1_HIGH) || defined(RB1_LOW))
SET_BIT(1)
#endif
#if (defined(RB2_HIGH) || defined(RB2_LOW))
SET_BIT(2)
#endif
#if (defined(RB3_HIGH) || defined(RB3_LOW))
SET_BIT(3)
#endif
#if (defined(RB4_HIGH) || defined(RB4_LOW))
SET_BIT(4)
#endif
#if (defined(RB5_HIGH) || defined(RB5_LOW))
SET_BIT(5)
#endif
#if (defined(RB6_HIGH) || defined(RB6_LOW))
SET_BIT(6)
#endif
#if (defined(RB7_HIGH) || defined(RB7_LOW))
SET_BIT(7)
#endif
#if (defined(RB8_HIGH) || defined(RB8_LOW))
SET_BIT(8)
#endif
#if (defined(RB9_HIGH) || defined(RB9_LOW))
SET_BIT(9)
#endif
#if (defined(RB10_HIGH) || defined(RB10_LOW))
SET_BIT(10)
#endif
#if (defined(RB11_HIGH) || defined(RB11_LOW))
SET_BIT(11)
#endif
#if (defined(RB12_HIGH) || defined(RB12_LOW))
SET_BIT(12)
#endif
#if (defined(RB13_HIGH) || defined(RB13_LOW))
SET_BIT(13)
#endif
#if (defined(RB14_HIGH) || defined(RB14_LOW))
SET_BIT(14)
#endif
#if (defined(RB15_HIGH) || defined(RB15_LOW))
SET_BIT(15)
#endif
);

	TRISC = ~(0
#if (defined(RC1_HIGH) || defined(RC1_LOW))
SET_BIT(1)
#endif
#if (defined(RC2_HIGH) || defined(RC2_LOW))
SET_BIT(2)
#endif
#if (defined(RC3_HIGH) || defined(RC3_LOW))
SET_BIT(3)
#endif
#if (defined(RC4_HIGH) || defined(RC4_LOW))
SET_BIT(4)
#endif
#if (defined(RC12_HIGH) || defined(RC12_LOW))
SET_BIT(12)
#endif
#if (defined(RC13_HIGH) || defined(RC13_LOW))
SET_BIT(13)
#endif
#if (defined(RC14_HIGH) || defined(RC14_LOW))
SET_BIT(14)
#endif
#if (defined(RC15_HIGH) || defined(RC15_LOW))
SET_BIT(15)
#endif
);

	TRISD = ~(0
#if (defined(RD0_HIGH) || defined(RD0_LOW))
SET_BIT(0)
#endif
#if (defined(RD1_HIGH) || defined(RD1_LOW))
SET_BIT(1)
#endif
#if (defined(RD2_HIGH) || defined(RD2_LOW))
SET_BIT(2)
#endif
#if (defined(RD3_HIGH) || defined(RD3_LOW))
SET_BIT(3)
#endif
#if (defined(RD4_HIGH) || defined(RD4_LOW))
SET_BIT(4)
#endif
#if (defined(RD5_HIGH) || defined(RD5_LOW))
SET_BIT(5)
#endif
#if (defined(RD6_HIGH) || defined(RD6_LOW))
SET_BIT(6)
#endif
#if (defined(RD7_HIGH) || defined(RD7_LOW))
SET_BIT(7)
#endif
#if (defined(RD8_HIGH) || defined(RD8_LOW))
SET_BIT(8)
#endif
#if (defined(RD9_HIGH) || defined(RD9_LOW))
SET_BIT(9)
#endif
#if (defined(RD10_HIGH) || defined(RD10_LOW))
SET_BIT(10)
#endif
#if (defined(RD11_HIGH) || defined(RD11_LOW))
SET_BIT(11)
#endif
#if (defined(RD12_HIGH) || defined(RD12_LOW))
SET_BIT(12)
#endif
#if (defined(RD13_HIGH) || defined(RD13_LOW))
SET_BIT(13)
#endif
#if (defined(RD14_HIGH) || defined(RD14_LOW))
SET_BIT(14)
#endif
#if (defined(RD15_HIGH) || defined(RD15_LOW))
SET_BIT(15)
#endif
);

	TRISE = ~(0
#if (defined(RE0_HIGH) || defined(RE0_LOW))
SET_BIT(0)
#endif
#if (defined(RE1_HIGH) || defined(RE1_LOW))
SET_BIT(1)
#endif
#if (defined(RE2_HIGH) || defined(RE2_LOW))
SET_BIT(2)
#endif
#if (defined(RE3_HIGH) || defined(RE3_LOW))
SET_BIT(3)
#endif
#if (defined(RE4_HIGH) || defined(RE4_LOW))
SET_BIT(4)
#endif
#if (defined(RE5_HIGH) || defined(RE5_LOW))
SET_BIT(5)
#endif
#if (defined(RE6_HIGH) || defined(RE6_LOW))
SET_BIT(6)
#endif
#if (defined(RE7_HIGH) || defined(RE7_LOW))
SET_BIT(7)
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RE8_HIGH) || defined(RE8_LOW))
 SET_BIT(8)
 #endif
 #if (defined(RE9_HIGH) || defined(RE9_LOW))
 SET_BIT(9)
 #endif
#endif // (100-pin devices only)
);

	TRISF = ~(0
#if (defined(RF0_HIGH) || defined(RF0_LOW))
SET_BIT(0)
#endif
#if (defined(RF1_HIGH) || defined(RF1_LOW))
SET_BIT(1)
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RF2_HIGH) || defined(RF2_LOW))
 SET_BIT(2)
 #endif
#endif // (100-pin devices only)
#if (defined(RF3_HIGH) || defined(RF3_LOW))
SET_BIT(3)
#endif
#if (defined(RF4_HIGH) || defined(RF4_LOW))
SET_BIT(4)
#endif
#if (defined(RF5_HIGH) || defined(RF5_LOW))
SET_BIT(5)
#endif
#if (defined(RF7_HIGH) || defined(RF7_LOW))
SET_BIT(7)
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RF8_HIGH) || defined(RF8_LOW))
 SET_BIT(8)
 #endif
 #if (defined(RF12_HIGH) || defined(RF12_LOW))
 SET_BIT(12)
 #endif
 #if (defined(RF13_HIGH) || defined(RF13_LOW))
 SET_BIT(13)
 #endif
#endif // (100-pin devices only)
);

	TRISG = ~(0
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RG0_HIGH) || defined(RG0_LOW))
 SET_BIT(0)
 #endif
 #if (defined(RG1_HIGH) || defined(RG1_LOW))
 SET_BIT(1)
 #endif
#endif // (100-pin devices only)
#if (defined(RG2_HIGH) || defined(RG2_LOW))
SET_BIT(2)
#endif
#if (defined(RG3_HIGH) || defined(RG3_LOW))
SET_BIT(3)
#endif
#if (defined(RG6_HIGH) || defined(RG6_LOW))
SET_BIT(6)
#endif
#if (defined(RG7_HIGH) || defined(RG7_LOW))
SET_BIT(7)
#endif
#if (defined(RG8_HIGH) || defined(RG8_LOW))
SET_BIT(8)
#endif
#if (defined(RG9_HIGH) || defined(RG9_LOW))
SET_BIT(9)
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #if (defined(RG12_HIGH) || defined(RG12_LOW))
 SET_BIT(12)
 #endif
 #if (defined(RG13_HIGH) || defined(RG13_LOW))
 SET_BIT(13)
 #endif
 #if (defined(RG14_HIGH) || defined(RG14_LOW))
 SET_BIT(14)
 #endif
 #if (defined(RG15_HIGH) || defined(RG15_LOW))
 SET_BIT(15)
 #endif
#endif // (100-pin devices only)
);

	// Unlock Registers RPORx & RPINRx
	asm volatile(
		"MOV #OSCCON, w1 \n"
		"MOV #0x46, w2 \n"
		"MOV #0x57, w3 \n"
		"MOV.b w2, [w1] \n"
		"MOV.b w3, [w1] \n"
		"BCLR OSCCON, #6");

#ifndef RP0_OUT
 #define RP0_OUT	RP_NULL
#endif
#ifndef RP1_OUT
 #define RP1_OUT	RP_NULL
#endif
	RPOR0 = RP0_OUT | (RP1_OUT << 8);

#ifndef RP2_OUT
 #define RP2_OUT	RP_NULL
#endif
#ifndef RP3_OUT
 #define RP3_OUT	RP_NULL
#endif
	RPOR1 = RP2_OUT | (RP3_OUT << 8);

#ifndef RP4_OUT
 #define RP4_OUT	RP_NULL
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #ifndef RP5_OUT
  #define RP5_OUT	RP_NULL
 #endif
#else // (64-pin devices only)
 #undef RP5_OUT
 #define RP5_OUT	RP_NULL
#endif
	RPOR2 = RP4_OUT | (RP5_OUT << 8);

#ifndef RP6_OUT
 #define RP6_OUT	RP_NULL
#endif
#ifndef RP7_OUT
 #define RP7_OUT	RP_NULL
#endif
	RPOR3 = RP6_OUT | (RP7_OUT << 8);

#ifndef RP8_OUT
 #define RP8_OUT	RP_NULL
#endif
#ifndef RP9_OUT
 #define RP9_OUT	RP_NULL
#endif
	RPOR4 = RP8_OUT | (RP9_OUT << 8);

#ifndef RP10_OUT
 #define RP10_OUT	RP_NULL
#endif
#ifndef RP11_OUT
 #define RP11_OUT	RP_NULL
#endif
	RPOR5 = RP10_OUT | (RP11_OUT << 8);

#ifndef RP12_OUT
 #define RP12_OUT	RP_NULL
#endif
#ifndef RP13_OUT
 #define RP13_OUT	RP_NULL
#endif
	RPOR6 = RP12_OUT | (RP13_OUT << 8);

#ifndef RP14_OUT
 #define RP14_OUT	RP_NULL
#endif
#ifdef _ANSA6 // (100-pin devices only)
 #ifndef RP15_OUT
  #define RP15_OUT	RP_NULL
 #endif
#else // (64-pin devices only)
 #undef RP15_OUT
 #define RP15_OUT	RP_NULL
#endif
	RPOR7 = RP14_OUT | (RP15_OUT << 8);

#ifndef RP16_OUT
 #define RP16_OUT	RP_NULL
#endif
#ifndef RP17_OUT
 #define RP17_OUT	RP_NULL
#endif
	RPOR8 = RP16_OUT | (RP17_OUT << 8);

#ifndef RP18_OUT
 #define RP18_OUT	RP_NULL
#endif
#ifndef RP19_OUT
 #define RP19_OUT	RP_NULL
#endif
	RPOR9 = RP18_OUT | (RP19_OUT << 8);

#ifndef RP20_OUT
 #define RP20_OUT	RP_NULL
#endif
#ifndef RP21_OUT
 #define RP21_OUT	RP_NULL
#endif
	RPOR10 = RP20_OUT | (RP21_OUT << 8);

#ifndef RP22_OUT
 #define RP22_OUT	RP_NULL
#endif
#ifndef RP23_OUT
 #define RP23_OUT	RP_NULL
#endif
	RPOR11 = RP22_OUT | (RP23_OUT << 8);

#ifndef RP24_OUT
 #define RP24_OUT	RP_NULL
#endif
#ifndef RP25_OUT
 #define RP25_OUT	RP_NULL
#endif
	RPOR12 = RP24_OUT | (RP25_OUT << 8);

#ifndef RP26_OUT
 #define RP26_OUT	RP_NULL
#endif
#ifndef RP27_OUT
 #define RP27_OUT	RP_NULL
#endif
	RPOR13 = RP26_OUT | (RP27_OUT << 8);

#ifndef RP28_OUT
 #define RP28_OUT	RP_NULL
#endif
#ifndef RP29_OUT
 #define RP29_OUT	RP_NULL
#endif
	RPOR14 = RP28_OUT | (RP29_OUT << 8);

#ifdef _ANSA6 // (100-pin devices only)
 #ifndef RP30_OUT
  #define RP30_OUT	RP_NULL
 #endif
 #ifndef RP31_OUT
  #define RP31_OUT	RP_NULL
 #endif
	RPOR15 = RP30_OUT | (RP31_OUT << 8);
#endif // (100-pin devices only)

#ifndef RP_INT1
 #define RP_INT1 	RPI_NULL
#endif
	RPINR0 = RP_INT1 << 8;

#ifndef RP_INT2
 #define RP_INT2 	RPI_NULL
#endif
#ifndef RP_INT3
 #define RP_INT3 	RPI_NULL
#endif
	RPINR1 = RP_INT2 | (RP_INT3 << 8);

#ifndef RP_INT4
 #define RP_INT4 	RPI_NULL
#endif
	RPINR2 = RP_INT4;

#ifndef RP_T2CK
 #define RP_T2CK 	RPI_NULL
#endif
#ifndef RP_T3CK
 #define RP_T3CK 	RPI_NULL
#endif
	RPINR3 = RP_T2CK | (RP_T3CK << 8);

#ifndef RP_T4CK
 #define RP_T4CK 	RPI_NULL
#endif
#ifndef RP_T5CK
 #define RP_T5CK 	RPI_NULL
#endif
	RPINR4 = RP_T4CK | (RP_T5CK << 8);

#ifndef RP_IC1
 #define RP_IC1 	RPI_NULL
#endif
#ifndef RP_IC2
 #define RP_IC2 	RPI_NULL
#endif
	RPINR7 = RP_IC1 | (RP_IC2 << 8);

#ifndef RP_IC3
 #define RP_IC3 	RPI_NULL
#endif
#ifndef RP_IC4
 #define RP_IC4 	RPI_NULL
#endif
	RPINR8 = RP_IC3 | (RP_IC4 << 8);

#ifndef RP_IC5
 #define RP_IC5 	RPI_NULL
#endif
#ifndef RP_IC6
 #define RP_IC6 	RPI_NULL
#endif
	RPINR9 = RP_IC5 | (RP_IC6 << 8);

#ifndef RP_IC7
 #define RP_IC7 	RPI_NULL
#endif
#ifndef RP_IC8
 #define RP_IC8 	RPI_NULL
#endif
	RPINR10 = RP_IC7 | (RP_IC8 << 8);

#ifndef RP_OCFA
 #define RP_OCFA 	RPI_NULL
#endif
#ifndef RP_OCFB
 #define RP_OCFB 	RPI_NULL
#endif
	RPINR11 = RP_OCFA | (RP_OCFB << 8);

#ifndef RP_IC9
 #define RP_IC9 	RPI_NULL
#endif
	RPINR15 = RP_IC9 << 8;

#ifndef RP_U3RX
 #define RP_U3RX 	RPI_NULL
#endif
	RPINR17 = RP_U3RX << 8;

#ifndef RP_U1RX
 #define RP_U1RX 	RPI_NULL
#endif
#ifndef RP_U1CTS
 #define RP_U1CTS 	RPI_NULL
#endif
	RPINR18 = RP_U1RX | (RP_U1CTS << 8);

#ifndef RP_U2RX
 #define RP_U2RX 	RPI_NULL
#endif
#ifndef RP_U2CTS
 #define RP_U2CTS 	RPI_NULL
#endif
	RPINR19 = RP_U2RX | (RP_U2CTS << 8);

#ifndef RP_SDI1
 #define RP_SDI1 	RPI_NULL
#endif
#ifndef RP_SCK1IN
 #define RP_SCK1IN 	RPI_NULL
#endif
	RPINR20 = RP_SDI1 | (RP_SCK1IN << 8);

#ifndef RP_SS1IN
 #define RP_SS1IN 	RPI_NULL
#endif
#ifndef RP_U3CTS
 #define RP_U3CTS 	RPI_NULL
#endif
	RPINR21 = RP_SS1IN | (RP_U3CTS << 8);

#ifndef RP_SDI2
 #define RP_SDI2 	RPI_NULL
#endif
#ifndef RP_SCK2IN
 #define RP_SCK2IN 	RPI_NULL
#endif
	RPINR22 = RP_SDI2 | (RP_SCK2IN << 8);

#ifndef RP_SS2IN
 #define RP_SS2IN 	RPI_NULL
#endif
	RPINR23 = RP_SS2IN;

#ifndef RP_U4RX
 #define RP_U4RX 	RPI_NULL
#endif
#ifndef RP_U4CTS
 #define RP_U4CTS 	RPI_NULL
#endif
	RPINR27 = RP_U4RX | (RP_U4CTS << 8);

#ifndef RP_SDI3
 #define RP_SDI3 	RPI_NULL
#endif
#ifndef RP_SCK3IN
 #define RP_SCK3IN 	RPI_NULL
#endif
	RPINR28 = RP_SDI3 | (RP_SCK3IN << 8);

#ifndef RP_SS3IN
 #define RP_SS3IN 	RPI_NULL
#endif
	RPINR29 = RP_SS3IN;

	// Lock Registers
	asm volatile(
		"MOV #OSCCON, w1 \n"
		"MOV #0x46, w2 \n"
		"MOV #0x57, w3 \n"
		"MOV.b w2, [w1] \n"
		"MOV.b w3, [w1] \n"
		"BSET OSCCON, #6");
}
