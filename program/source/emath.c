#include "../headers/emath.h"

#ifdef USE_CUSTOM_MATH

//--------------------------------------------------------------------------------------------------------
// Basic functions
//--------------------------------------------------------------------------------------------------------

double			cabs_custom(complex_double a) {
	return sqrt(a.re*a.re+a.im*a.im);
}

complex_double	cadd(complex_double a, complex_double b) {
	a.re += b.re;
	a.im += b.im;
	return a;
}
complex_double	csub(complex_double a, complex_double b) {
	a.re -= b.re;
	a.im -= b.im;
	return a;
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

complex_double	conj(complex_double a) {
	a.im = -a.im;
	return a;
}

double alog10(double n) {
	return pow(10.0,n);
}

double alog(double n) {
	return exp(n);
}

#endif

//--------------------------------------------------------------------------------------------------------
// Extended functions
//--------------------------------------------------------------------------------------------------------


// returns the square of the absolute value of a complex number
real cabs2(complex_double c) {
	return c.re * c.re + c.im * c.im;
}

// multiplies a real and a complex number
complex_double cmul2(real a, complex_double b) {
	b.re *= a;
	b.im *= a;
	return b;
}

// adds a real and a complex number
complex_double cadd2(real a, complex_double b) {
	b.re = a + b.re;
	return b;
}

// subtracts a complex number from a real number
complex_double csub2(real a, complex_double b) {
	b.re = a - b.re;
	b.im = -b.im;
	return b;
}

// returns the square root of a complex number
complex_double csqrt(complex_double c) {
	complex_double ret;
	real mag;
	
	if(c.im == 0) {
		if(c.re >= 0) {
			c.re = sqrt(c.re);
		} else {
			c.im = sqrt(-c.re);
			c.re = 0;
		}
		return c;
	}
	else {
		mag = cabs(c);
		ret.re = sqrt((c.re+mag)/2);
		ret.im = sqrt((-c.re+mag)/2);
		if( c.im < 0 ) {
			ret.im = -ret.im;
		}
		return ret;
	}
	
}

// true if the complex number is zero
uint cisnull(complex_double  c) {
	return ( c.re == 0 && c.im == 0) ? 1 : 0;
}

// true if the imaginary part is zero => the number is real
uint cisreal(complex_double  c) {
	return ( c.im == 0) ? 1 : 0;
}

// true if the number is imaginary
uint cisimag(complex_double  c) {
	return ( c.re == 0) ? 1 : 0;
}

// true if the two complex numbers are equal
uint cisequal(complex_double  a, complex_double b) {
	return ( a.im == b.im && a.re == b.re ) ? 1 : 0;
}