#include "../headers/filterinfo.h"
#include "../headers/iir_filters.h"

//--------------------------------------------------------------------------------------------------------
// filterInfo functions
//--------------------------------------------------------------------------------------------------------

filterInfo defaultIirFilterInfo() {
	filterInfo r;
	
	r.iirP			= defaultIirParameters();
	r.transformP	= defaultTransformParameters();
	r.implementP	= defaultImplementParameters();
	r.filter		= &passThrough;
	r.type			= empty;
	r.subtype		= empty;
	r.supertype		= empty;
	r.ticks			= 0;
	r.mem_coeff		= 0;
	r.mem_delay		= 0;
	
	return r;
}

filterInfo copyFilterInfo( filterInfo * fi ) {
	filterInfo r;
	
	r.iirP			= fi->iirP;
	r.transformP	= fi->transformP;
	r.implementP	= fi->implementP;
	r.filter		= &passThrough;
	r.type			= fi->type;
	r.subtype		= fi->subtype;
	r.supertype		= fi->supertype;
	r.ticks			= 0;
	r.mem_coeff		= 0;
	r.mem_delay		= 0;

	
	return r;
}

// creates a new default parameterset for the frequency transform
transformParameters defaultTransformParameters() {
	transformParameters r;
	
	r.w0	= 0;
	r.w1	= 0;
	r.inHz	= 1;
	r.isDw	= 0;
	return r;
}

// creates a new default parameterset for an IIR filter
iirParameters defaultIirParameters() {
	iirParameters r;
	
	r.ac	= 0;
	r.as	= 0;
	r.ws	= 0;
	r.e0	= 0;
	r.n		= 0;
	r.inDb	= 1;
	r.fixWs	= 0;
	return r;
}

// creates a new default parameterset for an IIR filter
implementParameters defaultImplementParameters() {
	implementParameters r;

	r.pair	= PAIR_POLES_TO_ZEROS;
	r.sort	= SORT_BY_QFACTOR;
	r.order	= ORDER_UP;	
}
