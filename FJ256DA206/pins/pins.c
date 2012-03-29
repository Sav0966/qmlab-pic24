#include <p24Fxxxx.h>
#include "pins.h"

#define SET_BIT(nbit) | (1 << nbit)

void pins_init(void)
{
#ifdef ANSA // (100-pin devices only)
	ANSA =
 ~(0
 #if (defined(RA6_IN) || defined(RA6_LOW) || defined(RA6_HI))
 SET_BIT(6)
 #endif
 #if (defined(RA7_IN) || defined(RA7_LOW) || defined(RA7_HI))
 SET_BIT(7)
 #endif
 #if (defined(RA9_IN) || defined(RA9_LOW) || defined(RA9_HI))
 SET_BIT(9)
 #endif
 #if (defined(RA10_IN) || defined(RA10_LOW) || defined(RA10_HI))
 SET_BIT(10)
 #endif
 );
#endif // (100-pin devices only)

	ANSB =
~(0
#if (defined(RB0_IN) || defined(RB0_LOW) || defined(RB0_HI))
SET_BIT(0)
#endif
#if (defined(RB1_IN) || defined(RB1_LOW) || defined(RB1_HI))
SET_BIT(1)
#endif
#if (defined(RB2_IN) || defined(RB2_LOW) || defined(RB2_HI))
SET_BIT(2)
#endif
#if (defined(RB3_IN) || defined(RB3_LOW) || defined(RB3_HI))
SET_BIT(3)
#endif
#if (defined(RB4_IN) || defined(RB4_LOW) || defined(RB4_HI))
SET_BIT(4)
#endif
#if (defined(RB5_IN) || defined(RB5_LOW) || defined(RB5_HI))
SET_BIT(5)
#endif
#if (defined(RB6_IN) || defined(RB6_LOW) || defined(RB6_HI))
SET_BIT(6)
#endif
#if (defined(RB7_IN) || defined(RB7_LOW) || defined(RB7_HI))
SET_BIT(7)
#endif
#if (defined(RB8_IN) || defined(RB8_LOW) || defined(RB8_HI))
SET_BIT(8)
#endif
#if (defined(RB9_IN) || defined(RB9_LOW) || defined(RB9_HI))
SET_BIT(9)
#endif
#if (defined(RB10_IN) || defined(RB10_LOW) || defined(RB10_HI))
SET_BIT(10)
#endif
#if (defined(RB11_IN) || defined(RB11_LOW) || defined(RB11_HI))
SET_BIT(11)
#endif
#if (defined(RB12_IN) || defined(RB12_LOW) || defined(RB12_HI))
SET_BIT(12)
#endif
#if (defined(RB13_IN) || defined(RB13_LOW) || defined(RB13_HI))
SET_BIT(13)
#endif
#if (defined(RB14_IN) || defined(RB14_LOW) || defined(RB14_HI))
SET_BIT(14)
#endif
#if (defined(RB15_IN) || defined(RB15_LOW) || defined(RB15_HI))
SET_BIT(15)
#endif
);

	ANSC =
~(0
#ifdef ANSA // (100-pin devices only)
 #if (defined(RC4_IN) || defined(RC4_LOW) || defined(RC4_HI))
 SET_BIT(4)
 #endif
#endif // (100-pin devices only)
#if (defined(RC13_IN) || defined(RC13_LOW) || defined(RC13_HI))
SET_BIT(13)
#endif
#if (defined(RC14_IN) || defined(RC14_LOW) || defined(RC14_HI))
SET_BIT(14)
#endif
);

	ANSD =
~(0
#if (defined(RD6_IN) || defined(RD6_LOW) || defined(RD6_HI))
SET_BIT(6)
#endif
#if (defined(RD7_IN) || defined(RD7_LOW) || defined(RD7_HI))
SET_BIT(7)
#endif
);

#ifdef ANSE // (100-pin devices only)
	ANSE =
 ~(0
 #if (defined(RE9_IN) || defined(RE9_LOW) || defined(RE9_HI))
 SET_BIT(9)
 #endif
 );
#endif //(100-pin devices only)

	ANSF =
~(0
#if (defined(RF0_IN) || defined(RF0_LOW) || defined(RF0_HI))
SET_BIT(0)
#endif
);

	ANSG =
~(0
#if (defined(RG6_IN) || defined(RG6_LOW) || defined(RG6_HI))
SET_BIT(6)
#endif
#if (defined(RG7_IN) || defined(RG7_LOW) || defined(RG7_HI))
SET_BIT(7)
#endif
#if (defined(RG8_IN) || defined(RG8_LOW) || defined(RG8_HI))
SET_BIT(8)
#endif
#if (defined(RG9_IN) || defined(RG9_LOW) || defined(RG9_HI))
SET_BIT(9)
#endif
);

}
