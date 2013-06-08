#include "global.h"
#include <math.h>

#ifndef _COMPILE_WITH_BLACKFIN
#define cabs cabs_custom
#endif

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
		nop += pzk->no_wz;
	}
	if( pzk->no_wz < 0 ) {
		noz -= pzk->no_wz;
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


real getPrewarpFreq(real radps, real samplingTime) {
	return ( radps == 0.0 ) ? ( 2 / samplingTime ) : ( radps / tan(radps * samplingTime / 2) );
}

pzkContainer * bilinear(pzkContainer * pzk, real Fs, real pwf) {
	pzkContainer * f;
	uint i;
	int no_1z;
	complex denom, tmp, one;
	
	one.re = -1.0;
	one.im = 0.0;
	
	tmp.re = pwf;
	pwf = getPrewarpFreq(pwf, 1.0/Fs);
	no_1z = (int)countPoles(pzk) - (int)countZeros(pzk);

	i = countPoles(pzk);		//TODO: finomítani!
	if( i < countZeros(pzk) ) {
		i = countZeros(pzk);
	}
	
	f = createPzkContainer(i, i);
	
	f->pwf = pwf;
	f->no_wz = pzk->no_wz;
	
	if( pzk->wz == 0 ) {
		f->amp = pzk->amp * pow(pwf, (real)pzk->no_wz);
	} else {
		f->amp = pzk->amp * pow(tmp.re/sin(tmp.re/(2*Fs)), (real)(2*pzk->no_wz));
		f->wz = pzk->wz;
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
			denom.re = pwf - pzk->zeros[i].re;
			f->amp *= denom.re;
			tmp.re = (pwf + pzk->zeros[i].re) / denom.re;
			tmp.im = 0.0;
		} else {
			denom = csub2(pwf, pzk->zeros[i]);
			f->amp *= cabs2(denom);
			tmp = cdiv( cadd2(pwf, pzk->zeros[i]), denom );
		}
		addZero(f, tmp);
	}
	
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( cisreal(pzk->poles[i]) ) {
			denom.re = 1 / ( pwf - pzk->poles[i].re );
			f->amp *= denom.re;
			tmp.re = (pwf + pzk->poles[i].re) * denom.re;
			tmp.im = 0.0;
		} else {
			denom = cdiv( one, csub2(pwf, pzk->poles[i]) );
			f->amp *= cabs2(denom);
			tmp = cmlt( cadd2(pwf, pzk->poles[i]), denom );
		}
		addPole(f, tmp);
	}
	
	return f;
}