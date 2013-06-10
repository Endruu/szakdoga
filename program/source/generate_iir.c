#include "../headers/global.h"
#include <math.h>

//--------------------------------------------------------------------------------------------------------
// IIR parameters preprocessing
//--------------------------------------------------------------------------------------------------------

// creates a new default parameterset for an IIR filter
iirParameters newIirParameters() {
	iirParameters r;
	r.ac	= 0;
	r.as	= 0;
	r.ws	= 0;
	r.e0	= 0;
	r.n		= 0;
	r.inDb	= 1;
	r.fixWs	= 0;
	return r;
}

/*
Checks and normalizes the input parameters for later use.
Returns	0 if parameters can not be normalized
		1 if parameters are OK
*/
int normalizeIirParameters(iirParameters *ip) {
	real	tmp;
	
	if(ip->ac < 0) { ip->ac *= -1; warn(101); }
	if(ip->as < 0) { ip->as *= -1; warn(102); }
	if( ip->ac && ip->ac == ip->as) { error(102); }

	if( ip->inDb ) {
		if(ip->ac > MAX_DB_INPUT) { ip->ac = MAX_DB_INPUT; warn(103); }
		if(ip->as > MAX_DB_INPUT) { ip->as = MAX_DB_INPUT; warn(104); }
		if( ip->ac && ip->as) {
			if(ip->ac > ip->as) {
				tmp = ip->ac;
				ip->ac = ip->as;
				ip->as = tmp;
				warn(105);
			}
		}
	} else {
		if(ip->ac >= 1) { error(100); }
		if(ip->as >= 1) { error(101); }
		if( ip->ac && ip->ac < MIN_LIN_INPUT) { ip->ac = MIN_LIN_INPUT; warn(106); }
		if( ip->as && ip->as < MIN_LIN_INPUT) { ip->as = MIN_LIN_INPUT; warn(107); }
		if( ip->ac && ip->as) {
			if(ip->ac < ip->as) {
				tmp = ip->ac;
				ip->ac = ip->as;
				ip->as = ip->ac;
				warn(108);
			}
		}
	}
	
	if( ip->ws ) {
		if(ip->ws < 0) { ip->ws *= -1; warn(109); }
		
		if(ip->ws == 1) { error(103); }
		if( ip->ws > MAX_WS_INPUT ) { ip->ws = MAX_WS_INPUT; warn(110); }
		if(ip->ws < 1) {
			if( ip->ws < 1.0/MAX_WS_INPUT ) {
				ip->ws = 1/MAX_WS_INPUT;
				warn(111);
			} else {
				ip->ws = 1/ip->ws;
			}
			ip->fixWs = 1;
		}
	}
	
	return 1;
		
}

//--------------------------------------------------------------------------------------------------------
// Creates the referent filter
//--------------------------------------------------------------------------------------------------------

int createReferentFilter( filterInfo *fi ) {
	pzkContainer *pzk, *tmp;

	switch( fi->subtype ) {
		case butterworth:
			if( convertParametersForButterworth( &fi->iirP ) ) {
				pzk = createButterworth(fi->iirP.n, fi->iirP.e0);
			} else {
				error(104);
			}
			break;
			
		case chebyshev1:
			if( convertParametersForChebyshev1( &fi->iirP ) ) {
				pzk = createChebyshev1(fi->iirP.n, fi->iirP.e0);
			} else {
				error(105);
			}
			break;
		case chebyshev2:
			if( convertParametersForChebyshev2( &fi->iirP ) ) {
				pzk = createChebyshev2(fi->iirP.n, fi->iirP.ws, fi->iirP.as);
			} else {
				error(106);
			}
			break;
		default:
			error(107);
	}
	
	if( pzk == NULL ) {
		error(108);
	} else {
		if( fi->iirP.fixWs ) {
			tmp = t2lp(pzk,1/fi->iirP.ws);
			if( tmp != NULL ) {
				fi->iFilter = tmp;
			} else {
				error(109);
			}
		} else {
			fi->iFilter = pzk;
		}
	}
	return 1;
}

//--------------------------------------------------------------------------------------------------------
// BUTTERWORTH
//--------------------------------------------------------------------------------------------------------

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
			return 1;
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
					error(110);
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
			error(111);
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
	
	if( filt == NULL ) {
		errorR(112, NULL);
	}
	
	if( e0 != 1.0 ) {
		filt->amp = pow(e0, -1.0/(real)n);
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

//--------------------------------------------------------------------------------------------------------
// CHEBYSHEV																
//--------------------------------------------------------------------------------------------------------

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
			return 1;
		} else {
			if( ip->ws && ip->as ) {
				if( ip->inDb ) {
					as = alog10(ip->as/10.0);
				} else {
					as = 1/( ip->as * ip->as );
				}
				tmp = pow( ip->ws + sqrt(ip->ws*ip->ws - 1), ip->n);
				ip->e0 = sqrt(2*tmp*sqrt(as-1) - 1) / tmp;
			} else {
				if( ip->ws || ip->as ) {
					error(113);
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
			error(114);
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
	
	if( filt == NULL ) {
		errorR(115, NULL);
	}
	
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

//--------------------------------------------------------------------------------------------------------
// INVERSE CHEBYSHEV																
//--------------------------------------------------------------------------------------------------------

int convertParametersForChebyshev2( iirParameters * ip ) {
	// createChebyshev1 uses n, ws and as (linear) parameters
	real tmp;
	
	if( ip->inDb ) {
		if( ip->as ) {
			ip->as = alog10( -ip->as / 20 );
		}
		if( ip->ac ) {
			ip->ac = alog10( -ip->ac / 20 );
		}
		ip->inDb = 0;
	}
	
	if( ip->n ) {
		if( ip->ws && ip->as) {
				return 1;
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
				error(116);
			}
		}
	} else {
		if( ip->ac && ip->ws && ip->as ) {
			ip->e0 = (1/(ip->as*ip->as)-1)/(1/(ip->ac*ip->ac)-1);
			ip->n = (uint)ceil(log( sqrt(ip->e0) + sqrt(ip->e0-1) )/log( ip->ws + sqrt(ip->ws*ip->ws - 1) ));
		} else {
			error(117);
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
	
	if( filt == NULL ) {
		errorR(118, NULL);
	}

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
