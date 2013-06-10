#include "../headers/global.h"
#include <math.h>

#ifndef _COMPILE_WITH_BLACKFIN
#define cabs cabs_custom
#endif

// returns the square of the absolute value of a complex number
real cabs2(complex c) {
	return c.re * c.re + c.im * c.im;
}

// multiplies a real and a complex number
complex cmul2(real a, complex b) {
	b.re *= a;
	b.im *= a;
	return b;
}

// adds a real and a complex number
complex cadd2(real a, complex b) {
	b.re = a + b.re;
	return b;
}

// subtracts a complex number from a real number
complex csub2(real a, complex b) {
	b.re = a - b.re;
	b.im = -b.im;
	return b;
}

// returns the square root of a complex number
complex csqrt(complex c) {
	complex ret;
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
uint cisnull(complex  c) {
	return ( c.re == 0 && c.im == 0) ? 1 : 0;
}

// true if the imaginary part is zero => the number is real
uint cisreal(complex  c) {
	return ( c.im == 0) ? 1 : 0;
}

// true if the number is imaginary
uint cisimag(complex  c) {
	return ( c.re == 0) ? 1 : 0;
}

// true if the two complex numbers are equal
uint cisequal(complex  a, complex b) {
	return ( a.im == b.im && a.re == b.re ) ? 1 : 0;
}
