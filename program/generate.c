#include "global.h"
#include <math.h>

iirParameters newIirParameters() {
	iirParameters r;
	r.ac	= 0;
	r.as	= 0;
	r.ws	= 0;
	r.e0	= 0;
	r.n		= 0;
	r.inDb	= 1;
	return r;
}

/*
Checks and normalizes the input parameters for later use.
Returns	0 if parameters can not be normalized
		1 if parameters are OK
		ws if ws<1, in this case ws has been normalized, so that
			the corner freq. would be 1 and ws higher than this
		-1 or -ws if some of the parameters were out of bounds (should warn)
*/
real normalizeIirParameters(iirParameters *ip) {
	real	tmp;
	int		warn = 1;	// warn-ok cseréje belsõ warn-ra
	//ac as csere ha fordítva adva
	
	if(ip->ac < 0) { ip->ac *= -1; warn = -1; }
	if(ip->as < 0) { ip->as *= -1; warn = -1; }
	
	if( ip->inDb ) {
		if(ip->ac > MAX_DB_INPUT) { ip->ac = MAX_DB_INPUT; warn = -1; }
		if(ip->as > MAX_DB_INPUT) { ip->as = MAX_DB_INPUT; warn = -1; }
	} else {
		if(ip->ac >= 1) return 0;
		if(ip->as >= 1) return 0;
		if( ip->ac && ip->ac < MIN_LIN_INPUT) { ip->ac = MIN_LIN_INPUT; warn = -1; }
		if( ip->as && ip->as < MIN_LIN_INPUT) { ip->as = MIN_LIN_INPUT; warn = -1; }
	}
	
	if( ip->ac && ip->ac == ip->as) return 0;
	
	if( ip->ws ) {
		if(ip->ws < 0) { ip->ws *= -1; warn = -1; }
		
		if(ip->ws == 1) return 0;
		if( ip->ws > MAX_WS_INPUT ) { ip->ws = MAX_WS_INPUT; warn = -1; }
		if(ip->ws < 1) {
			if( ip->ws < 1.0/MAX_WS_INPUT ) { ip->ws = 1/MAX_WS_INPUT; warn = -1; }
			tmp = ip->ws;
			ip->ws = 1 / ip->ws;
			return (real)warn * tmp;
		}
	}
	
	return (real)warn;
		
}

int convertParametersForButterworth( iirParameters * ip ) {
	// createButterworth uses n and e0 parameters
	if( ip->ac ) {
		if( ip->inDb ) {
			ip->e0 = sqrt( alog10(ip->ac/10.0) - 1 );
		} else {
			ip->e0 = sqrt( 1/( ip->ac * ip->ac ) - 1 );
		}
	}
	
	if( ip->n ) {
		if( ip->e0 ) {
			return;
		} else {
			if( ip->ws && ip->as ) {
				if( ip->inDb ) {
					ip->e0 = sqrt( alog10(ip->as/10.0) - 1 );
				} else {
					ip->e0 = sqrt( 1/( ip->as * ip->as ) - 1 );	//átírni
				}
				ip->e0 /= pow( ip->ws, ip->n );
			} else {
				if( ip->ws || ip->as ) {
					//HIBA
				} else {
					ip->e0 = DEFAULT_BW_E0;
				}
			}
		}
	} else {
		if( ip->e0 && ip->ws && ip->as ) {
			if( ip->inDb ) {
				ip->n = (uint)ceil(log(sqrt( alog10(ip->as/10.0) - 1 )/ip->e0)/log(ip->ws));
			} else {
				ip->n = (uint)ceil(log(sqrt( 1/( ip->as * ip->as ) - 1 )/ip->e0)/log(ip->ws));
			}
		} else {
			//HIBA
		}
	}
	
	return 1;
}

pzkContainer * createButterworth(uint n, real e0) {
	pzkContainer * filt = createPzkContainer((n+1)/2, 0);
	real fi;
	complex tmp;
	const real prn = PIP2 / (real)n;	// pi/(2n)
	uint i;
	
	if( e0 != 1.0 ) {
		filt->amp = pow(e0, -(real)n);
	}
	
	for( i = 0; i < n/2; i++ ) {
		fi = (real)( 2*i + 1 + n ) * prn ;
		addPole(filt, polar( filt->amp, fi ) );	//TODO! hibaellenõrzés
	}
	
	if( n % 2 ) {
		tmp.re = -filt->amp;
		tmp.im = 0.0;
		addPole(filt, tmp);
	}
	
	filt->amp = 1 / e0;
	filt->type = lowpass;
	
	return filt;
}


