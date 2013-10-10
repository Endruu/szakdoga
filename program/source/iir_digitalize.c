#include "../headers/iir_functions.h"

pzkContainer * digitalizeFilter( filterInfo *fi, pzkContainer * transformed ) {
	pzkContainer * pzk;
	real pwf;										// PreWarp Factor
	
	if( fi->warping == WARP_AUTO_FIX ) {			// fix pwf according to important frequency
		pwf = fi->transformP.w0;
		if( fi->iirP.fixWs ) pwf /= fi->iirP.ws;
		pwf = getPrewarpFactor( pwf );
	} else if( fi->warping == WARP_FACTOR ) {		// dynamic pwf for each pole/zero
		pwf = WARP_FACTOR;
	} else if( fi->warping == WARP_FREQUENCY ) {	// fix pwf (2/Ts), but prewarps the pole/zero
		pwf = WARP_FREQUENCY;
	} else {
		pwf = getPrewarpFactor( fi->warping );		// fix pwf, supplied by user
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
	uint i;
	char auto_pwf = 0;
	complex denom, tmp, one, pz;
	
	if( pwf == WARP_FACTOR ) {
		auto_pwf = 1;
	} else if( pwf == WARP_FREQUENCY ) {
		auto_pwf = 2;
		pwf = getPrewarpFactor(0);
	}
	
	f = createPzkContainer(pzk->nextPole, pzk->nextZero );
	
	f->no_wz = pzk->no_wz;

	pz.re = 0;
	pz.im = pzk->wz;

	if( auto_pwf == 2 ) {
		pz = prewarpPZ(pz);
		if( pzk->wz != 0 ) {
			f->amp = pzk->amp * pow((pzk->wz*pzk->wz)/(pz.im*pz.im), (real)pzk->no_wz);
		}
	}

	if( auto_pwf == 1 ) pwf = getPrewarpFactor( pzk->wz );
	if( pzk->wz == 0 ) {
		f->amp	= pzk->amp * pow(pwf, (real)pzk->no_wz);
		f->wz	= -0;
	} else {
		tmp.re = pwf * pwf;
		tmp.im = pz.im * pz.im;
		f->amp	= pzk->amp * pow(tmp.re + tmp.im, (real)(pzk->no_wz));
		f->wz	= -acos((tmp.re - tmp.im)/(tmp.re + tmp.im));
	}

	one.re = 1.0;
	one.im = 0.0;

	for( i = 0; i < pzk->nextZero; i++ ) {
		pz = pzk->zeros[i];

		if( cisreal(pz) ) {
			if( auto_pwf == 2 ) {
				f->amp *= pz.re;
				pz = prewarpPZ(pz);
				f->amp /= pz.re;
			}
			if( auto_pwf == 1 ) pwf = getPrewarpFactor( pz.re );
			denom.re = pwf - pz.re;
			f->amp *= denom.re;
			tmp.re = (pwf + pz.re) / denom.re;
			tmp.im = 0.0;
		} else {
			if( auto_pwf == 2 ) {
				f->amp *= cabs2(pz);
				pz = prewarpPZ(pz);
				f->amp /= cabs2(pz);
			}
			if( auto_pwf == 1) pwf = getPrewarpFactor( cabs(pz) );
			denom = csub2(pwf, pz);
			f->amp *= cabs2(denom);
			tmp = cdiv( cadd2(pwf, pz), denom );
		}
		addZero(f, tmp);
	}
	
	for( i = 0; i < pzk->nextPole; i++ ) {
		pz = pzk->poles[i];

		if( cisreal(pz) ) {
			if( auto_pwf == 2 ) {
				f->amp /= pz.re;
				pz = prewarpPZ(pz);
				f->amp *= pz.re;
			}
			if( auto_pwf == 1 ) pwf = getPrewarpFactor( pz.re );
			denom.re = 1 / ( pwf - pz.re );
			f->amp *= denom.re;
			tmp.re = (pwf + pz.re) * denom.re;
			tmp.im = 0.0;
		} else {
			if( auto_pwf == 2 ) {
				f->amp /= cabs2(pz);
				pz = prewarpPZ(pz);
				f->amp *= cabs2(pz);
			}
			if( auto_pwf == 1 ) pwf = getPrewarpFactor( cabs(pz) );
			denom = cdiv( one, csub2(pwf, pz) );
			f->amp *= cabs2(denom);
			tmp = cmlt( cadd2(pwf, pz), denom );
		}
		addPole(f, tmp);
	}
	
	return f;
}
