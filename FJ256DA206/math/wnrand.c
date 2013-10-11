#include <mathlib.h>


double m_wnrand(void)
{ // White noise in range -1..1 (double)
	return(2 * ((m_rand()/((double)RAND_MAX)) - 0.5));
}
