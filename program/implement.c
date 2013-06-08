#include "global.h"
#include <math.h>

#define MAX_FR16	0x7FFF

#ifdef _COMPILE_WITH_BLACKFIN

#include <fract2float_conv.h>

#else

#define float_to_fr16(p)	(int)(32768.0*p)

#endif

real direct1_biquad( fract16 coeffs[], complex * z1, complex * z2, complex * p1, complex * p2, int scale_mod ) {
	real K = 1, tmp1, tmp2, tmp3, tmp4;	
	const real m1pln2 = -1/log(2.0);
	
	if( !cisreal(*p1) ) {
		if( p2 != p1 || p2 != NULL ) {
			error(0);									// if p1 is complex then p2 is assumed to be its conjugate
		}
		coeffs[4] = float_to_fr16( (float)( -cabs2(*p1) ) );
		coeffs[5] = float_to_fr16( (float)( p1->re ) );
		K *= 1-2*p1->re+cabs2(*p1);
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
		
		if( p1->re != 1 ) {
			K *= 1 - p1->re;
		}
		if( p2->re != 1 ) {
			K *= 1 - p2->re;
		}
	}
	
	if( !cisreal(*z1) ) {
		if( z2 != z1 || z2 != NULL ) {
			error(0);									// if z1 is complex then z2 is assumed to be its conjugate
		}
		K /= abs(1-2*z1->re+cabs2(*z1));
		tmp3 = -K*cabs2(*z1);
		tmp2 = K*z1->re;
		tmp1 = -K;
	} else {
		if( z1->re != 1 ) {
			K /= 1 - z1->re;
		}
		if( p1->re != 1 ) {
			K /= 1 - z2->re;
		}
		K = abs(K);
		
		tmp3 = -K * z1->re * z2->re;
		tmp2 = K *( z1->re + z2->re )/2;
	}
	
	tmp1 = -K;
	if( tmp2 < tmp1 ) tmp1 = tmp2;
	if( tmp3 < tmp1 ) tmp1 = tmp3;				// tmp1 = min
	tmp4 = tmp3;
	if( tmp2 > tmp4 ) tmp4 = tmp2;
	if(  -K  > tmp1 ) tmp1 =  -K ;				// tmp4 = max
	
	tmp1 = floor( log(abs(tmp1)) * m1pln2 );
	tmp4 = ceil( log(abs(tmp4)) * m1pln2 ) - 1;
	if( tmp4 < tmp1 ) tmp4 = tmp1;				// max(tmp1, tmp4)
	tmp1 = pow(2, -tmp4);
	tmp3 *= tmp1;
	tmp2 *= tmp1;
	tmp1 *= -K;
	
	coeffs[2] = float_to_fr16( (float)tmp1 );
	coeffs[1] = float_to_fr16( (float)tmp2 );
	coeffs[0] = float_to_fr16( (float)tmp3 );
	coeffs[3] = (int)tmp4;
		
	return K;
}

int implementFilter( filterInfo * fi ) {
	return 1;
}
