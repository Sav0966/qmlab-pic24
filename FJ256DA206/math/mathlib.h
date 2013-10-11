/*
*	Additional mathematical functions
*/
#ifndef _MATHLIB_INCL_
#define _MATHLIB_INCL_

#include <math.h>
#include <stdlib.h>
/*
*	Random number generators
*/
#define m_rand		rand	// Use std function

double m_wnrand(void);	// White noise in range -1..1 (double)
float m_wnrandf(void);	// White noise in range -1..1 (float)

#endif /*_MATHLIB_INCL_*/
