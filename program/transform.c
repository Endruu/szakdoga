#include "global.h"
#include <math.h>

#ifndef _COMPILE_WITH_BLACKFIN
#define cabs cabs_custom
#endif

transformParameters newTransformParameters() {
	transformParameters r;
	r.w0	= 0;
	r.w1	= 0;
	r.inHz	= 1;
	r.isDw	= 0;
	return r;
}

int normalizeTransformParameters(transformParameters *tp) {
	double tmp1, tmp2;
	int	bw_minimal = 0,
		freq_changed = 0;
	
	if( tp->inHz ) {
		tp->w0 *= 2*PI;
		tp->w1 *= 2*PI;
		tp->inHz = 0;
	}
	
	if( tp->w0 < MIN_FREQUENCY ) {
		warn(0);
		tp->w0 = MIN_FREQUENCY;
	} else if( tp->w0 > MAX_FREQUENCY ) {
		warn(0);
		tp->w0 = MAX_FREQUENCY;
	}
	
	if( tp->w1 ) {
		if( tp->isDw ) {
			if( tp->w1 < MIN_BANDWIDTH ) {
				tp->w1 = MIN_BANDWIDTH;
				warn(0);
				bw_minimal++;
			}
			tmp1 = (sqrt(tp->w1*tp->w1+4*tp->w0*tp->w0) - tp->w1) / 2;
			tmp2 = tmp1 + tp->w1;
			
			if( tmp1 < MIN_FREQUENCY ) {
				if( bw_minimal ) {	//error: bandwidth should be shrunk but it is already minimal
					error(0);
				} else {
					tmp1 = MIN_FREQUENCY;
					freq_changed++;
				}
			}
			if( tmp2 > MAX_FREQUENCY ) {
				if( bw_minimal ) {	//error: bandwidth should be shrunk but it is already minimal
					error(0);
				} else {
					tmp2 = MAX_FREQUENCY;
					freq_changed++;
				}
			}
			
			if( freq_changed ) {
				warn(0);
				tp->w0 = sqrt(tmp1*tmp2);
				tp->w1 = tmp2 - tmp1;
			}
			
		} else {
			if( tp->w0 > tp->w1) {
				tmp1 = tp->w0;
				tp->w0 = tp->w1;
				tp->w1 = tmp1;
			}
			if( tp->w0 < MIN_FREQUENCY ) {
				tp->w0 = MIN_FREQUENCY;
				freq_changed++;
				warn(0);
			}
			if( tp->w1 > MAX_FREQUENCY ) {
				tp->w1 = MAX_FREQUENCY;
				freq_changed++;
				warn(0);
			}
			
			tp->w0 = sqrt(tp->w1*tp->w0);
			tp->w1 = tp->w1 - tp->w0;
			
			if( tp->w1 < MIN_BANDWIDTH ) {
				if( freq_changed ) {	//error: bandwidth should be shrunk but it is already minimal
					error(0);
				} else {
					tp->w1 = MIN_BANDWIDTH;
					warn(0);
				}
			}
		}
	}
	
	return 1;
}

int transformFilter( filterInfo *fi ) {
	pzkContainer * pzk;
	real w0;
	
	w0 = fi->transformP.w0;
	if( fi->iirP.fixWs ) w0 /= fi->iirP.ws;

	switch( fi->type ) {
		case lowpass:
			pzk = t2lp(fi->iFilter, w0);
			break;
		case highpass:
			pzk = t2hp(fi->iFilter, w0);
			break;
		case bandpass:
			pzk = t2bp(fi->iFilter, w0, fi->transformP.w1);
			break;
		case bandstop:
			pzk = t2bp(fi->iFilter, w0, fi->transformP.w1);
			break;
		default:
			error(0);
	}
	
	if( pzk == NULL ) {
		error(0);
	} else {
		fi->tFilter = pzk;
		return 1;
	}
}


pzkContainer * t2lp(pzkContainer * pzk, real w0) {
	pzkContainer * f = createPzkContainer(pzk->nextPole, pzk->nextZero);
	uint i;
	complex tmp;
	
	f->no_wz = pzk->no_wz;
	f->amp *= pzk->amp * pow(w0,(real)(-f->no_wz));

	for( i = 0; i < pzk->nextPole; i++ ) {
		tmp = cmul2(w0, pzk->poles[i]);
		addPole(f, tmp);
		f->amp *= w0;
		if( !cisreal(tmp) ) {
			f->amp *= w0;
		}
	}
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		tmp = cmul2(w0, pzk->zeros[i]);
		addZero(f, tmp);
		if( cisreal(tmp) ) {
			f->amp /= w0;
		} else {
			f->amp /= w0*w0;
		}
	}
	
	f->type = lowpass;
	return f;
	
}

