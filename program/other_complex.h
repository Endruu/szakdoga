#ifndef	_OTHER_COMPLEX_H
#define	_OTHER_COMPLEX_H

typedef struct {
	double re, im;
} complex_double;

double			cabs_custom(complex_double a);
complex_double	cadd(complex_double a, complex_double b);
complex_double	csub(complex_double a, complex_double b);
complex_double	cmlt(complex_double a, complex_double b);
complex_double	cdiv(complex_double a, complex_double b);
complex_double	polar(double abs, double fi);
complex_double	conj(complex_double a);

/*
complex_double      cacos(complex_double);
complex_double       cacosh(complex_double);
double               carg(complex_double);
complex_double       casin(complex_double);
complex_double       casinh(complex_double);
complex_double       catan(complex_double);
complex_double       catanh(complex_double);
complex_double       ccos(complex_double);
complex_double       ccosh(complex_double);
complex_double       cexp(complex_double);
double               cimag(complex_double);
complex_double       clog(complex_double);
complex_double       conj(complex_double);
complex_double       cpow(complex_double, complex_double);
complex_double       cproj(complex_double);
double               creal(complex_double);
complex_double       csin(complex_double);
complex_double       csinh(complex_double);
complex_double       csqrt(complex_double);
complex_double       ctan(complex_double);
complex_double       ctanh(complex_double);
*/

#endif
