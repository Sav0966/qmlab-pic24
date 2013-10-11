#include <mathlib.h>

float m_ndrandf(void)
{ // Normal (or Gaussian) distribution
	static char is_second = 0;
	static float second;

	float u, v, s;

	if (is_second) { // We have it now
		is_second = 0; return(second); 
	}

	do { // Box–Muller transform
		u = m_wnrandf(); v = m_wnrandf();
		if ((s = u*u + v*v) == 0) continue;
	} while (s > 1); // 78,5% success

	s = sqrtf(-2.0 * logf(s)/s);
	is_second = 1; second = u * s;

	return(v * s);
}