pzkContainer * t2hp(pzkContainer * pzk, real w0) {
	pzkContainer * f = createPzkContainer(pzk->nextPole, pzk->nextZero);
	uint i;
	complex tmp;
	
	f->amp = pzk->amp * pow(w0,(real)pzk->no_wz);
	f->no_wz = -pzk->no_wz;

	for( i = 0; i < pzk->nextPole; i++ ) {
		tmp.re = w0;
		tmp.im = 0;
		f->no_wz++;
		if( cisreal(pzk->poles[i]) ) {
			f->amp /= -pzk->poles[i].re;
			tmp.re /= pzk->poles[i].re;
		} else {
			tmp = cdiv(tmp, pzk->poles[i]);
			f->amp /= cabs2(pzk->poles[i]);
			f->no_wz++;
		}
		addPole(f, tmp);
	}
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		tmp.re = w0;
		tmp.im = 0;
		f->no_wz--;	
		if( cisreal(pzk->zeros[i]) ) {
			f->amp *= -pzk->zeros[i].re;
			tmp.re /= pzk->zeros[i].re;
		} else {
			tmp = cdiv(tmp, pzk->zeros[i]);
			f->amp *= cabs2(pzk->zeros[i]);
			f->no_wz--;
		}
		addZero(f, tmp);
	}
	
	f->type = highpass;
	return f;
	
}

pzkContainer * t2bp(pzkContainer * pzk, real w0, real dw) {
	uint nop, noz, i;
	pzkContainer * f;
	const real dwp2 = dw / 2;
	const real w02 = w0*w0;
	complex tmp, beta, gamma, cw0;
	
	cw0.re = w0;
	cw0.im = 0;
	
	noz = 2*pzk->nextZero;
	nop = 2*pzk->nextPole;
	
	if( pzk->no_wz > 0 ) {
		noz += pzk->no_wz;
	}
	if( pzk->no_wz < 0 ) {
		nop -= pzk->no_wz;
	}
	
	f = createPzkContainer(nop, noz);
	
	f->no_wz = -pzk->no_wz;
	f->amp = pzk->amp * pow(dw, (real)(f->no_wz)+(real)countPoles(pzk)-(real)countZeros(pzk));
		
	gamma.re = 0;
	gamma.im = w0;
	if( pzk->no_wz > 0 ) {
		for( i = 0; i < pzk->no_wz; i++ ) {
			addZero(f, gamma);
		}
	}
	if( pzk->no_wz < 0 ) {
		for( i = 0; i < -pzk->no_wz; i++ ) {
			addPole(f, gamma);
		}
	}

	for( i = 0; i < pzk->nextPole; i++ ) {
		if( cisreal(pzk->poles[i]) ) {
			beta.re = dwp2 * pzk->poles[i].re;
			tmp.re = 1 - ( w02/(beta.re*beta.re) );

			if( tmp.re >= 0 ) {
				tmp.re = sqrt( tmp.re );
				gamma.im = 0;
				gamma.re = beta.re * (1 + tmp.re);
				addPole(f, gamma);
				gamma.re = beta.re * (1 - tmp.re);
				addPole(f, gamma);
			} else {
				tmp.im = sqrt( -tmp.re );
				tmp.re = 1;
				gamma = cmul2( beta.re, tmp );
				addPole(f, gamma);
			}

			f->no_wz++;
		}
		else {
			beta = cmul2(dwp2, pzk->poles[i]);	
			tmp = cdiv(cw0, beta);
			tmp = csqrt( csub2(1, cmlt(tmp, tmp)) );

			gamma = cmlt(beta, cadd2(1, tmp));
			addPole(f, gamma);
			gamma = cmlt(beta, csub2(1, tmp));
			addPole(f, gamma);
			f->no_wz += 2;
		}
	}
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		if( cisreal(pzk->zeros[i]) ) {
			beta.re = dwp2 * pzk->zeros[i].re;
			tmp.re = 1 - ( w02/(beta.re*beta.re) );

			if( tmp.re >= 0 ) {
				tmp.re = sqrt( tmp.re );
				gamma.im = 0;
				gamma.re = beta.re * (1 + tmp.re);
				addZero(f, gamma);
				gamma.re = beta.re * (1 - tmp.re);
				addZero(f, gamma);
			} else {
				tmp.im = sqrt( -tmp.re );
				tmp.re = 1;
				gamma = cmul2( beta.re, tmp );
				addZero(f, gamma);
			}

			f->no_wz--;
		}
		else {
			beta = cmul2(dwp2, pzk->zeros[i]);	
			tmp = cdiv(cw0, beta);
			tmp = csqrt( csub2(1, cmlt(tmp, tmp)) );

			gamma = cmlt(beta, cadd2(1, tmp));
			addZero(f, gamma);
			gamma = cmlt(beta, csub2(1, tmp));
			addZero(f, gamma);
			f->no_wz -= 2;
		}
	}
	
	//shrink
	return f;
	
}

