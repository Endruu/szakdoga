#include "global.h"
#include <stdio.h>
#include <math.h>
//#include <string.h>

//#define sscanf sscanf_s

pzkContainer * createButterworth(uint n, real e0) {
	pzkContainer * bw = createPzkContainer((n+1)/2, 0);
	real fi;
	complex tmp;
	const real prn = PIP2 / (real)n;	// pi/(2n)
	uint i;
	
	if( e0 != 1.0 ) {
		bw->amp = pow(e0, -(real)n);
	}
	
	for( i = 0; i < n/2; i++ ) {
		fi = (real)( 2*i + 1 + n ) * prn ;
		addPole(bw, polar( bw->amp, fi ) );	//TODO! hibaellenõrzés
	}
	
	if( n % 2 ) {
		tmp.re = -bw->amp;
		tmp.im = 0.0;
		addPole(bw, tmp);
	}
	
	bw->amp = 1 / e0;
	bw->type = lowpass;
	
	return bw;
}
/*
real transformA2E(real a0, filterType type) {
	switch( type  ) {
		case butterworth:
		case chebyshev1: 
			a0 = sqrt(alog(a0/10.0)-1);
			break;
		case chebyshev2:
			a0 = sqrt(1)
	}
	return a0;
}
*/
void decodeInput(char in[]) {
	filterType type, subtype, shape;
	float wz = 0.0, az, amp = 1.0,  a0 = 0.0, w0, dw;
	uint n, tmp = 0;
	char *ch1 = in;

	switch(in[0]) {
		case 'B' : type = iir; subtype = butterworth; break;
		case 'C' : type = iir; subtype = chebyshev1; break;
		case 'I' : type = iir; subtype = chebyshev2; break;
		case 'X' : type = iir; subtype = custom_iir; break;
	}

	if( type == iir ) {
		if( subtype != custom_iir ) {
			ch1 += 3;
			if(in[1] == 'N') {
				if(in[2] == 'E') {
					if(!sscanf( ch1, "%u%n", &n, &tmp )) {
						//HIBA
					}
				} else if(in[2] == 'A') {
					if(!sscanf( ch1, "%u:%g%n", &n, &a0, &tmp )) {
						//HIBA
					}
				} else {
					//HIBA
				}
			} else if(in[1] == 'W') {
				if(in[2] == 'N') {
					if(!sscanf( ch1, "%g:%g:%u%n", &wz, &az, &n, &tmp )) {
						//HIBA
					}
				} else if(in[2] == 'A') {
					if(!sscanf( ch1, "%g:%g:%g%n", &wz, &az, a0, &tmp )) {
						//HIBA
					}
				} else {
					//HIBA
				}
			} else {
				//HIBA
			}
			ch1 += tmp;
		}

		ch1++;
		switch (ch1[-1]) {
			case 'L':
				if(!sscanf( ch1, "%g%n", &w0, &tmp )) {
					//HIBA
				}
				shape = lowpass;
				break;
			case 'H':
				if(!sscanf( ch1, "%g%n", &w0, &tmp )) {
					//HIBA
				}
				shape = highpass;
				break;
			case 'P':
				if(!sscanf( ch1, "%g:%g%n", &w0, &dw, &tmp )) {
					//HIBA
				}
				shape = bandpass;
				break;
			case 'Q':
				if(!sscanf( ch1, "%g:%g%n", &w0, &dw, &tmp )) {	//w0 = wl, dw = wh
					//HIBA
				}
				amp = dw;			//amp, mint tmp
				dw -= w0;			//dw = sávszél
				w0 = sqrtf(w0*dw);	//w0 = sávközép
				shape = bandpass;
				break;
			case 'S':
				if(!sscanf( ch1, "%g:%g%n", &w0, &dw, &tmp )) {
					//HIBA
				}
				shape = bandstop;
				break;
			case 'T':
				if(!sscanf( ch1, "%g:%g%n", &w0, &dw, &tmp )) {	//w0 = wl, dw = wh
					//HIBA
				}
				amp = dw;			//amp, mint tmp
				dw -= w0;			//dw = sávszél
				w0 = sqrtf(w0*dw);	//w0 = sávközép
				shape = bandstop;
				break;
			case 'D':
				if(subtype == custom_iir) {
					//shape = digital;
				} else {
					//HIBA
				}
				break;
			default:
				;//HIBA
		}

		ch1 += tmp;
		if(ch1[0] == 'K') {
			if(!sscanf( ch1+1, "%g", &amp )) {
					//HIBA
				}
		}
	}	//fir
}
