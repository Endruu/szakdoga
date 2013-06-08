#include "global.h"
#include <math.h>

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
	
	f->amp *= pzk->amp * pow(w0,(real)pzk->no_wz);
	f->no_wz = -pzk->no_wz;

	for( i = 0; i < pzk->nextPole; i++ ) {
		tmp.re = w0;
		tmp.im = 0;
		f->no_wz++;
		if( cisreal(pzk->poles[i]) ) {
			f->amp = -f->amp;
			tmp.re /= pzk->poles[i].re;
		} else {
			tmp = cdiv(tmp, pzk->poles[i]);
f->no_wz++;
		}
		addPole(f, tmp);
	}
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		tmp.re = w0;
		tmp.im = 0;
		f->no_wz--;	
		if( cisreal(pzk->zeros[i]) ) {
			f->amp = -f->amp;
			tmp.re /= pzk->zeros[i].re;
		} else {
			tmp = cdiv(tmp, pzk->zeros[i]);
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
	f->amp = pzk->amp * pow(dw, (real)(f->no_wz));
		
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
		beta = cmul2(dwp2, pzk->poles[i]);
		
		tmp = cdiv(cw0, beta);
		tmp = csqrt( csub2(1, cmlt(tmp, tmp)) );
	
		if( cisreal(tmp) ) {
			gamma.im = 0;
			gamma.re = beta.re * (1 + tmp.re);
			addPole(f, gamma);
			gamma.re = beta.re * (1 - tmp.re);
			addPole(f, gamma);
			f->no_wz++;
			f->amp *= beta.re * 2;
		}
		else if( cisimag(tmp) ) {
			gamma = cmlt(beta, cadd2(1, tmp));
			addPole(f, gamma);
			f->no_wz++;
			f->amp *= beta.re * 2;
		}
		else {
			gamma = cmlt(beta, cadd2(1, tmp));
			addPole(f, gamma);
			gamma = cmlt(beta, csub2(1, tmp));
			addPole(f, gamma);
			f->no_wz += 2;
			f->amp *= cabs2(beta) * 4;
		}

	}
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		beta = cmul2(dwp2, pzk->zeros[i]);
		
		tmp = cdiv(cw0, beta);
		tmp = csqrt( csub2(1, cmlt(tmp, tmp)) );
	
		if( cisreal(tmp) ) {
			gamma.im = 0;
			gamma.re = beta.re * (1 + tmp.re);
			addZero(f, gamma);
			gamma.re = beta.re * (1 - tmp.re);
			addZero(f, gamma);
			f->no_wz--;
			f->amp /= beta.re * 2;
		}
		else if( cisimag(tmp) ) {
			gamma = cmlt(beta, cadd2(1, tmp));
			addZero(f, gamma);
			f->no_wz--;
			f->amp /= beta.re * 2;
		}
		else {
			gamma = cmlt(beta, cadd2(1, tmp));
			addZero(f, gamma);
			gamma = cmlt(beta, csub2(1, tmp));
			addZero(f, gamma);
			f->no_wz -= 2;
			f->amp /= cabs2(beta) * 4;
		}
		
	}
	
	//shrink
	return f;
	
}

pzkContainer * t2bs(pzkContainer * pzk, real w0, real dw) {
	uint nop, noz, i;
	pzkContainer * f;
	const real dwp2 = 2.0 / dw;
	complex tmp, beta, gamma;
	
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
		beta = cmul2(dwp2, pzk->poles[i]);
		
		tmp = cmul2(w0, beta);
		tmp = csqrt( csub2(1, cmlt(tmp, tmp)) );
	
		if( cisreal(tmp) ) {
			gamma.im = 0;
			gamma.re = (1 + tmp.re) / beta.re;
			addPole(f, gamma);
			gamma.re = (1 - tmp.re) / beta.re;
			addPole(f, gamma);
			f->no_wz++;
			f->amp = -f->amp;
		}
		else if( cisimag(tmp) ) {
			gamma = cdiv(cadd2(1, tmp), beta);
			addPole(f, gamma);
			f->no_wz++;
			f->amp = -f->amp;
		}
		else {
			gamma = cdiv(cadd2(1, tmp), beta);
			addPole(f, gamma);
			gamma = cdiv(csub2(1, tmp), beta);
			addPole(f, gamma);
			f->no_wz += 2;
		}

	}
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		beta = cmul2(dwp2, pzk->zeros[i]);
		
		tmp = cmul2(w0, beta);
		tmp = csqrt( csub2(1, cmlt(tmp, tmp)) );
	
		if( cisreal(tmp) ) {
			gamma.im = 0;
			gamma.re = (1 + tmp.re) / beta.re;
			addZero(f, gamma);
			gamma.re = (1 - tmp.re) / beta.re;
			addZero(f, gamma);
			f->no_wz--;
			f->amp = -f->amp;
		}
		else if( cisimag(tmp) ) {
			gamma = cdiv(cadd2(1, tmp), beta);
			addZero(f, gamma);
			f->no_wz--;
			f->amp = -f->amp;
		}
		else {
			gamma = cdiv(cadd2(1, tmp), beta);
			addZero(f, gamma);
			gamma = cdiv(csub2(1, tmp), beta);
			addZero(f, gamma);
			f->no_wz -= 2;
		}
		
	}
	
	return f;
	
}


real getPrewarpFreq(real radps, real samplingTime) {
	return ( radps == 0.0 ) ? ( 2 / samplingTime ) : ( radps / tan(radps * samplingTime / 2) );
}

pzkContainer * bilinear(pzkContainer * pzk, real pwf) {
	pzkContainer * f;
	uint i;
	int no_1z;
	real rdenom;
	complex cdenom, tmp, one, cpwf, cwz;
	
	one.re = -1.0;
	one.im = 0.0;
	cpwf.re = pwf;
	cpwf.im = 0.0;
	
	i = pzk->nextPole;
	if( i < pzk->nextZero ) {
		i = pzk->nextZero;
	}
	
	f = createPzkContainer(i, i);
	
	f->pwf = pwf;
	f->no_wz = pzk->no_wz;
	
	if( pzk->wz == 0 ) {
		cwz.re = 1.0;
		cwz.im = 0;
		f->amp = pzk->amp * pow(pwf, pzk->no_wz);
	} else {
		f->wz = pzk->wz;
		tmp.re = pwf;
		tmp.im = pzk->wz;
		cwz = cdiv(tmp, conj(tmp));
		f->amp = pzk->amp * pow(cabs2(cwz), pzk->no_wz);
	}
	
	no_1z = (int)countPoles(pzk) - (int)countZeros(pzk);
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
			rdenom = pwf - pzk->zeros[i].re;
			f->amp *= rdenom;
			tmp.re = (pwf + pzk->zeros[i].re) / rdenom;
			tmp.im = 0.0;
		} else {
			cdenom = csub(one, pzk->zeros[i]);
			f->amp *= cabs2(cdenom);
			tmp = cdiv( cadd(one, pzk->zeros[i]), cdenom );
		}
		addZero(f, tmp);
	}
	
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( cisreal(pzk->poles[i]) ) {
			rdenom = 1 /( pwf - pzk->poles[i].re );
			f->amp *= rdenom;
			tmp.re = (pwf + pzk->poles[i].re) * rdenom;
			tmp.im = 0.0;
		} else {
			cdenom = cdiv( one, csub(cpwf, pzk->poles[i]) );
			f->amp *= cabs2(cdenom);
			tmp = cmlt( cadd(cpwf, pzk->poles[i]), cdenom );
		}
		addPole(f, tmp);
	}
	
	return f;
}
