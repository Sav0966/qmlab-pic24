#include <mathlib.h>

double math23_dperiod(unsigned long long s, unsigned int n)
{ // ~T~ = S / (2*_N1*(_N1+1)), return doubled period (n = (1/3)N)
	return((n != 0)? (double)s / ((unsigned long)n * (n+1)): 0.0);
}

float math23_qmc(unsigned long s, unsigned int n, int nCT3)
{ // QMC calculation constants = sqrt(pi)/4; s - QMC sum;
	return( ((float)(_rPI/4)) * (float)s /	// n = (1/3)N
	(((3 * (unsigned long)n + 1 - nCT3) * n) * sqrtf(n + 1)) );
}
