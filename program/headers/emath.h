#ifndef	_EMATH_H
#define	_EMATH_H

#include <math.h>
#include "types.h"

#ifdef USE_CUSTOM_MATH

//--------------------------------------------------------------------------------------------------------
// Basic functions
//--------------------------------------------------------------------------------------------------------

#define cabs cabs_custom	// workaround for existing cabs function

double			cabs_custom(complex_double a);
complex_double	cadd(complex_double a, complex_double b);
complex_double	csub(complex_double a, complex_double b);
complex_double	cmlt(complex_double a, complex_double b);
complex_double	cdiv(complex_double a, complex_double b);
complex_double	polar(double abs, double fi);
complex_double	conj(complex_double a);

double alog10(double n);
double alog(double n);

#else

#include <complex.h>

#endif	// use custom math

//--------------------------------------------------------------------------------------------------------
// Extended functions
//--------------------------------------------------------------------------------------------------------

real cabs2(complex_double c);
complex_double cmul2(real a, complex_double b);
complex_double cadd2(real a, complex_double b);
complex_double csub2(real a, complex_double b);
complex_double csqrt(complex_double c);

uint cisnull(complex_double  c);
uint cisreal(complex_double  c);
uint cisimag(complex_double  c);
uint cisequal(complex_double  a, complex_double b);

#endif
