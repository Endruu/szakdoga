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

pzkContainer * bilinear(pzkContainer * pzk, real pwf) {
	pzkContainer * f = createPzkContainer(pzk->nextPole, pzk->nextZero);
	uint i;
	real rdenom;
	complex cdenom, tmp, one, cpwf;
	
	one.re = 1.0;
	one.im = 0.0;
	cpwf.re = pwf;
	cpwf.im = 0.0;
	
	
	if( pzk->type != bandstop ) {
		//f->no_wz =
		f->no_0z = -pzk->no_0z;
	} else {
	
		if(pzk->no_0z > 0) {
			for( f->no_0z = 0; f->no_0z < pzk->no_0z; f->no_0z++ ) {
				addZero(f, one);
			}
		}
		
		if(pzk->no_0z < 0) {
			for( f->no_0z = 0; f->no_0z > pzk->no_0z; f->no_0z-- ) {
				addPole(f, one);
			}
		}
		
		//kiegészítésre szorul
		
	}
	
	f->amp = pzk->amp * pow(pwf, pzk->no_0z);
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		if( cisreal(pzk->zeros[i]) ) {
			rdenom = pwf - pzk->zeros[i].re;
			f->amp *= rdenom;
			tmp.re = (pwf + pzk->zeros[i].re) / rdenom;
			tmp.im = 0.0;
			f->no_0z--;
		} else {
			cdenom = csub(one, pzk->zeros[i]);
			f->amp *= cabs2(cdenom);
			tmp = cdiv( cadd(one, pzk->zeros[i]), cdenom );
			f->no_0z -= 2;
		}
		addZero(f, tmp);
	}
	
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( cisreal(pzk->poles[i]) ) {
			rdenom = 1 /( pwf - pzk->poles[i].re );
			f->amp *= rdenom;
			tmp.re = (pwf + pzk->poles[i].re) * rdenom;
			tmp.im = 0.0;
			f->no_0z++;
		} else {
			cdenom = cdiv( one, csub(cpwf, pzk->poles[i]) );
			f->amp *= cabs2(cdenom);
			tmp = cmlt( cadd(cpwf, pzk->poles[i]), cdenom );
			f->no_0z += 2;
		}
		addPole(f, tmp);
	}
	
	return f;
}
