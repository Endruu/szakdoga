#include "global.h"

/*
complex_double polar(double a, double f) {
	complex_double c;
	c.re = a * cos(f);
	c.im = a * sin(f);
	return c;
}

complex_double cdiv(complex_double a, complex_double b) {
	complex_double c;
	double denom = 1/(b.re*b.re + b.im*b.im);
	c.re = ( a.re*b.re + a.im*b.im ) * denom;
	c.im = ( a.im*b.re - a.re*b.im ) * denom;
	return c;
}

double alog10(double e) {
	return exp(e * log(10.0));
}
*/

real cabs2(complex c) {
	return c.re * c.re + c.im * c.im;
}

complex cmul2(real a, complex b) {
	b.re *= a;
	b.im *= a;
	return b;
}

complex csub2(real a, complex b) {
	b.re = a - b.re;
	b.im = -b.im;
	return b;
}

uint cisnull(complex  c) {
	return ( c.re == 0 && c.im == 0) ? 1 : 0;
}

uint cisreal(complex  c) {
	return ( c.im == 0) ? 1 : 0;
}

uint cisimag(complex  c) {
	return ( c.re == 0) ? 1 : 0;
}

uint cisequal(complex  a, complex b) {
	return ( a.im == b.im && a.re == b.re ) ? 1 : 0;
}

