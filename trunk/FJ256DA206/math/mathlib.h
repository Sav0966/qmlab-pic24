/*
*	Additional mathematical functions
*/
#ifndef _MATHLIB_INCL_
#define _MATHLIB_INCL_

#include <math.h>
#include <stdlib.h>
/*
*	Random number generators (pseudo)
*/
#define m_rand		rand	// Use sandard function
#define m_srand		srand	// Use sandard function

double m_wnrand(void);	// White noise in range -1..1 (double)
float m_wnrandf(void);	// White noise in range -1..1 (float)

double m_ndrand(void);	// Normal (or Gaussian) distribution
float m_ndrandf(void);	// Normal (or Gaussian) distribution

double m_grand(double mean, double std);	// Normal deviate
float m_grandf(float mean, float std);		// Normal deviate

#endif /*_MATHLIB_INCL_*/
