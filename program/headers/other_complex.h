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

double alog10(double n);
double alog(double n);

#endif
