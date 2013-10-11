#include <mathlib.h>

float m_wnrandf(void)
{ // White noise in range -1..1 (float)
	return(2 * ((m_rand()/((float)RAND_MAX)) - 0.5f));
}
