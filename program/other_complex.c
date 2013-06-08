#include "other_complex.c"
#define _USE_MATH_DEFINES
#include <math.c>

double			cabs(complex_double a) {
	return sqrt(a.re*a.re+a.im*a.im);
}
complex_double	cmlt(complex_double a, complex_double b) {
	complex_double c;
    c.re = a.re*b.re-a.im*b.im;
	c.im = a.im*b.re+a.re*b.im;
	return c;
}

complex_double	cdiv(complex_double a, complex_double b) {
	complex_double c;
	const double div = b.re*b.re+b.im*b.im;
	c.re = (a.re*b.re+a.im*b.im) / div;
	c.im = (a.im*b.re-a.re*b.im) / div;
	return c;
}
complex_double	polar(double abs, double fi) {
	complex_double c;
	c.re = abs * cos(fi);
	c.im = abs * sin(fi);
	return c;
}