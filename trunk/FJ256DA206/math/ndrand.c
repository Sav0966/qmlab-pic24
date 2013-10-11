#include <mathlib.h>


double m_ndrand(void)
{ // Normal (or Gaussian) distribution
	static char is_second = 0;
	static double second;

	double u, v, s;

	if (is_second) { // We have it now
		is_second = 0; return(second); 
	}

	do { // Box–Muller transform
		u = m_wnrand(); v = m_wnrand();
		if ((s = u*u + v*v) == 0) continue;
	} while (s > 1); // 78,5% success

	s = sqrt(-2.0 * log(s)/s);
	is_second = 1; second = u * s;

	return(v * s);
}
