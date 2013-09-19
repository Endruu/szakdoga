#include "../headers/iir_functions.h"

pzkContainer * digitalizeFilter( filterInfo *fi, pzkContainer * transformed ) {
	pzkContainer * pzk;
	real pwf;
	
	if( fi->warping == WARP_AUTO ) {
		pwf = fi->transformP.w0;
		if( fi->iirP.fixWs ) pwf /= fi->iirP.ws;
		pwf = getPrewarpFactor( pwf );
	} else if( fi->warping == WARP_NORMAL ) {
		pwf = WARP_NORMAL;
	} else {
		pwf = getPrewarpFactor( fi->warping );
	}
	
	pzk = bilinear(transformed, pwf);
	
	if( pzk == NULL ) {
		errorR(36, NULL);
	} else {
		return pzk;
	}
}

real getPrewarpFactor(real radps) {
	return ( radps == 0.0 ) ? ( 2 *F_SAMPLING ) : ( radps / tan( radps/(F_SAMPLING * 2) ) );
}

complex prewarpPZ(complex pz) {
	const real abs = cabs(pz);
	const real f2 = 2*F_SAMPLING;
	return cmul2(f2*atan(abs/f2)/abs, pz);
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
	
	f->no_wz = 0;
	f->wz = DIGITAL_FILTER;
	
	if( auto_pwf ) pwf = getPrewarpFactor( pzk->wz );
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
			if( auto_pwf ) pwf = getPrewarpFactor( pzk->zeros[i].re );
			denom.re = pwf - pzk->zeros[i].re;
			f->amp *= denom.re;
			tmp.re = (pwf + pzk->zeros[i].re) / denom.re;
			tmp.im = 0.0;
		} else {
			if( auto_pwf ) pwf = getPrewarpFactor( cabs(pzk->zeros[i]) );
			denom = csub2(pwf, pzk->zeros[i]);
			f->amp *= cabs2(denom);
			tmp = cdiv( cadd2(pwf, pzk->zeros[i]), denom );
		}
		addZero(f, tmp);
	}
	
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( cisreal(pzk->poles[i]) ) {
			if( auto_pwf ) pwf = getPrewarpFactor( pzk->poles[i].re );
			denom.re = 1 / ( pwf - pzk->poles[i].re );
			f->amp *= denom.re;
			tmp.re = (pwf + pzk->poles[i].re) * denom.re;
			tmp.im = 0.0;
		} else {
			if( auto_pwf ) pwf = getPrewarpFactor( cabs(pzk->poles[i]) );
			denom = cdiv( one, csub2(pwf, pzk->poles[i]) );
			f->amp *= cabs2(denom);
			tmp = cmlt( cadd2(pwf, pzk->poles[i]), denom );
		}
		addPole(f, tmp);
	}
	
	return f;
}
