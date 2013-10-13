#include "../headers/iir_filters.h"
#include "../headers/type_converters.h"
#include <math.h>

fract32 passThrough(fract16 input, fract16 * coeffs, fract16 * delays) {
	return fr16_to_fr32( input );
}

#ifndef _COMPILE_WITH_BLACKFIN
fract32 direct1(fract16 input, fract16 * coeffs, fract16 * delays) {
	return input;
}
#endif

fract32 direct1_float(fract16 input, fract16 * coeffs, fract16 * delays) {
	int	i,
		nextC = 1,
		nextD = 0,
		prevD = 0,
		limit = coeffs[0];

	float acc;
	fract16	tmp = input;

	for( i = 0; i < limit; i++ ) {
		acc = 0;
		acc -= fr16_to_float(coeffs[nextC++]) * fr16_to_float(delays[nextD++]);
		delays[prevD++] = delays[nextD];
		acc -= fr16_to_float(coeffs[nextC]) * fr16_to_float(delays[nextD]);
		acc -= fr16_to_float(coeffs[nextC++]) * fr16_to_float(delays[nextD++]);
		delays[prevD++] = tmp;
		acc -= fr16_to_float(coeffs[nextC++]) * fr16_to_float(tmp);

		acc *= pow( 2.0, (float)(coeffs[nextC++]) );

		
		acc += fr16_to_float(coeffs[nextC++]) * fr16_to_float(delays[nextD++]);
		delays[prevD++] = delays[nextD];
		acc -= fr16_to_float(coeffs[nextC]) * fr16_to_float(delays[nextD]);
		acc -= fr16_to_float(coeffs[nextC++]) * fr16_to_float(delays[nextD++]);
		tmp = float_to_fr16((float)acc);
		delays[prevD++] = tmp;	
	}

	acc *= fr16_to_float(coeffs[nextC]);
	tmp = float_to_fr16( acc );
	return fr16_to_fr32( tmp );
}

fract32 f_direct1_float_postK(fract16 input, COEFF_TYPE * coeffs, DELAY_TYPE * delays) {
	float in, acc, K;
	int ic, id, limit;
	float *c, *d;

	c = (float*)coeffs;
	d = (float*)delays;

	limit = (int)c[0]*4-2;
	K = c[1];
	c += 2;

	in = (float)input;
	id = ic = 0;

	acc = d[id++]*c[ic++];
	d[id-1] = d[id];
	acc += d[id]*c[ic++];
	d[id++] = in;

	while( ic<limit ) {
		acc += d[id]*c[ic++];
		acc += d[id+1]*c[ic++];
		in = acc;
		acc += d[id]*c[ic++];
		acc += d[id+1]*c[ic++];
		d[id] = d[id+1];
		d[id+1] = in;
		id += 2;
	}

	in = acc;
	acc += d[id++]*c[ic++];
	d[id-1] = d[id];
	acc += d[id]*c[ic];
	d[id] = in;

	acc *= K;

	return float_to_fr32( acc );
}
