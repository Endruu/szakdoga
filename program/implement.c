#include "global.h"
#include <math.h>

#ifdef _COMPILE_WITH_BLACKFIN

#include <fract2float_conv.h>

//#define D1BQ_A1	0

void direct1_biquad( fract16 coeffs[], complex * z1, complex * z2, complex * p1, complex * p2, int scale_mod ) {
	real K = 1;
	if( !cisreal(*p1) ) {
		coeffs[0] = float_to_fr16( (float)( -cabs(*p1) ) );
		coeffs[1] = float_to_fr16( (float)( p1->re ) );
	} else {
		coeffs[0] = float_to_fr16( (float)( -cabs(*p1) ) );
		coeffs[1] = float_to_fr16( (float)( p1->re ) );
	}
}

void iDirect1(pzkContainer * pzk)
{
	stages = countBiquads(pzk);

	
	// allocate memory
	delayLine = (fract16 *) malloc(stages * sizeof(fract16));
	
	
	
	
}

#endif