pzkContainer * t2bs(pzkContainer * pzk, real w0, real dw) {
	uint nop, noz, i;
	pzkContainer * f;
	const real w02 = w0*w0;
	complex tmp, beta, gamma, cw0, cdwp2;

	cw0.re = w0;
	cw0.im = 0;

	cdwp2.re = dw / 2;
	cdwp2.im = 0;
	
	noz = 2*pzk->nextZero;
	nop = 2*pzk->nextPole;
	
	if( pzk->no_wz > 0 ) {
		noz += pzk->no_wz;
	}
	if( pzk->no_wz < 0 ) {
		nop -= pzk->no_wz;
	}
	
	f = createPzkContainer(nop, noz);
	
	f->wz = w0;
	f->no_wz = -pzk->no_wz;
	f->amp = pzk->amp * pow(dw, (real)(pzk->no_wz));
		
	gamma.re = 0;
	gamma.im = 0;
	if( pzk->no_wz > 0 ) {
		for( i = 0; i < pzk->no_wz; i++ ) {
			addZero(f, gamma);
		}
	}
	if( pzk->no_wz < 0 ) {
		for( i = 0; i < -pzk->no_wz; i++ ) {
			addPole(f, gamma);
		}
	}

	for( i = 0; i < pzk->nextPole; i++ ) {
		if( cisreal(pzk->poles[i]) ) {
			beta.re = cdwp2.re / pzk->poles[i].re;
			tmp.re = 1 - ( w02/(beta.re*beta.re) );

			if( tmp.re >= 0 ) {
				tmp.re = sqrt( tmp.re );
				gamma.im = 0;
				gamma.re = beta.re * (1 + tmp.re);
				addPole(f, gamma);
				gamma.re = beta.re * (1 - tmp.re);
				addPole(f, gamma);
			} else {
				tmp.im = sqrt( -tmp.re );
				tmp.re = 1;
				gamma = cmul2( beta.re, tmp );
				addPole(f, gamma);
			}

			f->amp /= -pzk->poles[i].re;
			f->no_wz++;
		}
		else {
			beta = cdiv(cdwp2, pzk->poles[i]);	
			tmp = cdiv(cw0, beta);
			tmp = csqrt( csub2(1, cmlt(tmp, tmp)) );

			gamma = cmlt(beta, cadd2(1, tmp));
			addPole(f, gamma);
			gamma = cmlt(beta, csub2(1, tmp));
			addPole(f, gamma);

			f->amp /= cabs2(pzk->poles[i]);
			f->no_wz += 2;
		}
	}
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		if( cisreal(pzk->zeros[i]) ) {
			beta.re = cdwp2.re / pzk->zeros[i].re;
			tmp.re = 1 - ( w02/(beta.re*beta.re) );

			if( tmp.re >= 0 ) {
				tmp.re = sqrt( tmp.re );
				gamma.im = 0;
				gamma.re = beta.re * (1 + tmp.re);
				addZero(f, gamma);
				gamma.re = beta.re * (1 - tmp.re);
				addZero(f, gamma);
			} else {
				tmp.im = sqrt( -tmp.re );
				tmp.re = 1;
				gamma = cmul2( beta.re, tmp );
				addZero(f, gamma);
			}

			f->amp *= -pzk->zeros[i].re;
			f->no_wz--;
		}
		else {
			beta = cdiv(cdwp2, pzk->zeros[i]);	
			tmp = cdiv(cw0, beta);
			tmp = csqrt( csub2(1, cmlt(tmp, tmp)) );

			gamma = cmlt(beta, cadd2(1, tmp));
			addZero(f, gamma);
			gamma = cmlt(beta, csub2(1, tmp));
			addZero(f, gamma);

			f->amp *= cabs2(pzk->zeros[i]);
			f->no_wz -= 2;
		}
	}
	
	return f;	
}

