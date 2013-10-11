#include <mathlib.h>

double m_grand(double mean, double std)
{ // Normal deviate (Gaussian distribution)
	// with median and standard deviation
	return(std * m_ndrand() + mean);
}
