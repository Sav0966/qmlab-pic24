#include <mathlib.h>

float m_grandf(float mean, float std)
{ // Normal deviate (Gaussian distribution)
	// with median and standard deviation
	return(std * m_ndrandf() + mean);
}