int digitalizeFilter( filterInfo *fi ) {
	pzkContainer * pzk;
	real pwf;
	
	if( fi->warping == WARP_AUTO ) {
		pwf = fi->transformP.w0;
		if( fi->iirP.fixWs ) pwf /= fi->iirP.ws;
		pwf = getPrewarpFreq( pwf );
	} else if( fi->warping == WARP_NORMAL ) {
		pwf = WARP_NORMAL;
	} else {
		pwf = getPrewarpFreq( fi->warping );
	}
	
	pzk = bilinear(fi->tFilter, pwf);
	
	if( pzk == NULL ) {
		error(0);
	} else {
		fi->dFilter = pzk;
		return 1;
	}
}

real getPrewarpFreq(real radps) {
	return ( radps == 0.0 ) ? ( 2 *F_SAMPLING ) : ( radps / tan( radps/(F_SAMPLING * 2) ) );
}

pzkContainer * bilinear(pzkContainer * pzk, real pwf) {
	pzkContainer * f;
	uint i, auto_pwf = 0;
	int no_1z;
	complex denom, tmp, one;
	
	if( pwf == WARP_NORMAL ) auto_pwf = 1;
	
	one.re = -1.0;
	one.im = 0.0;
	
	no_1z = (int)countPoles(pzk) - (int)countZeros(pzk);

	i = countPoles(pzk);
	if( i < countZeros(pzk) ) {
		i = countZeros(pzk);
	}
	f = createPzkContainer(i, i);
	
	f->no_wz = DIGITAL_FILTER;
	f->wz = DIGITAL_FILTER;
	
	if( auto_pwf ) pwf = getPrewarpFreq( pzk->wz );
	if( pzk->wz == 0 ) {
		f->amp = pzk->amp * pow(pwf, (real)pzk->no_wz);
		tmp.re = 1;
		tmp.im = 0;
	} else {
		f->amp = pzk->amp * pow(pwf*pwf + pzk->wz*pzk->wz, (real)(pzk->no_wz));
		tmp.re = pwf;
		tmp.im = pzk->wz;
		tmp = cdiv(tmp,conj(tmp));
	}
	
	if( pzk->no_wz >= 0 ) {
		for( i = 0; i < pzk->no_wz; i++ ) {
			addZero(f, tmp);
		}
	} else {
		for( i = 0; i < -pzk->no_wz; i++ ) {
			addPole(f, tmp);
		}
	}
	
	if(  no_1z >= 0 ) {
		for( i = 0; i < no_1z; i++ ) {
			addZero(f, one);
		}
	} else {					// should not be the case
		for( i = 0; i < -no_1z; i++ ) {
			addPole(f, one);
		}
	}

	one.re = 1.0;

	for( i = 0; i < pzk->nextZero; i++ ) {
		if( cisreal(pzk->zeros[i]) ) {
			if( auto_pwf ) pwf = getPrewarpFreq( pzk->zeros[i].re );
			denom.re = pwf - pzk->zeros[i].re;
			f->amp *= denom.re;
			tmp.re = (pwf + pzk->zeros[i].re) / denom.re;
			tmp.im = 0.0;
		} else {
			if( auto_pwf ) pwf = getPrewarpFreq( cabs(pzk->zeros[i]) );
			denom = csub2(pwf, pzk->zeros[i]);
			f->amp *= cabs2(denom);
			tmp = cdiv( cadd2(pwf, pzk->zeros[i]), denom );
		}
		addZero(f, tmp);
	}
	
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( cisreal(pzk->poles[i]) ) {
			if( auto_pwf ) pwf = getPrewarpFreq( pzk->poles[i].re );
			denom.re = 1 / ( pwf - pzk->poles[i].re );
			f->amp *= denom.re;
			tmp.re = (pwf + pzk->poles[i].re) * denom.re;
			tmp.im = 0.0;
		} else {
			if( auto_pwf ) pwf = getPrewarpFreq( cabs(pzk->poles[i]) );
			denom = cdiv( one, csub2(pwf, pzk->poles[i]) );
			f->amp *= cabs2(denom);
			tmp = cmlt( cadd2(pwf, pzk->poles[i]), denom );
		}
		addPole(f, tmp);
	}
	
	return f;
}
