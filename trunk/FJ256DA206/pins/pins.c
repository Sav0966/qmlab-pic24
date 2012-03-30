#include <p24Fxxxx.h>
#include <config.h>

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

}