int convertParametersForChebyshev1( iirParameters * ip ) {
	// createChebyshev1 uses n and e0 parameters
	real as, tmp;
	
	if( ip->ac ) {
		if( ip->inDb ) {
			ip->e0 = sqrt( alog10(ip->ac/10.0) - 1 );
		} else {
			ip->e0 = sqrt( 1/( ip->ac * ip->ac ) - 1 );
		}
	}
	
	if( ip->n ) {
		if( ip->e0 ) {
			return;
		} else {
			if( ip->ws && ip->as ) {
				if( ip->inDb ) {
					as = alog10(ip->as/10.0);
				} else {
					as = 1/( ip->as * ip->as );
				}
				tmp = pow( ip->ws + sqrt(ip->ws*ip->ws - 1), n);
				ip->e0 = sqrt(2*tmp*sqrt(as-1) - 1) / tmp;
			} else {
				if( ip->ws || ip->as ) {
					//HIBA
				} else {
					ip->e0 = DEFAULT_C1_E0;
				}
			}
		}
	} else {
		if( ip->e0 && ip->ws && ip->as ) {
			if( ip->inDb ) {
				as = alog10(ip->as/10.0);
			} else {
				as = 1/( ip->as * ip->as );
			}
			tmp = (as-1)/(ip->e0*ip->e0);
			ip->n = (uint)ceil(log( sqrt(tmp) + sqrt(tmp-1) )/log( ip->ws + sqrt(ip->ws*ip->ws - 1) ));
		} else {
			//HIBA
		}
	}
	
	return 1;
}

pzkContainer * createChebyshev1(uint n, real e0) {
	pzkContainer * filt = createPzkContainer((n+1)/2, 0);
	real ch, sh, fi;
	complex tmp;
	const real prn = PIP2 / (real)n;	// pi/(2n)
	uint i;
	
	tmp.re = sqrt(1 + e0*e0);
	tmp.re = (1 + tmp.re) / e0;
	tmp.re = pow(tmp.re, 1.0/(real)n);
	tmp.im = 1 / tmp.re;
	sh = -(tmp.re - tmp.im)/2;
	ch = (tmp.re + tmp.im)/2;
	
	for( i = 0; i < n/2; i++ ) {
		fi = (real)( 2*i + 1 ) * prn ;
		tmp.re = sh * sin(fi);
		tmp.im = ch * cos(fi);
		addPole(filt, tmp);	//TODO! hibaellenõrzés
		filt->amp *= cabs2(tmp);
	}
	
	if( n % 2 ) {
		tmp.re = sh;
		tmp.im = 0.0;
		addPole(filt, tmp);
		filt->amp *= -tmp.re;
	} else {
		filt->amp *= 1 / sqrt(1 + e0*e0);
	}
	
	return filt;
}


int convertParametersForChebyshev2( iirParameters * ip ) {
	// createChebyshev1 uses n, ws and as (linear) parameters
	real tmp;
	
	if( ip->inDb ) {
		if( ip->as ) {
			ip->as = alog( -ip->as / 20 );
		}
		if( ip->ac ) {
			ip->ac = alog( -ip->ac / 20 );
		}
		ip->inDb = 0;
	}
	
	if( ip->n ) {
		if( ip->ws && ip->as) {
				return;
		} else {
			if( ip->ac ) {
				ip->e0 = 1/(ip->ac*ip->ac)-1;
			} else {
				ip->e0 = DEFAULT_C2_E0;
			}
				
			if( ip->ws ) {
				tmp = pow(ip->ws + sqrt(ip->ws*ip->ws - 1), ip->n);
				tmp = (ip->e0*tmp*tmp+1)/(2*tmp);
				ip->as = sqrt( 1/(tmp*tmp+1) );
			} else if( ip->as ) {
				ip->e0 = (1/(ip->as*ip->as)-1)/ip->e0;
				tmp = pow(sqrt(ip->e0) + sqrt(ip->e0-1), 1.0/(real)ip->n);
				ip->ws = (tmp*tmp+1)/(2*tmp);
			} else {
				//HIBA
			}
		}
	} else {
		if( ip->ac && ip->ws && ip->as ) {
			ip->e0 = (1/(ip->as*ip->as)-1)/(1/(ip->ac*ip->ac)-1);
			ip->n = (uint)ceil(log( sqrt(ip->e0) + sqrt(ip->e0-1) )/log( ip->ws + sqrt(ip->ws*ip->ws - 1) ));
		} else {
			//HIBA
		}
	}
	
	return 1;
}

pzkContainer * createChebyshev2(uint n, real Os, real d2) {
	pzkContainer * filt = createPzkContainer((n+1)/2, n/2);
	real ch, sh, fi;
	complex tmp, cOs;
	const real prn = PIP2 / (real)n;	// pi/(2n)
	uint i;

	cOs.re = Os;
	cOs.im = 0;
	
	tmp.re = sqrt(1 - d2*d2);
	tmp.re = (1 + tmp.re) / d2;
	tmp.re = pow(tmp.re, 1.0/(real)n);
	tmp.im = 1 / tmp.re;
	sh = -(tmp.re - tmp.im)/2;
	ch = (tmp.re + tmp.im)/2;
	
	for( i = 0; i < n/2; i++ ) {
		fi = (real)( 2*i + 1 ) * prn ;
		tmp.re = sh * sin(fi);
		tmp.im = ch * cos(fi);
		tmp = cdiv(cOs, tmp);
		addPole(filt, tmp);	//TODO! hibaellenõrzés
		filt->amp *= cabs2(tmp);
		tmp.re = 0;
		tmp.im = Os / cos(fi);
		addZero(filt, tmp);
		filt->amp /= tmp.im * tmp.im;
	}
	
	if( n % 2 ) {
		tmp.re = Os / sh;
		tmp.im = 0.0;
		addPole(filt, tmp);
		filt->amp *= -tmp.re;
	}
	
	return filt;
}

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
