/*
*	Additional mathematical functions
*/
#ifndef _MATHLIB_INCL_
#define _MATHLIB_INCL_

#include <math.h>
#include <stdlib.h>

#ifndef _PI
#define _PI		3.1415926535897932384626433832795
#define _rPI	1.7724538509055160272981674833411
#endif
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
/*
*	2/3-SUM statistical algorithm
*/
double math23_dperiod(unsigned long long s, unsigned int n);
float math23_qmc(unsigned long s, unsigned int n, int nCT3);

#endif /*_MATHLIB_INCL_*/
