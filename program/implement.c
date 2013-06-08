#include "global.h"
#include <math.h>
#include <stdio.h>

#define MAX_FR16	0x7FFF

#ifdef _COMPILE_WITH_BLACKFIN

#include <fract2float_conv.h>

#else

#define float_to_fr16(p)	(int)(32768.0*p)
#define fr16_to_float(p)	((float)p)/32768.0

#endif

real direct1_biquad( fract16 coeffs[], complex * z1, complex * z2, complex * p1, complex * p2, int scale_mod ) {
	real K = 1, tmp1, tmp2, tmp3, tmp4;	
	const real m1pln2 = -1/log(2.0);
	
	if( !cisreal(*p1) ) {
		if( p2 != p1 && p2 != NULL ) {
			error(1);									// if p1 is complex then p2 is assumed to be its conjugate
		}
		coeffs[4] = float_to_fr16( (float)( -cabs2(*p1) ) );
		coeffs[5] = float_to_fr16( (float)( p1->re ) );
	} else {
		tmp1 = -p1->re * p2->re;
		tmp2 = -(p1->re + p2->re)/2;
		if( tmp1 == 1 ) {								// if -p1*p2 = 1	: fract16 can only store [-1,1)
			warn(0);
			if( 1 )	{									// abs p1 and p2 both decreased minimally
				coeffs[4] = MAX_FR16;
				coeffs[5] = 0;
			} else {
				coeffs[4] = MAX_FR16 - 1;
				if( 1 ) {								// pole in 0 is decreased, pole in infinity is maintained
					coeffs[5] = 1;
				} else {								// pole in infinity is decreased, pole in 0 is maintained
					coeffs[5] = -1;
				}
			}
		} else {
			if( tmp2 == 1 ) {							// if p1 = p2 = -1	: fract16 can only store [-1,1)
				warn(0);
				coeffs[5] = MAX_FR16;
				if( 1 )	{
					coeffs[4] = MAX_FR16 - 1;	// p1 decreased, p2 maintained
				}										// else: abs(p1)=abs(p2) p1 = conj(p2)
			} else {
				coeffs[4] = float_to_fr16( (float)tmp1 );
				coeffs[5] = float_to_fr16( (float)tmp2 );
			}
		}
	}
	tmp1 = fr16_to_float( 1-2*coeffs[5]-coeffs[4] );
	if(tmp1 != 0) K *= tmp1; 
	
	if( !cisreal(*z1) ) {
		if( z2 != z1 && z2 != NULL ) {
			error(2);									// if z1 is complex then z2 is assumed to be its conjugate
		}
		tmp1= fabs(1-2*z1->re+cabs2(*z1));
		if(tmp1 != 0) K /= tmp1;
		tmp3 = -K*cabs2(*z1);
		tmp2 = K*z1->re;
		tmp1 = -K;
	} else {
		if( z1->re != 1 ) {
			K /= 1 - z1->re;
		}
		if( z1->re != 1 ) {
			K /= 1 - z2->re;
		}
		K = fabs(K);
		
		tmp3 = -K * z1->re * z2->re;
		tmp2 = K *( z1->re + z2->re )/2;
	}
	
	tmp1 = -K;
	if( tmp2 < tmp1 || tmp1 == 0 ) tmp1 = tmp2;
	if( tmp3 < tmp1 || tmp1 == 0 ) tmp1 = tmp3;				// tmp1 = min
	tmp4 = tmp3;
	if( tmp2 > tmp4 || tmp4 == 0 ) tmp4 = tmp2;
	if(  -K  > tmp4 || tmp4 == 0 ) tmp4 =  -K ;				// tmp4 = max
	
	tmp1 = floor( log(fabs(tmp1)) * m1pln2 );
	tmp4 = ceil( log(fabs(tmp4)) * m1pln2 );
	if( tmp4 > tmp1 ) tmp4 = tmp1;				// min(tmp1, tmp4)
	tmp1 = pow(2, tmp4);
	tmp3 *= tmp1;
	tmp2 *= tmp1;
	tmp1 *= -K;
	
	coeffs[2] = float_to_fr16( (float)tmp1 );
	coeffs[1] = float_to_fr16( (float)tmp2 );
	coeffs[0] = float_to_fr16( (float)tmp3 );
	coeffs[3] = (int)(-tmp4);
		
	return K;
}

real implementFilter( filterInfo * fi ) {
	int i = 0, j = 0, k = 0;
	complex *p1, *p2, *z1, *z2;
	complex zero;
	real tmp;
	real K = fi->dFilter->amp;
	
	if(countZeros(fi->dFilter) != countPoles(fi->dFilter)) {
		error(3);
	}

	zero.re = 0;
	zero.im = 0;
	
	sortDigitalPZ(fi->dFilter);
	while( i < fi->dFilter->nextPole ) {
		if( k+6 > COEFF_SIZE ) {
			error(4);
		}
		p1 = &fi->dFilter->poles[i];
		if( cisreal( *p1 ) ) {
			if( i+1 < fi->dFilter->nextPole ) {
				p2 = &fi->dFilter->poles[i+1];
			} else {
				p2 = &zero;
			}
			i += 2;
		} else {
			p2 = NULL;
			i++;
		}
		z1 = &fi->dFilter->zeros[j];
		if( cisreal( *z1 ) ) {
			if( j+1 < fi->dFilter->nextZero ) {
				z2 = &fi->dFilter->zeros[j+1];
			} else {
				z2 = &zero;
			}
			j += 2;
		} else {
			z2 = NULL;
			j++;
		}
		tmp = direct1_biquad(coeffLineTemp+k, z1, z2, p1, p2, 0);
		if(tmp) {
			K /= tmp;
		} else {
			error(5);
		}
		k += 6;
	}
	
	if( k+2 > COEFF_SIZE ) {
		error(6);
	}
	
	//coeffLineTemp[k] = float_to_fr16( (float)K );
	
	fi->stages = countBiquads(fi->dFilter);
	fi->filter = &direct1;
	
	return K;
}

fract32 passThrough(fract16 * coeffs, fract16 * delays, uint count) {
		//return fr16_to_fr32( delays[0] );
	return delays[0];
}


fract32 direct1(fract16 * coeffs, fract16 * delays, uint count) {
	return delays[0];
}