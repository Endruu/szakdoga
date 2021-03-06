#include "../headers/iir_filters.h"
#include "../headers/type_converters.h"

OUTPUT_TYPE passThrough(INPUT_TYPE input, COEFF_TYPE * coeffs, DELAY_TYPE * delays) {
	return input_to_output( input );
}

OUTPUT_TYPE f_direct1_float_postK(INPUT_TYPE input, COEFF_TYPE * coeffs, DELAY_TYPE * delays) {
	float in, acc, K;
	int ic, id, limit;
	float *c, *d;

	c = (float*)coeffs;
	d = (float*)delays;

	limit = (int)c[0]*4-2;
	K = c[1];
	c += 2;

	in = input_to_float(input);
	id = ic = 0;

	acc = in;
	acc += d[id++]*c[ic++];
	d[id-1] = d[id];
	acc += d[id]*c[ic++];
	d[id++] = in;

	while( ic<limit ) {
		acc -= d[id]*c[ic++];
		acc -= d[id+1]*c[ic++];
		in = acc;
		acc += d[id]*c[ic++];
		acc += d[id+1]*c[ic++];
		d[id] = d[id+1];
		d[id+1] = in;
		id += 2;
	}

	acc -= d[id++]*c[ic++];
	d[id-1] = d[id];
	acc -= d[id]*c[ic];
	d[id] = acc;

	acc *= K;

	return float_to_output( acc );
}
