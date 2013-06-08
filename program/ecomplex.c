#include "global.h"
#include <math.h>

#ifndef _COMPILE_WITH_BLACKFIN
#define cabs cabs_custom
#endif

real cabs2(complex c) {
	return c.re * c.re + c.im * c.im;
}

complex cmul2(real a, complex b) {
	b.re *= a;
	b.im *= a;
	return b;
}

complex cadd2(real a, complex b) {
	b.re = a + b.re;
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
